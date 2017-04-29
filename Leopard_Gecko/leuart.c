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

#define DMA_CHNL0_LEUART0 5

#define delay(X) for(int i=0; i<X; i++)

//static uint8_t ret_data;
static uint8_t rx_data_count;
static uint8_t rx_test_buffer[16];
static uint8_t Storage_Buffer_RX[16];
//static uint8_t count = 0;

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
    .hprot        = 0
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
	/* Clear the Flag */
	__disable_irq();
	DMA->CONFIG &= ~DMA_CONFIG_EN;                          //Disable DMA
	DMA->IFC = 1 << DMA_CHNL0_LEUART0;                     //Clear all interrupt flags
	__enable_irq();

}

void Setup_Rx_from_BG(void)
{
	/* Clear any pending LEUART0 interrupts */
	uint8_t pending_LEUART = LEUART_IntGet(LEUART0);
	LEUART_IntClear(LEUART0, pending_LEUART);

	LEUART0->CMD = LEUART_CMD_RXEN;

	/* Set the interrupt for RXDATAV */
	LEUART0->IEN = LEUART_IEN_RXDATAV;
	NVIC_EnableIRQ(LEUART0_IRQn);

}

uint8_t test_check_pin_data[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
uint8_t pin_trials;

void LEUART0_IRQHandler(void)
{
	__disable_irq();

	/* Clear the RXdata interrupt flag */
	LEUART0->IFC = 0xFF;

	Storage_Buffer_RX[rx_data_count] = LEUART0->RXDATA;
	rx_data_count++;

	if(rx_data_count == 16) {
		/* Disable the AES interrupt flag */
		AES->IEN = AES_IEN_DONE_DEFAULT;
		NVIC_DisableIRQ(AES_IRQn);

		/* Decrypt the data now! */
		AesCBC128DmaDecrypt(decryptionKey, Storage_Buffer_RX, rx_test_buffer, SIZEOF_DMA_BLOCK, initVector);
		while((AES->STATUS & AES_STATUS_RUNNING));
		delay(200);
		DMA_Reset();

		/* Reset the counter */
		rx_data_count = 0;

		switch(rx_test_buffer[0])
		{
		case set_pin_rcvd_ACK:
			/* Print ACK on the LCD */
			//SegmentLCD_Write("ACK!");
			//Do nothing, wait for pin input
			packet_to_BG.command = check_pin;
			packet_to_BG.data_length = 6;
			packet_to_BG.data = &test_check_pin_data[0];
			create_packet(packet_to_BG);
			break;

		case check_pin_ACK:
			//Do nothing
			SegmentLCD_Write("pin ACK'd");
			break;

		case discard_pin_ACK:
			//Ask to generate new pin,  or system lockdown??
			SegmentLCD_Write("NAK!");
			break;

		case timeout:
			//send timeout ACK
			packet_to_BG.command = timeout;
			packet_to_BG.data_length = 0;
			create_packet(packet_to_BG);
			//Display timeout for 2 seconds
			//EMU_EnterEM4();
			break;

		case correct_pin_entered:
			SegmentLCD_Write("CORRECT");
			break;

		case incorrect_pin_entered:
			if(++pin_trials == 3)
			{
			//send incorrect pin ACK
			//Display Wrong for 2 seconds
			//EMU_EnterEM4();
			}
			break;

		default:
			break;
		}

		/* Go to the next state from here now! */
#if 0
			packet_to_BG.command = check_pin;
			packet_to_BG.data_length = 6;
			packet_to_BG.data = &test_check_pin_data[0];
			pin_trials++;
			create_packet(packet_to_BG);
#endif

	}

	__enable_irq();
}

/* This function will only create the TX packets! */
void create_packet(msg_to_BG packet)
{
	if (packet.data_length > 16) {
		/* Drop/disregard the packet */
		return;
	}

	switch(packet.command)
	{
		case set_new_pin: /* Ask the BG to generate a new pin */
						 final_message[0] = packet.command;
						 if(packet.data_length != 0) {
							 for(int i = 1; i<=packet.data_length; i++) {
								 final_message[i] = packet.data[i-1];
							 }
						 }
						 /* Start the encryption procedure now! */
						 Encrypt_Data(final_message, aes_buffer);
						 break;

		case check_pin: /* Check if the pin entered by the user is correct or not! */

						LEUART_Reset(LEUART0);
						Setup_LEUART();
						Setup_LEUART_DMA();
						ENABLE_AES_INTERRUPT;

						final_message[0] = packet.command;
						if(packet.data_length != 0) {
							for(int i = 1; i<=packet.data_length; i++) {
								final_message[i] = packet.data[i-1];
							}
						}
						/* Start the encryption procedure now! */
						Encrypt_Data(final_message, aes_buffer);
						break;

		case timeout: /* Send the timeout ack! */
						final_message[0] = packet.command;
						if(packet.data_length != 0) {
							for(int i = 1; i<=packet.data_length; i++) {
								final_message[i] = packet.data[i-1];
							}
						}
						Encrypt_Data(final_message, aes_buffer);
						break;

		default:
						break;
	}
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
#if 0
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
#endif
