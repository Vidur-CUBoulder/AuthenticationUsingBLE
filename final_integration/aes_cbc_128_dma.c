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

#include "aes_cbc_128_dma.h"


/* The channel numbers can be changed, but the order must be the same */
#define DMA_CH_WRITEPREVDATA   0
#define DMA_CH_READDATA        1
#define DMA_CH_WRITEDATA       2

#define DMA_MAX_TRANSFERS   1024

/* DMA callback structures */
DMA_CB_TypeDef encryptCb;
DMA_CB_TypeDef decryptCb;

static uint16_t          numberOfBlocks;
static uint16_t          blockIndex;
static uint32_t*         outputDataG;
static uint32_t*         inputDataG;
static const uint32_t*   keyG;
static const uint32_t*   ivG;
static volatile bool     aesFinished;
static uint32_t          wordTransfers;

static volatile uint8_t incorrect_pin_count;

uint8_t data_count;
uint8_t rx_data_from_BG[17];
uint8_t letimer_flag = 0;

/**************************************************************************//**
 * @brief  Call-back called when DMA encryption read data transfer is done
 * All DMA channels are refreshed using this callback, until no more data is
 * left to encrypt. Up to DMA_MAX_TRANSFERS transfer are performed between
 * calls to this function.
 *****************************************************************************/
static void dmaEncryptDataReadDone(unsigned int channel, bool primary, void *user)
{
  (void) channel;
  (void) primary;
  (void) user;

  /* Update block index */
  blockIndex=blockIndex+wordTransfers/4;

  if (blockIndex < numberOfBlocks)
  {
    /* Calculate how many word transfers to set up the DMA for */
    wordTransfers = (numberOfBlocks - blockIndex)*4;
    if (wordTransfers > DMA_MAX_TRANSFERS)
      wordTransfers = DMA_MAX_TRANSFERS;

    /* Re-activate the data read DMA channel */
    DMA_ActivateBasic(DMA_CH_READDATA,
                      true,
                      false,
                      (void *)&outputDataG[blockIndex*4], /* Destination address */
                      (void *)&(AES->DATA),               /* Source address */
                      wordTransfers - 1);                 /* Number of transfers - 1 */

  /* Re-activate the data write DMA channel */
    DMA_ActivateBasic(DMA_CH_WRITEDATA,
                      true,
                      false,
                      (void *)&(AES->XORDATA),           /* Destination address */
                      (void *)&inputDataG[blockIndex*4], /* Source address */
                      wordTransfers - 1);                /* Number of transfers - 1 */
  }

  else
  {
    /* Indicate last block */
    aesFinished = true;
  }

}

/**************************************************************************//**
 * @brief  Call-back called when DMA decryption read data transfer is done
 * All DMA channels are refreshed using this callback, until no more data is
 * left to decrypt. Up to DMA_MAX_TRANSFERS transfer are performed between
 * calls to this function.
 *****************************************************************************/
static void dmaDecryptDataReadDone(unsigned int channel, bool primary, void *user)
{
  (void) channel;
  (void) primary;
  (void) user;

  /* Update block index */
  blockIndex=blockIndex+wordTransfers/4;

  if (blockIndex < numberOfBlocks)
  {
    /* Calculate how many word transfers to set up the DMA for */
    wordTransfers = (numberOfBlocks - blockIndex)*4;
    if (wordTransfers > DMA_MAX_TRANSFERS)
      wordTransfers = DMA_MAX_TRANSFERS;

    /* Re-activate the previous data write DMA channel */
    DMA_ActivateBasic(DMA_CH_WRITEPREVDATA,
                      true,
                      false,
                      (void *)&(AES->XORDATA),             /* Destination address */
                      (void *)&inputDataG[blockIndex*4-4], /* Source address */
                      wordTransfers - 1);                  /* Number of transfers - 1 */

    /* Re-activate the data read DMA channel */
    DMA_ActivateBasic(DMA_CH_READDATA,
                      true,
                      false,
                      (void *)&outputDataG[blockIndex*4], /* Destination address */
                      (void *)&(AES->DATA),               /* Source address */
                      wordTransfers - 1);                 /* Number of transfers - 1 */

    /* Re-activate the data write DMA channel */
    DMA_ActivateBasic(DMA_CH_WRITEDATA,
                      true,
                      false,
                      (void *)&(AES->DATA),              /* Destination address */
                      (void *)&inputDataG[blockIndex*4], /* Source address */
                      wordTransfers - 1);                /* Number of transfers - 1 */
  }

  else
  {
    /* Indicate last block */
    aesFinished = true;
  }

}



