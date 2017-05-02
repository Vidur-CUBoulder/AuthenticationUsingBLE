/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Thermometer Example Application
 *
 * This Thermometer and OTA example allows the user to measure temperature
 * using the temperature sensor on the WSTK. The values can be read with the
 * Health Thermometer reader on the Blue Gecko smartphone app.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silicon Labs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

#ifndef GENERATION_DONE
#error You must run generate first!
#endif

/* Board Headers */
#include "boards.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "aat.h"
//#include "infrastructure.h"

/* GATT database */
#include "gatt_db.h"

/* EM library (EMlib) */
#include "em_system.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#else
#error This sample app only works with a Silicon Labs Board
#endif

#ifdef FEATURE_IOEXPANDER
#include "bsp.h"
#include "bsp_stk_ioexp.h"
#endif /* FEATURE_IOEXPANDER */


/* Device initialization header */
#include "InitDevice.h"
#include "bsp.h"
#include "bsp_trace.h"

#include "em_gpio.h"

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */

#include "em_leuart.h"
#include "em_gpio.h"
#include "em_ldma.h"
#include "aes_header.h"
#include "em_crypto.h"
#include "sleep.h"
#include "retargetserial.h"
#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"
#include <stdio.h>
#include <stdbool.h>

#define TIMEOUT 3932160  		//2 minutes for a 32.768kHz clock

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

/* Gecko configuration parameters (see gecko_configuration.h) */
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

static const gecko_configuration_t config = {
  .config_flags=0,
  .sleep.flags=SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections=MAX_CONNECTIONS,
  .bluetooth.heap=bluetooth_stack_heap,
  .bluetooth.heap_size=sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb=&bg_gattdb_data,
  .ota.flags=0,
  .ota.device_name_len=3,
  .ota.device_name_ptr="OTA",
  #ifdef FEATURE_PTI_SUPPORT
//  .pti = &ptiInit,
  #endif
};

static uint8_t rx_data_count = 0;
static uint8_t tx_data_count = 0;
uint8_t rx_test_buffer[16];

void Display_init()
{
	DISPLAY_Init();
	RETARGET_TextDisplayInit();
}


/* Buffer to store values that are received from the Leopard gecko*/
uint8_t Storage_Buffer[16];

/* Flag for indicating DFU Reset must be performed */
uint8_t boot_to_dfu = 0;

typedef struct {
	enum {
		set_pin_rcvd_ACK = 0x01,
		check_pin_ACK = 0x02,
		discard_pin_ACK,
		timeout,
		correct_pin_entered,
		incorrect_pin_entered
	} command;
	uint8_t data[6];
	uint8_t data_length;
}ret_mesg_to_LG;


typedef struct{
	enum{
		set_new_pin = 0x01,
		check_pin,
		discard_pin,
		timeout_ACK,
		correct_pin_entered_ACK,
		incorrect_pin_entered_ACK,
	}command;
	uint8_t data[6];
	uint8_t data_length;
}msg_from_LG;


msg_from_LG packet_from_LG;
ret_mesg_to_LG ack_nack_packet;

uint8_t ack_message[16];

  /*Default pin value*/
static uint8_t pinvalue[6] = {1,2,3,4,5,6};
static uint8_t * ptr_pinvalue = &pinvalue;
  /*Default new pin attribute*/
static uint8_t newpin = 0x01;
static uint8_t * ptr_newpin = &newpin;

static uint8_t pin[6][6] = {5,2,1,9,9,1,
							1,1,0,0,4,8,
							4,0,0,0,9,7,
							1,8,1,9,9,4,
							3,1,7,9,7,2,
							9,8,6,1,8,5};
static uint8_t pin_counter = 0;


void Setup_LEUART0(void)
{
	/* Configure GPIO pins */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* To avoid false start, configure output as high */
	GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
	GPIO_DriveStrengthSet(RETARGET_RXPORT,gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 1);

	/* Enable peripheral clocks */
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

	/* Select LFXO for LEUARTs (and wait for it to stabilize) */
	CMU_ClockEnable(cmuClock_LEUART0, true);

	LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

	LEUART_Init(LEUART0, &init);

	/* Enable pins at default location */
	LEUART0->ROUTELOC0 = (LEUART0->ROUTELOC0 & ~(_LEUART_ROUTELOC0_TXLOC_MASK
										   | _LEUART_ROUTELOC0_RXLOC_MASK))
	                    				   | (RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT)
										   | (RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT);

	LEUART0->ROUTEPEN  = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;

	LEUART0->IEN = 0;
	LEUART0->IFC = 0xFF;

	/* Finally enable it */
	LEUART_Enable(LEUART0, leuartEnable);
	LEUART0->CMD = LEUART_CMD_TXEN | LEUART_CMD_RXEN;
}

