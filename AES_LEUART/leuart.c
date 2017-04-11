/*
 * leuart.c
 *
 *  Created on: Mar 11, 2017
 *      Author: vidursarin
 */
#include "leuart.h"
#include "sleep_modes.h"
#include "em_timer.h"

#define DATA_BUFFER_SIZE 5
#define BAUD_RATE 9600
#define SEL_SLEEP_MODE    sleepEM2
#define LEUART_SLEEP_MODE sleepEM2

#define USE_LEUART_DMA

#define AES_DATA_SIZE 64
#define DMA_CHNL0_LEUART0 0

#define delay(X) for(int i=0; i<X; i++)

uint8_t ret_data;
uint8_t rx_test_buffer[AES_DATA_SIZE];
uint8_t count = 0;

extern uint8_t aes_data_counter;
extern c_buf aes_buffer;

/* Function: void Setup_LEUART(void)
 * Parameters:
 *      void
 * Return:
 *      void
 * Description:
 *    - Do the initial setup for the LEUART
 */
void Setup_LEUART(void)
{

  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInputPull, 1);
  
  static LEUART_Init_TypeDef init_leuart = {
    .baudrate   = BAUD_RATE,        /* 9600 bits/s. */
    .databits   = leuartDatabits8,  /* 8 databits. */
    .enable     = leuartDisable,    /* Disable RX/TX when init completed. */
    .parity     = leuartNoParity,   /* No parity. */ 
    .refFreq    = 0,                /* Use current configured reference clock for configuring baudrate. */
    .stopbits   = leuartStopbits1   /* 1 stopbit. */
  };

  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
  CMU_ClockEnable(cmuClock_LEUART0, true);
  
  /* Do not prescale clock */
  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

  /* Configure LEUART */
  init_leuart.enable = leuartDisable;

  /* In order to be cautious, reset the LEUART and start the init. procedures */
  LEUART_Init(LEUART0, &init_leuart);
  
  /* Enable pins at default location */
  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;
  
  /* Clear the interrupt flags */
  LEUART0->IFC = 0xFF;

  /* Set RXDMAWU & TXDMAWU to wake up the DMA controller in EM2 */
  LEUART_RxDmaInEM2Enable(LEUART0, true);
  LEUART_TxDmaInEM2Enable(LEUART0, true);

  /* Enable the interrupt flag for the LEUART 
   * - Enable the interrupt on the completion of the TX.
   */
#ifdef USE_LEUART_DMA
  /* Don't need to enable LEUART interrupts here! */
#else
  /* Enable the LEUART0 interrupts */
  LEUART0->IEN = LEUART_IEN_TXC;
  NVIC_EnableIRQ(LEUART0_IRQn);
#endif

  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);

  return;
}

/* Function: void Setup_LEUART_DMA(void)
 * Parameters:
 *      void
 * Return:
 *      void
 * Description:
 *    - Do the setup for the LEUART to work with DMA
 */
void Setup_LEUART_DMA(void)
{
  /* Config the callback routine properties */
  static DMA_CB_TypeDef dma_cb_config = {
    .cbFunc = cb_Chnl0_DMA,
    .userPtr = NULL,
    .primary = true
  };
  
  /* Initializing the DMA */
  static DMA_Init_TypeDef dmaInit = {
    .controlBlock = dmaControlBlock, 
    .hprot        = DMA_CHNL0_LEUART0
  };

  /*Setting up DMA channel */
  static DMA_CfgChannel_TypeDef channelCfg = {
    .cb         = &dma_cb_config, 
    .select     = DMAREQ_LEUART0_TXBL,
    .enableInt  = true,  
    .highPri    = true
  };

  /* Setting up channel descriptor */
  static DMA_CfgDescr_TypeDef descrCfg = {
    .dstInc   = dmaDataIncNone,
    .srcInc   = dmaDataInc1,
    .size     = dmaDataSize1,
    .arbRate  = dmaArbitrate1, 
    .hprot    = 0
  };

  /* Call all the initialization functions now */
  DMA_Init(&dmaInit);
  DMA_CfgChannel(DMA_CHNL0_LEUART0, &channelCfg);
  DMA_CfgDescr(DMA_CHNL0_LEUART0, true, &descrCfg);
  
  DMA_IntClear(DMA_CHNL0_LEUART0);
  DMA_IntEnable(DMA_CHNL0_LEUART0);

  return;
}

void cb_Chnl0_DMA(unsigned int channel, bool primary, void *user)
{
	INT_Disable();
	
  /* Clear the Flag */
	DMA_IntClear(DMA_CHNL0_LEUART0);

	/* Clear the TX Buffer */
	LEUART0->CMD = LEUART_CMD_CLEARTX;
	LEUART0->CMD = LEUART_CMD_CLEARRX;

	/* Disable the DMA controller */
	DMA->CONFIG &= ~DMA_CONFIG_EN;

	INT_Enable();
}

/* Function:void LEUART0_IRQHandler(void)
 * Parameters:
 *      void
 * Return:
 *      void
 * Description:
 *    - Interrupt handler for LEUART0. This will be triggered on ever
 *      successful TXC event.
 */
void LEUART0_IRQHandler(void)
{
  INT_Disable();
 	
  /* Clear the TXC flag */
	LEUART0->IFC = LEUART_IFC_TXC;

  /* Wait for the data to be received on the RX buffer */
	delay(100);
	rx_test_buffer[aes_data_counter] = LEUART0->RXDATA;
	aes_data_counter++;

	if(aes_data_counter != AES_DATA_SIZE) {
		/* Continue to push data on the tx bus */
		remove_from_buffer(&aes_buffer, &ret_data, sizeof(uint8_t));
		LEUART0->TXDATA = ret_data;
	} else {
		/* Reset the aes_byte counter*/
		aes_data_counter = 0;
	}

  INT_Enable();

  return;
}