/**************************************************************************//**
 * @brief  Configure DMA for encryption
 * Two DMA channels are used:
 * - Read output data from AES->DATA
 * - Write new data to AES->XORDATA
 * Both DMA requests are set when an AES encryption finishes.
 *****************************************************************************/
static void setupEncryptDma(void)
{
  DMA_Init_TypeDef        dmaInit;
  DMA_CfgChannel_TypeDef  chnlCfg;
  DMA_CfgDescr_TypeDef    descrCfg;

  /* Clearing AES DMA requests by writing AES->CTRL */
  AES->CTRL = AES->CTRL;

  /* Enable DMA clock */
  CMU_ClockEnable(cmuClock_DMA, true);

  /* Initializing the DMA */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);

  /* Setting up call-back function for read data */
  encryptCb.cbFunc  = dmaEncryptDataReadDone;
  encryptCb.userPtr = NULL;

  /* Setting up data read channel */
  chnlCfg.highPri   = false;
  chnlCfg.enableInt = true;
  chnlCfg.select    = DMAREQ_AES_DATARD;  /* DMA request cleared by AES->DATA reads */
  chnlCfg.cb        = &encryptCb;
  DMA_CfgChannel(DMA_CH_READDATA, &chnlCfg);

  /* Setting up data read channel descriptor */
  descrCfg.dstInc  = dmaDataInc4;    /* Word destination address increase */
  descrCfg.srcInc  = dmaDataIncNone; /* No source address increase */
  descrCfg.size    = dmaDataSize4;   /* Word transfers */
  descrCfg.arbRate = dmaArbitrate4;  /* Arbitrate after 4 transfers */
  descrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CH_READDATA, true, &descrCfg);

  /* Activate DMA channel */
  DMA_ActivateBasic(DMA_CH_READDATA,
                    true,
                    false,
                    (void *)&outputDataG[0], /* Destination address */
                    (void *)&(AES->DATA),    /* Source address */
                    wordTransfers - 1);      /* Number of transfers - 1 */

  /* Setting up data write channel */
  /* No callback used as channel is handled in the read channel callback */
  chnlCfg.highPri   = false;
  chnlCfg.enableInt = true;
  chnlCfg.select    = DMAREQ_AES_XORDATAWR; /* DMA request cleared by AES->XORDATA writes */
  chnlCfg.cb        = NULL;
  DMA_CfgChannel(DMA_CH_WRITEDATA, &chnlCfg);

  /* Setting up data write channel descriptor */
  descrCfg.dstInc  = dmaDataIncNone;  /* No destination address increase */
  descrCfg.srcInc  = dmaDataInc4;     /* Word source address increase */
  descrCfg.size    = dmaDataSize4;    /* Word transfers */
  descrCfg.arbRate = dmaArbitrate4;   /* Arbitrate after 4 transfers */
  descrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CH_WRITEDATA, true, &descrCfg);

  /* Activate DMA channel */
  DMA_ActivateBasic(DMA_CH_WRITEDATA,
                    true,
                    false,
                    (void *)&(AES->XORDATA), /* Destination address */
                    (void *)&inputDataG[0],  /* Source address */
                    wordTransfers - 1);      /* Number of transfers - 1 */
}