void Setup_LEUART_Rx(void)
{
	LEUART0->CMD = LEUART_CMD_RXEN;
	LEUART0->IFC = LEUART_IFC_RXOF | LEUART_IFC_RXUF;
	LEUART0->IEN = LEUART_IEN_RXDATAV;

	NVIC_ClearPendingIRQ(LEUART0_IRQn);
	NVIC_EnableIRQ(LEUART0_IRQn);
}

void Setup_LEUART_Tx(void)
{
	LEUART0->CMD = LEUART_CMD_TXEN;
	LEUART0->IFC = 0xFF;
	LEUART0->IEN = LEUART_IEN_TXC;

	NVIC_ClearPendingIRQ(LEUART0_IRQn);
	NVIC_EnableIRQ(LEUART0_IRQn);
}

void Send_Encrypted_to_LG(void *data, uint8_t data_length)
{
	/* Enable the clock to the Crypto engine */
	CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
	CMU_ClockEnable(cmuClock_CRYPTO,true);

	/* Encrypt now! */
	CRYPTO_AES_CBC128(CRYPTO, tx_data_to_lg,\
					  data, data_length,\
					  exampleKey, initVector, true);


	while(CRYPTO->STATUS & CRYPTO_STATUS_SEQRUNNING);
	CRYPTO->IFC = 0xFF;
	NVIC_ClearPendingIRQ(CRYPTO_IRQn);
	/* You'll have to setup LEUART everytime this function is called.
	 * CRYPTO is spoiling the DMA party everytime!
	 */
	Setup_LEUART0();
	Setup_LEUART_Tx();

	tx_data_count=0;
	LEUART0->IFS = LEUART_IFS_TXC;

	return;
}

void create_packet_to_LG(ret_mesg_to_LG packet)
{
	if(packet.data_length > 16) {
		/* Drop/disregard the packet */
		return;
	}
	ack_message[0] = packet.command;
	Send_Encrypted_to_LG(ack_message, 16);
	return;
}

void LEUART0_IRQHandler(void)
{
	uint8_t x = 0;

	__disable_irq();
	int Flags = LEUART0->IF;

	if(Flags & LEUART_IF_RXDATAV)
	{
		Storage_Buffer[rx_data_count] = LEUART0->RXDATA;
		if((rx_data_count == 0) && (Storage_Buffer[rx_data_count] == 0))
		{
			LEUART0->IFC = Flags;
			__enable_irq();
			return;
		}
		rx_data_count++;
		if(rx_data_count == 16) {
			/* Decrypt the data now and send an ack! */
			CRYPTO_AES_CBC128(CRYPTO,\
								rx_test_buffer,\
								Storage_Buffer,\
								16,\
								decryptionKey,\
								initVector,\
								false);

			/* Reset the Counter */
			while(CRYPTO->STATUS & CRYPTO_STATUS_SEQRUNNING);
			CRYPTO->IFC = 0xFF;
			NVIC_ClearPendingIRQ(CRYPTO_IRQn);

			rx_data_count = 0;


			switch(rx_test_buffer[0])
			{
			case set_new_pin:
				/* Do the pin generation here! */
				gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);

				/* Reset software timer on new pin generation*/
				gecko_cmd_hardware_set_soft_timer(0, 0, 0);
				gecko_cmd_hardware_set_soft_timer(TIMEOUT, 0, 0);

				/*Send received ACK*/
				ack_nack_packet.command = set_pin_rcvd_ACK;
				create_packet_to_LG(ack_nack_packet);
				break;

			case check_pin:

				ack_nack_packet.command = check_pin_ACK;
				create_packet_to_LG(ack_nack_packet);
				if(
						(rx_test_buffer[1] == pinvalue[0]) &&
						(rx_test_buffer[2] == pinvalue[1]) &&
						(rx_test_buffer[3] == pinvalue[2]) &&
						(rx_test_buffer[4] == pinvalue[3]) &&
						(rx_test_buffer[5] == pinvalue[4]) &&
						(rx_test_buffer[6] == pinvalue[5])
						)
				{
					ack_nack_packet.command = correct_pin_entered;
					create_packet_to_LG(ack_nack_packet);
					memset(ptr_pinvalue,0,6);
					gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);
				}
				else
				{
					//Incorrect
					ack_nack_packet.command = incorrect_pin_entered;
					create_packet_to_LG(ack_nack_packet);
				}
				break;

			case timeout_ACK:
				__NOP();
				break;

			case discard_pin:
				/* Reset the pin value to zero */
				memset(ptr_pinvalue,0,6);
				gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);
				Display_init();
				printf("\f");
				printf("Pin Discarded!\n");
				break;
			}
			/* Send back and ACK */
		}
	}
	else if(Flags & LEUART_IF_TXC)
	{
		LEUART0->TXDATA = tx_data_to_lg[tx_data_count++];
		if(tx_data_count == 16)
		{
			tx_data_count = 0;
			LEUART0->IEN = 0;
			LEUART0->CMD = LEUART_CMD_TXDIS;
			Setup_LEUART_Rx();
		}
	}

	/* Clear the flag */
	LEUART0->IFC = Flags;
	__enable_irq();
}

