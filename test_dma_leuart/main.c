/**************************************************************************//**
 * @file
 * @brief LEUART/LDMA in EM2 example for SLSTK3401A starter kit
 * @version 5.1.2
 ******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_int.h"
#include "em_leuart.h"
#include "em_ldma.h"
#include "bspconfig.h"
#include "retargetserial.h"

#include "em_crypto.h"

#define AES_DATA_SIZE 64

uint8_t Storage_Buffer[AES_DATA_SIZE];

const uint8_t exampleKey[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                               0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

uint8_t decryptionKey[16];

uint8_t pop_data[64];

const uint8_t initVector[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

/** LDMA Descriptor initialization */
static LDMA_Descriptor_t xfer =
  LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(&LEUART0->RXDATA, /* Peripheral source address */
                                   &Storage_Buffer,  /* Peripheral destination address */
                                   AES_DATA_SIZE,    /* Number of bytes */
                                   0);               /* Link to same descriptor */

/**************************************************************************//**
 * @brief  Setting up LEUART
 *****************************************************************************/
void setupLeuart(void)
{
  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);

  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 0);

  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
  CMU_ClockEnable(cmuClock_LEUART0, true);

  /* Do not prescale clock */
  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

  /* Configure LEUART */
  init.enable = leuartDisable;

  LEUART_Init(LEUART0, &init);

  /* Enable pins at default location */
  LEUART0->ROUTELOC0 = (LEUART0->ROUTELOC0 & ~(_LEUART_ROUTELOC0_TXLOC_MASK
                                               | _LEUART_ROUTELOC0_RXLOC_MASK))
                       | (RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT)
                       | (RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT);

  LEUART0->ROUTEPEN  = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;

  /* Set RXDMAWU to wake up the DMA controller in EM2 */
  LEUART_RxDmaInEM2Enable(LEUART0, true);

  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);
}

/*******************************************************************************//**
 * @brief  Setting up LDMA
 *
 * @details
 *   This function configures LDMA transfer trigger to LEUART0 Rx.
 *   It also disables LDMA interrupt on transfer complete and makes the
 *   destination address increment to None. Since the we are using
 *   LDMA_DESCRIPTOR_LINKREL_P2M_BYTE as the channel descriptor and it enables
 *   LDMA interrupt and destination increment to a none-zero value
 *   ldmaCtrlDstIncOne. In the end this function initializes LDMA ch0 using
 *   channel descriptor(xfer) and the transfer trigger.
 *********************************************************************************/
void setupLdma(void)
{
  /* LDMA transfer configuration for LEUART */
  const LDMA_TransferCfg_t periTransferRx =
    LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_RXDATAV);

  xfer.xfer.dstInc  = ldmaCtrlDstIncOne;

  /* Set the bit to trigger the DMA Done Interrupt */
  xfer.xfer.doneIfs = 1;

  /* LDMA initialization mode definition */
  LDMA_Init_t init = LDMA_INIT_DEFAULT;

  /* Enable the interrupts */
  LDMA->IEN = 0x01;
  NVIC_EnableIRQ(LDMA_IRQn);

  /* LDMA initialization */
  LDMA_Init(&init);
  LDMA_StartTransfer(0, (LDMA_TransferCfg_t *)&periTransferRx, &xfer);
}

void LDMA_IRQHandler(void)
{
	INT_Enable();

	/* Clear the LDMA DONE flag */
	LDMA->IFC = LDMA_IFC_DONE_DEFAULT;

	/* Decrypt the Data received */
	CRYPTO_AES_CBC128(CRYPTO,\
			pop_data,\
			Storage_Buffer,\
			64,\
			decryptionKey,\
			initVector,\
			false);

	INT_Disable();
}

void Setup_CRYPTO(void)
{
  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  /* Enable the clock to the CRYPTO engine */
  CMU->HFBUSCLKEN0 = CMU_HFBUSCLKEN0_CRYPTO;

  /* Get the decryption key from the original key, needs to be done once for each key */
  CRYPTO_AES_DecryptKey128(CRYPTO, decryptionKey, exampleKey);
  
  return;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Use default settings for DCDC */
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator with kit specific parameters */
  EMU_DCDCInit(&dcdcInit);

  Setup_CRYPTO();

  /* Initialize LEUART */
  setupLeuart();

  /* Configure LDMA */
  setupLdma();

  while (1)
  {
    /* On every wakeup enter EM2 again */
    //EMU_EnterEM2(true);
  }
}
