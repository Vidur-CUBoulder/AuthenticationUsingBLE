/**************************************************************************//**
 * @file main.c
 * @brief LEUART/DMA in EM2 example for EFM32LG_DK3750 starter kit
 * @version 5.0.0
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
#include "em_leuart.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "em_int.h"

void cb_Chnl0_DMA(unsigned int channel, bool primary, void *user);

/** LEUART Rx/Tx Port/Pin Location */
#define LEUART_LOCATION    0
#define LEUART_TXPORT      gpioPortD            /* LEUART transmission port */
#define LEUART_TXPIN       4                    /* LEUART transmission pin */
#define LEUART_RXPORT      gpioPortD            /* LEUART reception port */
#define LEUART_RXPIN       5                    /* LEUART reception pin */

uint8_t Tx_Buffer[] = {0x32, 0x41, 0x13, 0x46, 0x63, 0x9A, 0x43};
uint8_t Rx_Buffer[10];
uint8_t count;


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
  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInputPull, 1);

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
  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;
  
  /* Set RXDMAWU to wake up the DMA controller in EM2 */
  LEUART_TxDmaInEM2Enable(LEUART0, true);
  LEUART_RxDmaInEM2Enable(LEUART0, true);
  
  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);
}

/**************************************************************************//**
 * @brief  Setup DMA
 * 
 * @details
 *   This function initializes DMA controller.
 *   It configures the DMA channel to be used for LEUART0 transmit 
 *   and receive. The primary descriptor for channel0 is configured for 
 *   a single data byte transfer. For continous data reception and transmission 
 *   using LEUART DMA loopmode is enabled for channel0. 
 *   In the end DMA transfer cycle is configured to basicMode where 
 *   the channel source address, destination address, and 
 *   the transfercount per dma cycle have been specified.
 *   
 *****************************************************************************/
void setupDma(void)
{
  static DMA_CB_TypeDef dma_cb_config = {
    .cbFunc = cb_Chnl0_DMA,
    .userPtr = NULL,
    .primary = true
  };

  static DMA_Init_TypeDef dmaInit = {
    
    /* Initializing the DMA */
    .hprot        = 0,
    .controlBlock = dmaControlBlock
  };
  
  /* Setting up channel */
  static DMA_CfgChannel_TypeDef channelCfg = {

    /* Can't use with peripherals */
    .highPri   = true,
	/* Interrupt not needed in loop transfer mode */
    .enableInt = true,

    /* Configure channel 0 */
    /*Setting up DMA transfer trigger request*/
    .select = DMAREQ_LEUART0_TXBL,
    .cb     = &dma_cb_config
  };
  
  /* Setting up channel descriptor */
  /* Destination is LEUART_Tx register and doesn't move */
  static DMA_CfgDescr_TypeDef descrCfg = {
    .dstInc = dmaDataIncNone,

    /* Source is LEUART_RX register and transfers 8 bits each time */
    .srcInc = dmaDataInc1,
    .size   = dmaDataSize1,

    /* We have time to arbitrate again for each sample */
    .arbRate = dmaArbitrate1,
    .hprot   = 0
  };
  
  static DMA_CfgLoop_TypeDef loopCfg = {
    /* Configure loop transfer mode */
    .enable = true,
    .nMinus1 = 0  /* Single transfer per DMA cycle*/
  };

  DMA_Init(&dmaInit);
  DMA_CfgChannel(0, &channelCfg);
  
  /* Configure primary descriptor  */
  DMA_CfgDescr(0, true, &descrCfg);
 
  DMA_CfgLoop(0, &loopCfg);
  
  /*Clear and enable the DMA interrupt */
  DMA_IntClear(0);
  DMA_IntEnable(0);

  /* Activate basic dma cycle using channel0 */
  DMA_ActivateBasic(0,
		    true,
		    false,
	        (void *)&LEUART0->TXDATA,
		    &Tx_Buffer,
		    1);
}

void cb_Chnl0_DMA(unsigned int channel, bool primary, void *user)
{
	INT_Disable();

	/* Clear the Flag */
	DMA_IntClear(0);

	/* Store the RX value in a buffer */
	Rx_Buffer[count] = LEUART0->RXDATA;
	count++;

	/* Clear the TX Buffer */
	LEUART0->CMD = LEUART_CMD_CLEARTX;
	LEUART0->CMD = LEUART_CMD_CLEARRX;

	INT_Enable();

}


/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Initialize LEUART */
  setupLeuart();
  
  /* Setup DMA */
  setupDma();
  
  while (1)
  {
    /* On every wakeup enter EM2 again */
    //EMU_EnterEM2(true);
  }
}