/**************************************************************************//**
 * @brief  Configure DMA for decryption
 * Three DMA channels are used:
 * - Read output data from AES->DATA
 * - Write new data to AES->DATA
 * - Write previous data to AES->XORDATA
 * All DMA requests are set when an AES decryption finishes.
 *****************************************************************************/
static void setupDecryptDma(void)
{
  DMA_Init_TypeDef        dmaInit;
  DMA_CfgChannel_TypeDef  chnlCfg;
  DMA_CfgDescr_TypeDef    descrCfg;

  /* Clearing AES DMA requests by writing AES->CTRL */
  AES->CTRL = AES->CTRL;

  /* Enable DMA clock */
  CMU_ClockEnable(cmuClock_DMA, true);

  /* Initializing the DMA */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);

  /* Setting up call-back function for read data */
  decryptCb.cbFunc  = dmaDecryptDataReadDone;
  decryptCb.userPtr = NULL;

  /* Setting up data read channel */
  chnlCfg.highPri   = false;
  chnlCfg.enableInt = true;
  chnlCfg.select    = DMAREQ_AES_DATARD; /* DMA request cleared by AES->DATA reads */
  chnlCfg.cb        = &decryptCb;
  DMA_CfgChannel(DMA_CH_READDATA, &chnlCfg);

  /* Setting up data read channel descriptor */
  descrCfg.dstInc  = dmaDataInc4;    /* Word destination address increase */
  descrCfg.srcInc  = dmaDataIncNone; /* No source address increase */
  descrCfg.size    = dmaDataSize4;   /* Word transfers */
  descrCfg.arbRate = dmaArbitrate4;  /* Arbitrate after 4 transfers */
  descrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CH_READDATA, true, &descrCfg);

  /* Activate DMA channel */
  DMA_ActivateBasic(DMA_CH_READDATA,
                    true,
                    false,
                    (void *)&outputDataG[0], /* Destination address */
                    (void *)&(AES->DATA),    /* Source address */
                    3);                      /* Only process first block */

  /* Setting up data write channel */
  /* No callback used as channel is handled in the read channel callback */
  chnlCfg.highPri   = false;
  chnlCfg.enableInt = true;
  chnlCfg.select    = DMAREQ_AES_DATAWR; /* DMA request cleared by AES->DATA writes */
  chnlCfg.cb        = NULL;
  DMA_CfgChannel(DMA_CH_WRITEDATA, &chnlCfg);

  /* Setting up data write channel descriptor */
  descrCfg.dstInc  = dmaDataIncNone;   /* No destination address increase */
  descrCfg.srcInc  = dmaDataInc4;      /* Word source address increase */
  descrCfg.size    = dmaDataSize4;     /* Word transfers */
  descrCfg.arbRate = dmaArbitrate4;    /* Arbitrate after 4 transfers */
  descrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CH_WRITEDATA, true, &descrCfg);

  /* Activate DMA channel */
  DMA_ActivateBasic(DMA_CH_WRITEDATA,
                    true,
                    false,
                    (void *)&(AES->DATA),    /* Destination address */
                    (void *)&inputDataG[0],  /* Source address */
                    3);                      /* Only process first block */

  /* Setting up previous data write channel */
  /* No callback used as channel is handled in the read channel callback */
  chnlCfg.highPri   = false;
  chnlCfg.enableInt = true;
  chnlCfg.select    = DMAREQ_AES_XORDATAWR;  /* DMA request cleared by AES->XORDATA writes */
  chnlCfg.cb        = NULL;
  DMA_CfgChannel(DMA_CH_WRITEPREVDATA, &chnlCfg);

  /* Setting up previous data write channel descriptor */
  descrCfg.dstInc  = dmaDataIncNone;  /* No destination address increase */
  descrCfg.srcInc  = dmaDataInc4;     /* Word source address increase */
  descrCfg.size    = dmaDataSize4;    /* Word transfers */
  descrCfg.arbRate = dmaArbitrate4;   /* Arbitrate after 4 transfers */
  descrCfg.hprot   = 0;
  DMA_CfgDescr(DMA_CH_WRITEPREVDATA, true, &descrCfg);

  /* Activate DMA channel */
  DMA_ActivateBasic(DMA_CH_WRITEPREVDATA,
                    true,
                    false,
                    (void *)&(AES->XORDATA),  /* Destination address */
                    (void *)&ivG[0],          /* Source address */
                    3);                       /* Only process first block */

}


