/*
 * leuart.h
 *
 *  Created on: Mar 11, 2017
 *      Author: vidursarin
 */

#ifndef SRC_LEUART_H_
#define SRC_LEUART_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_leuart.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "em_int.h"
#include "aes_cbc_128_dma.h"

#include "segmentlcd.h"
#include "em_lcd.h"

/** LEUART Rx/Tx Port/Pin Location */
#define LEUART_LOCATION    0
#define LEUART_TXPORT      gpioPortD            /* LEUART transmission port */
#define LEUART_TXPIN       4                    /* LEUART transmission pin */
#define LEUART_RXPORT      gpioPortD            /* LEUART reception port */
#define LEUART_RXPIN       5                    /* LEUART reception pin */

#define SIZEOF_DMA_BLOCK (sizeof(dataBuffer) / (sizeof(dataBuffer[0]) * 16))

/* enum and structure declarations */
typedef struct {
	enum {
		set_new_pin = 0x01,
		check_pin = 0x02,
		discard_pin,
		timeout_ACK,
		correct_pin_entered_ACK,
		incorrect_pin_entered_ACK
	}command;
	uint8_t *data;
	uint8_t data_length;
}msg_to_BG;

extern const uint8_t exampleKey[16];

typedef struct{
	enum{
		set_pin_rcvd_ACK=0x01,
		check_pin_ACK,
		discard_pin_ACK,
		timeout,
		correct_pin_entered,
		incorrect_pin_entered
	}command;
	uint8_t data[6];
	uint8_t data_length;
}msg_from_BG;

msg_from_BG packet_from_BG;
msg_to_BG packet_to_BG;


static uint8_t final_message[] = { 0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46,
        					0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D};


/* 64B of exampleData */
extern uint8_t exampleData[16];
uint8_t aes_buffer[16];

extern uint8_t decryptionKey[16];
extern const uint8_t initVector[16];

void Setup_LEUART(void);

void Setup_LEUART_DMA(void);

void cb_Chnl0_DMA(unsigned int channel, bool primary, void *user);

void Setup_Rx_from_BG(void);

void create_packet(msg_to_BG packet);

#endif /* SRC_LEUART_H_ */
