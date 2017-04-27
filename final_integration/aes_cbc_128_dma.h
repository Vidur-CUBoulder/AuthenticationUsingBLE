/***************************************************************************//**
 * @file
 * @brief AES CBC 128-bit DMA driven functions for EFM32
 * @author Energy Micro AS
 * @version 1.10
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#ifndef AES_CBC_128_DMA_H
#define AES_CBC_128_DMA_H
#include <stdbool.h>
#include <stdint.h>
#include "string.h"
#include "em_device.h"
#include "em_dma.h"
#include "em_cmu.h"
#include "em_aes.h"
#include "leuart.h"
#include "dmactrl.h"


/*****************************************************
            * Global Variables *
 *****************************************************/
uint8_t decryptionKey[16];
static const uint8_t exampleKey[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                               0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};



#define delay(X) for(int i=0; i<X; i++)

static uint8_t exampleData[] = { 0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
                          0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
static uint8_t dataBuffer[sizeof(exampleData) / sizeof(exampleData[0])];
#define SIZEOF_DMA_BLOCK (sizeof(dataBuffer) / (sizeof(dataBuffer[0]) * 16))


static uint8_t message_packet[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                            0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};


static const uint8_t initVector[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

/* Store the AES data in a circular buffer */
static uint8_t aes_buffer[/*AES_DATA_SIZE*/16];

static uint8_t ret_data;

typedef struct
{
    enum {
        do_action     = 0x01,
    } cmd_list_enum;
    /*Action list enum */

    enum  {
        gen_new_pin   = 0x01,
        send_pin      = 0x02,
        discard_pin   = 0x03,
        ack_timeout   = 0x04,
        incorrect_pin = 0x05,
        disp_enc_data = 0x06,
        invalid       = 0xff
    } action_cmd_list_enum;

} msg_packet;


msg_packet trial_packet;


void AesCBC128DmaEncrypt(const uint8_t* key,
                         uint8_t*       inputData,
                         uint8_t*       outputData,
                         const uint32_t blockNumber,
                         const uint8_t* iv);
void AesCBC128DmaDecrypt(const uint8_t* key,
                         uint8_t*       inputData,
                         uint8_t*       outputData,
                         const uint32_t blockNumber,
                         const uint8_t* iv);
bool AesFinished(void);


void Config_AES(void);

void Decrypt_Data(uint8_t *data);

void create_msg_packet(msg_packet packet, char *pin_data);

void enable_rx_pin(void);

#endif