/**************************************************************************//**
 * @brief  Decrypt with 128-bit key in CBC mode. Function returns after
 * initializing decryption. Use AesFinished() to check for
 * completion. Output data only valid after completion.
 *
 * @param[in] key
 *   This is the 128-bit decryption key. The decryption key may
 *   be generated from the encryption key with AES_DecryptKey128().
 *
 * @param[in] inputData
 *   Buffer holding data to decrypt.
 *
 * @param[out] outputData
 *   Buffer to put output data, must be of size blockNumber*16 bytes. This
 *   buffer can NOT be the same as inputData.
 *
 * @param[in] blockNumber
 *   Number of 128-bit blocks to decrypt.
 *
 * @param[in] iv
 *   128-bit initialization vector to use
 *****************************************************************************/
void AesCBC128DmaDecrypt(const uint8_t* key,
                         uint8_t*       inputData,
                         uint8_t*       outputData,
                         const uint32_t blockNumber,
                         const uint8_t* iv)
{
  int i;

  /* Copy to global variables */
  inputDataG     = (uint32_t*) inputData;
  outputDataG    = (uint32_t*) outputData;
  keyG           = (uint32_t*) key;
  ivG            = (uint32_t*) iv;
  numberOfBlocks = blockNumber;

  /* Reset block index */
  blockIndex = 0;

  /* Initialize finished flag */
  aesFinished = false;

  /* Set number of words to transfer in first DMA round */
  wordTransfers = 4;

  /* Setup DMA for decryption*/
  setupDecryptDma();

  /* Configure AES module */
  AES->CTRL = AES_CTRL_DECRYPT |     /* Set decryption */
              AES_CTRL_BYTEORDER |   /* Set byte order */
              AES_CTRL_KEYBUFEN |    /* Use key buffering */
              AES_CTRL_DATASTART;    /* Start decryption on data write */

  /* Write the KEY to AES module */
  /* Writing only the KEYHA 4 times will transfer the key to all 4
  * high key registers, used here, in 128 bit key mode, as buffers */
  for (i = 0; i <= 3; i++)
  {
    AES->KEYHA = keyG[i];
  }

  /* Start DMA by software trigger to write first data */
  DMA->CHSWREQ = 1 << DMA_CH_WRITEDATA;
}

/**************************************************************************//**
 * @brief  Encrypt with 128-bit key in CBC mode. Function returns after
 * initializing encryption. Use AesFinished() to check for
 * completion. Output data only valid after completion.
 *
 * @param[in] key
 *   This is the 128-bit encryption key.
 *
 * @param[in] inputData
 *   Buffer holding data to encrypt.
 *
 * @param[out] outputData
 *   Buffer to put output data, must be of size blockNumber*16 bytes. This can
 *   be the same location as inputData
 *
 * @param[in] blockNumber
 *   Number of 128-bit blocks to encrypt.
 *
 * @param[in] iv
 *   128-bit initialization vector to use
 *****************************************************************************/
