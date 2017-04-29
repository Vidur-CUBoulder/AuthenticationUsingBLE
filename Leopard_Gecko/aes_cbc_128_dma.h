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
#ifndef __AES_H
#define __AES_H
#include <stdbool.h>
#include <stdint.h>
#include "em_aes.h"

#define ENABLE_AES_INTERRUPT  {\
                                AES->IFC = AES_IFC_DONE;\
                                AES->IEN = AES_IEN_DONE;\
                                NVIC_EnableIRQ(AES_IRQn);\
                              }

#define delay(X) for(int i=0; i<X; i++)

uint8_t exampleData[16];
uint8_t dataBuffer[sizeof(exampleData) / sizeof(exampleData[0])];

#define SIZEOF_DMA_BLOCK (sizeof(dataBuffer) / (sizeof(dataBuffer[0]) * 16))

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

void Encrypt_Data(uint8_t *input_data,\
				  uint8_t *output_data);

bool AesFinished(void);

void Config_AES(void);

extern uint8_t decryptionKey[16];

extern const uint8_t exampleKey[16];

extern uint8_t aes_buffer[16];

extern const uint8_t initVector[16];

#endif
