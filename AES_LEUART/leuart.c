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

#define AES_DATA_SIZE 64
extern c_buf aes_buffer;

#define delay(X) for(int i=0; i<X; i++)

//extern uint8_t *data_buffer[DATA_BUFFER_SIZE];
//extern uint8_t transfer_bytes = 0;
//extern uint8_t counter;

uint8_t ret_data;
uint8_t rx_count = 0;

extern uint8_t aes_data_counter;
uint8_t rx_test_buffer[AES_DATA_SIZE];

/* Select TIMER0 parameters */
TIMER_Init_TypeDef timerInit =
{
  .enable     = true,
  .debugRun   = true,
  .prescale   = timerPrescale1024,
  .clkSel     = timerClkSelHFPerClk,
  .fallAction = timerInputActionNone,
  .riseAction = timerInputActionNone,
  .mode       = timerModeUp,
  .dmaClrAct  = false,
  .quadModeX4 = false,
  .oneShot    = false,
  .sync       = false,
};

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
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 1);

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

  /* Configure LEUART */
  init_leuart.enable = leuartDisable;

  /* In order to be cautious, reset the LEUART and start the init. procedures */
  LEUART_Reset(LEUART0);
  LEUART_Init(LEUART0, &init_leuart);
  
  /* Enable pins at default location */
  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;
  
  /* Clear the interrupt flags */
  LEUART0->IFC = 0xFF;
 
  /* Enable the interrupt flag for the LEUART 
   * - Enable the interrupt on the completion of the TX.
   */
  LEUART0->IEN = LEUART_IEN_TXC;

  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);
  NVIC_EnableIRQ(LEUART0_IRQn);

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
  /* Initializing the DMA */
  static DMA_Init_TypeDef dmaInit = {
    .controlBlock = dmaControlBlock, 
    .hprot        = 0
  };

  /*Setting up DMA channel */
  static DMA_CfgChannel_TypeDef channelCfg = {
    .cb         = NULL, 
    .enableInt  = false,  
    .highPri    = false,
    .select     = DMAREQ_LEUART0_RXDATAV
  };

  /* Setting up channel descriptor */
  static DMA_CfgDescr_TypeDef descrCfg = {
    .arbRate  = dmaArbitrate1, 
    .dstInc   = dmaDataIncNone,
    .hprot    = 0,
    .size     = dmaDataSize1,
    .srcInc   = dmaDataIncNone
  };
 
  /* Configure loop transfer mode */
  static DMA_CfgLoop_TypeDef loopCfg = {
    .enable   = true,  
    .nMinus1  = 0     /* Single transfer per DMA cycle */
  };

  /* Call all the initialization functions now */
  DMA_Init(&dmaInit);
  DMA_CfgChannel(0, &channelCfg);
  DMA_CfgDescr(0, true, &descrCfg);
  DMA_CfgLoop(0, &loopCfg);

  /* Activate basic dma cycle using channel0 */
  DMA_ActivateBasic(0,\
      true,\
      false,\
      (void *)&LEUART0->TXDATA,\
      (void *)&LEUART0->RXDATA,\
      0);

  return;
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






#if 0
void LEUART0_IRQHandler(void)
{
	INT_Disable();

#ifdef USE_CIRCULAR_BUFFER
	uint8_t ret_data = 0;
#endif

	/* Clear the TXC flag */
	LEUART0->IFC = LEUART_IFC_TXC;

  /* Transfer all the data from the data buffer to the
   * peripheral device. In every other case, continue to 
   * block in the EM2 case.
   */
	if(counter != DATA_BUFFER_SIZE){
		/* Send the data again */
#ifdef USE_CIRCULAR_BUFFER
		if(remove_from_buffer(&buffer, &ret_data, sizeof(uint8_t)) != BUFFER_EMPTY) {
			counter++;
			LEUART0->TXDATA = ret_data;
		}
#else
		LEUART0->TXDATA = *data_buffer[counter++];
		ret_data[rx_count++] = LEUART0->RXDATA;

		TIMER_IntEnable(TIMER0, TIMER_IF_OF);
		TIMER_Init(TIMER0, &timerInit);

#endif
	} else {
		/* unblock in EM2 sleep mode */
		unblockSleepMode(LEUART_SLEEP_MODE);
	}

  INT_Enable();

  return;
}
#endif


