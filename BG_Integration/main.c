/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

#ifndef GENERATION_DONE
#error You must run generate first!
#endif

/* Board headers */
#include "boards.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "aat.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#endif

/* Device initialization header */
#include "InitDevice.h"

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */


#include "em_gpio.h"
#include "em_ldma.h"
#include "em_leuart.h"
#include "retargetserial.h"

//#include "sl_crypto.h"
//#include "cryptodrv.h"

//#include "mbedtls/aes.h"
//#include "mbedtls/entropy.h"
#include "em_crypto.h"

#include "trial_header.h"


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef FEATURE_PTI_SUPPORT
static const RADIO_PTIInit_t ptiInit = RADIO_PTI_INIT;
#endif

/* Gecko configuration parameters (see gecko_configuration.h) */
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
  .pti = &ptiInit,
  #endif
};

/* Flag for indicating DFU Reset must be performed */
uint8_t boot_to_dfu = 0;

#if 0

#define AES_DATA_SIZE 64
#define AES_BLOCK_SZ 16

uint8_t Storage_Buffer[AES_DATA_SIZE];

const uint8_t exampleKey[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,\
                               0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

uint8_t example_input_data[AES_BLOCK_SZ] = {0x03,0x36,0x76,0x3e,0x96,0x6d,0x92,0x59,0x5a,0x56,0x7c,0xc9,0xce,0x53,0x7f,0x5e};

uint8_t decryptionKey[16];

uint8_t pop_data[64];

const uint8_t initVector[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
#endif

/** LDMA Descriptor initialization */
static LDMA_Descriptor_t xfer =
		LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(&LEUART0->RXDATA, /* Peripheral source address */
				&Storage_Buffer,  /* Peripheral destination address */
				AES_DATA_SIZE,    /* Number of bytes */
				0);               /* Link to same descriptor */

#if 0
void Setup_CRYPTO(void) {

	/* Enable the clock to the CRYPTO engine */
	CMU->HFBUSCLKEN0 = CMU_HFBUSCLKEN0_CRYPTO;

	/* Get the decryption key from the original key, needs to be done once for each key */
	CRYPTO_AES_DecryptKey128(CRYPTO, decryptionKey, exampleKey);

	return;
}
#endif

#if 1
void Setup_LEUART0(void) {

	// $[LEUART0 initialization]
	 /* Enable peripheral clocks */
	  CMU_ClockEnable(cmuClock_HFPER, true);

	  /* Configure GPIO pins */
	  CMU_ClockEnable(cmuClock_GPIO, true);

	  /* To avoid false start, configure output as high */
	  GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
	  GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 0);

	  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

	  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
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

    // [LEUART0 initialization]$

}
#endif

#if 1
void Setup_LDMA(void) {

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
#endif
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

#endif /* FEATURE_SPI_FLASH */

  /* Initialize peripherals */
  enter_DefaultMode_from_RESET();

  //Setup_CRYPTO();
  Setup_LEUART0();
  Setup_LDMA();

  /* Initialize stack */
  gecko_init(&config);

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
        gecko_cmd_le_gap_set_adv_parameters(160,160,7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        break;

      case gecko_evt_le_connection_closed_id:
	  
        /* Check if need to boot to dfu mode */
        if (boot_to_dfu) {
          /* Enter to DFU OTA mode */
          gecko_cmd_system_reset(2);
        }
        else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        }
        break;


      /* Events related to OTA upgrading
      ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:
      
        if(evt->data.evt_gatt_server_user_write_request.characteristic==gattdb_ota_control)
        {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1; 
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection, 
            gattdb_ota_control, 
            bg_err_success);
         
          /* Close connection to enter to DFU OTA mode */        
          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      default:
        break;
    }
  }
  return 0;
}


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