void AesCBC128DmaEncrypt(const uint8_t* key,
                         uint8_t*       inputData,
                         uint8_t*       outputData,
                         const uint32_t blockNumber,
                         const uint8_t* iv)
{
  int i;

  /* Copy to global variables */
  inputDataG     = (uint32_t*) inputData;
  outputDataG    = (uint32_t*) outputData;
  keyG           = (uint32_t*) key;
  ivG            = (uint32_t*) iv;
  numberOfBlocks = blockNumber;

  /* Reset block index */
  blockIndex = 0;

  /* Initialize finished flag */
  aesFinished = false;

  /* Set number of words to transfer in first DMA round */
  wordTransfers = numberOfBlocks*4;
  if (wordTransfers > DMA_MAX_TRANSFERS)
    wordTransfers = DMA_MAX_TRANSFERS;

  /* Setup DMA for encryption*/
  setupEncryptDma();

  AES->CTRL = AES_CTRL_KEYBUFEN |     /* Use key buffering */
              AES_CTRL_BYTEORDER |    /* Set byte order */
              AES_CTRL_XORSTART;      /* Start encryption on data write */

  /* Write the KEY to AES module */
  /* Writing only the KEYHA 4 times will transfer the key to all 4
  * high key registers, used here, in 128 bit key mode, as buffers */
  for (i = 0; i <= 3; i++)
  {
    AES->KEYHA = keyG[i];
  }

  /* Writing to the DATA register here does NOT trigger encryption */
  for (i = 0; i <= 3; i++)
  {
    AES->DATA = ivG[i];
  }

  /* Start DMA by software trigger to write first data */
  DMA->CHSWREQ = 1 << DMA_CH_WRITEDATA;
}


void Config_AES(void)
{
    CMU_ClockEnable(cmuClock_HFPER,true);
    CMU_ClockEnable(cmuClock_AES,true);

    /* Enable the AES to hit the interrupt handler on completion
     * of encryption/decryption operations.
     */
    AES->IFC = AES_IFC_DONE;
    AES->IEN = AES_IEN_DONE;
    NVIC_EnableIRQ(AES_IRQn);

    DMA_Reset();

}

void create_msg_packet(msg_packet packet, char *pin_data)
{

  switch(packet.cmd_list_enum)
  {
    case do_action:
      switch(packet.action_cmd_list_enum)
      {
        case gen_new_pin:
        	/* Generate a new pin! and discard the old one! */
          break;

        case send_pin:
          /* Add the pin data to the buffer */
          message_packet[0] = packet.cmd_list_enum;
          message_packet[1] = packet.action_cmd_list_enum;
          for (int i = 0; i<6; i++) {
            message_packet[i+2] = *(pin_data + i);
          }

          break;

        case discard_pin:
        	/* Clear the pin buffer and call the func.
        	 * to generate a new pin
        	 */
          break;

        case ack_timeout:
          break;

        case incorrect_pin:
        	incorrect_pin_count++;
        	if(incorrect_pin_count >= 3) {
        		/* Discard the old ping and create a new one */
        	}
          break;

        case disp_enc_data:
          break;

        case invalid:
          /* Do something here! */
          break;

        default:
        	//enable_rx_pin();
        	/*Default state is to continue to wait for an input
        	 * from the Blue Gecko*/
//        	leuart_dma_setup_rx();
//        	/* Activate the RX pin to receive data */
//        	LEUART0->CMD = LEUART_CMD_RXEN;
//        	DMA_ActivateBasic(1,
//        			true,
//					false,
//					(void *)&aes_buffer,
//					(void *)&LEUART0->RXDATA,
//					/*(AES_DATA_SIZE-1)*/5);
        	//leuart_dma_setup_rx();
//        	LEUART_Enable_IRQ();
//        	LEUART0->CMD = LEUART_CMD_RXEN;
//        	while(i != 6) {
//        		while(!(LEUART0->STATUS & LEUART_STATUS_RXDATAV));
//        		rx_data_from_BG[i++] = LEUART0->RXDATA;
//        	}
          break;
      }
      break;
#ifdef EXPAND_FUNCTIONALITY
    case req_data:
      switch(packet.u.req_data_list_enum)
      {
        case kill_connection:
          /* Run the function! */
          break;

        default:
          break;
      }
      break;
#endif
    default:
      break;
  }

  CMU_ClockEnable(cmuClock_HFPER,true);
  CMU_ClockEnable(cmuClock_AES,true);

  /* Make the call to the DMA Encrypt! */
  /* Clear the interrupt flag */
  AES->IFC = AES_IFC_DONE;
  AES->IEN = AES_IEN_DONE;

  /* Trigger the AES interrupt */
  AesCBC128DmaEncrypt(exampleKey,\
      /*message_packet*/exampleData,\
      aes_buffer,\
      SIZEOF_DMA_BLOCK,\
      initVector);

  /* This is a very strategically placed delay.
   * Critical for the AES to give a complete output!
   * Don't dare touch it!!
   */
  delay(100);

  DMA_Reset();

    return;
}