void Setup_CRYPTO(void)
{
	/* Enable the clock to the Crypto engine */
	CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
	CMU_ClockEnable(cmuClock_CRYPTO,true);

	/* Get the decryption key and store it */
	CRYPTO_AES_DecryptKey128(CRYPTO, decryptionKey, exampleKey);

	return;
}

/**
 * @brief  Main function
 */
int main(void)
{
#ifdef FEATURE_SPI_FLASH
  /* Put the SPI flash into Deep Power Down mode for those radio boards where it is available */
  MX25_init();
  MX25_DP();
  /* We must disable SPI communication */
  USART_Reset(USART1);

  BSP_TraceProfilerSetup();

#endif /* FEATURE_SPI_FLASH */

  /* Initialize peripherals */
  enter_DefaultMode_from_RESET();

  /* Initialize stack */
  gecko_init(&config);

  /* Configure LEDs*/
  CMU_ClockEnable(cmuClock_GPIO,true);
  GPIO_PinModeSet(gpioPortF,4,gpioModePushPull,0);
  GPIO_PinModeSet(gpioPortF,5,gpioModePushPull,0);

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* Check for stack event. */
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {

    /* This boot event is generated when the system boots up after reset.
     * Here the system is set to start advertising immediately after boot procedure. */
    case gecko_evt_system_boot_id:
    	/* Set advertising parameters. 100ms advertisement interval. All channels used.
    	 * The first two parameters are minimum and maximum advertising interval, both in
    	 * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
    	gecko_cmd_le_gap_set_adv_parameters(800,800,7);
    	/* Start general advertising and enable connections. */
    	gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);


    	/*Setting default pin on system boot*/
    	gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);
    	/* Start software timer on boot to timeout on 2 minutes*/
    	gecko_cmd_hardware_set_soft_timer(TIMEOUT,0,0);

    	Setup_LEUART0();
    	Setup_LEUART_Rx();
    	Setup_CRYPTO();

    	break;

      /* This event is generated when the software timer has ticked. In this example the temperature
       * is read after every 1 second and then the indication of that is sent to the listening client. */
      case gecko_evt_hardware_soft_timer_id:
    	  GPIO_PinOutToggle(gpioPortF,5);

    	  ack_nack_packet.command = timeout;
    	  create_packet_to_LG(ack_nack_packet);
    	  memset(ptr_pinvalue,0,6);																//Discard the existing pin
    	  gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);		//Update value on the server
    	  gecko_cmd_hardware_set_soft_timer(0, 0, 0);											//Turn off                                                                                                                                                                                                        the soft timer

        break;

      case  gecko_evt_gatt_server_attribute_value_id:
    	  GPIO_PinOutToggle(gpioPortF,4);

    	  memcpy(ptr_pinvalue,&pin[pin_counter++][0],6);
    	  if(pin_counter == 6)
    		  pin_counter=0;
		  /* Reset software timer on new pin generation*/
    	  gecko_cmd_hardware_set_soft_timer(0, 0, 0);
    	  gecko_cmd_hardware_set_soft_timer(TIMEOUT, 0, 0);
    	  /*Send new pin value and request the new pin attribute*/
    	  gecko_cmd_gatt_server_write_attribute_value(gattdb_pin_value,0,6,ptr_pinvalue);
    	  gecko_cmd_gatt_server_write_attribute_value(gattdb_req_new_pin,0,1,ptr_newpin);
    	  break;

      case gecko_evt_le_connection_closed_id:
          /* Stop timer in case client disconnected before indications were turned off */
          gecko_cmd_hardware_set_soft_timer(0, 0, 0);
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        break;


      default:
        break;
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