void AES_IRQHandler(void)
{
    __disable_irq();

    /* Clear the AES interrupt and disable the
     * AES engine completely
     */
    AES->IFC = AES_IFC_DONE;
    AES->CMD = AES_CMD_STOP;
    CMU_ClockEnable(cmuClock_AES,false);
    CMU_ClockEnable(cmuClock_HFPER,false);
#if 1
    /* Reset the DMA controller completely */
    DMA_Reset();

    /* Do the DMA LEUART configuration */
    leuart_dma_setup_tx();

    /* Clear the interrupt flag */
    AES->IFC = AES_IFC_DONE;

  DMA_ActivateBasic(LEUART_DMA_CHANNEL,
      true,
      false,
      (void *)&LEUART0->TXDATA,
      (void *)&aes_buffer,
      /*(AES_DATA_SIZE-1)*/15);
#endif
  DMA_Reset();
#if 0
  tx_flag = 1;
  LEUART0->IFC = LEUART_IFC_TXC | LEUART_IFC_TXOF;
  LEUART0->IEN = LEUART_IEN_TXC;
  NVIC_ClearPendingIRQ(LEUART0_IRQn);
  NVIC_EnableIRQ(LEUART0_IRQn);
#endif

 __enable_irq();
}

void enable_rx_pin()
{

  //leuart_dma_setup_rx();
  LEUART0->CMD = LEUART_CMD_RXEN;
  /* Poll and wait for the data to come from the BG */
#ifdef DEBUG_POLL_RX
  while(i != 6) {
    while(!(LEUART0->STATUS & LEUART_STATUS_RXDATAV));
    rx_data_from_BG[i++] = LEUART0->RXDATA;
  }
#endif
  LEUART0->IFC = LEUART_IFC_RXOF | LEUART_IFC_RXUF;
  LEUART0->IEN = LEUART_IEN_RXDATAV;
  NVIC_EnableIRQ(LEUART0_IRQn);
  return;
}

void LEUART0_IRQHandler(void)
{
	__disable_irq();

	/* Clear the LEUART interrupt flags */
	LEUART0->IFC = LEUART_IFC_RXOF | LEUART_IFC_RXUF;

	rx_data_from_BG[data_count] = LEUART0->RXDATA;
	data_count++;

  if(data_count == 16) {
#if 0 
    /* Get the decryption key */
    AES_DecryptKey128(decryptionKey, exampleKey);
    
    /* Devcrypt the data now! */
    AesCBC128DmaDecrypt(decryptionKey,\
        rx_data_from_BG,\
        rx_data_from_BG,\
        SIZEOF_DMA_BLOCK,\
        initVector);
#endif
    Decrypt_Data(&rx_data_from_BG[0]);
    /* Reset the counter */
    data_count = 0;
  }


	__enable_irq();
}

void Decrypt_Data(uint8_t *data)
{
  /* Reconfigure the AES engine */
  Config_AES();

  /* Get the decryption key */
  AES_DecryptKey128(decryptionKey, exampleKey);
  AES->IFC = AES_IFC_DONE;

  /* Devcrypt the data now! */
  AesCBC128DmaDecrypt(decryptionKey,\
      rx_data_from_BG,\
      rx_data_from_BG,\
      SIZEOF_DMA_BLOCK,\
      initVector);

  return;
}
  






