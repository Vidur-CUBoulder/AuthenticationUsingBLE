/******************************************************************************
* File: leuart.c
*
* Created on: 19-Apr-2017
* Author: Shalin Shah
* 
*******************************************************************************
* @section License
* <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
*******************************************************************************
*
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
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
* obligation to support this Software. Silicon Labs is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Silicon Labs will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
******************************************************************************
******************************************************************************
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "leuart.h"



/************************************************************************
* Function to setup and initialize LEUART0
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void leuart_setup(){

    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);   //Select LFXO as the clock for LFB clock tree to the leuart
    CMU_ClockEnable(cmuClock_CORELE, true);
    CMU_ClockEnable(cmuClock_LEUART0, true);
    GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);      //Initialize and set TX pin as output
    GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 1);         //Initialize and set RX pin as input

    LEUART_Init_TypeDef leuart_init =
    {
      .enable = false,                                  //Do not enable on initialization
      .refFreq = LEUART_NOREF,                          //Use current clock as reference for configuring the baud rate
      .baudrate = LEUART_BAUD,                          //Set the LEUART BAUD rate
      .databits = leuartDatabits8,                      //Use 8 data bits
      .parity = leuartNoParity,                         //Don't use any parity
      .stopbits = leuartStopbits1                       //Use 1 stop bit
    };

    LEUART_Init(LEUART0, &leuart_init);                 //Initialize the LEUART

    LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION; //Select location 0, PD4- TX, PD5- RX
    LEUART_IntDisable(LEUART0,LEUART_DIS_ALL_INT);      //Disable all interrupts
    LEUART_IntClear(LEUART0, LEUART_CLEAR_ALL_INT);     //Clear all interrupt flags
    LEUART_RxDmaInEM2Enable(LEUART0,true);
    LEUART_TxDmaInEM2Enable(LEUART0,true);
    LEUART_Enable(LEUART0, true);                       //Enable LEUART
    LEUART0->CMD = LEUART_CMD_TXEN | LEUART_CMD_RXEN;
}

void leuart_dma_setup_rx()
{
    DMA_Init_TypeDef DMA_init =
    {
        .controlBlock = dmaControlBlock,                //Define the dma control block
        .hprot = 0                                      //No channel protection required
    };
    DMA_Init(&DMA_init);                                //Initialize the dma

    LEUART_cb.cbFunc = leuart_dma_done_rx;
    LEUART_cb.primary = true;
    LEUART_cb.userPtr = NULL;

    DMA_CfgDescr_TypeDef LEUART_DMA_cfg =
    {
       .arbRate = dmaArbitrate1,                            //Set arbitration to 0
       .dstInc = dmaDataInc1,                               //Increase destination address by 2 bytes
       .hprot = 0,                                         //No channel protection required
       .size = dmaDataSize1,                                    //Increase data size by 2 bytes
       .srcInc = dmaDataIncNone                            //Don not increment source address
    };
    DMA_CfgDescr(LEUART_DMA_CHANNEL_RX, true, &LEUART_DMA_cfg);    //Initialize the configure descriptor

    DMA_CfgChannel_TypeDef LEUART_DMA_channel =
    {
        .cb = &LEUART_cb,                                   //Assign the callback function pointer
        .enableInt = true,                                  //Enable interrupts
        .highPri = true,                                    //Set as high priority
        .select = DMAREQ_LEUART0_RXDATAV                        //Select ADC0 singlemode as input
    };
    DMA_CfgChannel(LEUART_DMA_CHANNEL_RX, &LEUART_DMA_channel);    //Initialize the channel configuration

    DMA->IFC = 1 << LEUART_DMA_CHANNEL_RX;                       //Clear all interrupt flags
    DMA->IEN = 1 << LEUART_DMA_CHANNEL_RX;                       //Enable interrupt for DMA complete

}




void leuart_dma_setup_tx(void)
{
    DMA_Init_TypeDef DMA_init =
    {
        .controlBlock = dmaControlBlock,                //Define the dma control block
        .hprot = 0                                      //No channel protection required
    };
    DMA_Init(&DMA_init);                                //Initialize the dma

    LEUART_cb.cbFunc = leuart_dma_done;
    LEUART_cb.primary = true;
    LEUART_cb.userPtr = NULL;

    DMA_CfgDescr_TypeDef LEUART_DMA_cfg =
    {
       .arbRate = dmaArbitrate1,                            //Set arbitration to 0
       .dstInc = dmaDataIncNone,                               //Increase destination address by 2 bytes
       .hprot = 0,                                         //No channel protection required
       .size = dmaDataSize1,                                    //Increase data size by 2 bytes
       .srcInc = dmaDataInc1                            //Don not increment source address
    };
    DMA_CfgDescr(LEUART_DMA_CHANNEL, true, &LEUART_DMA_cfg);    //Initialize the configure descriptor

    DMA_CfgChannel_TypeDef LEUART_DMA_channel =
    {
        .cb = &LEUART_cb,                                   //Assign the callback function pointer
        .enableInt = true,                                  //Enable interrupts
        .highPri = true,                                    //Set as high priority
        .select = DMAREQ_LEUART0_TXBL                        //Select ADC0 singlemode as input
    };
    DMA_CfgChannel(LEUART_DMA_CHANNEL, &LEUART_DMA_channel);    //Initialize the channel configuration

    DMA->IFC = 1 << LEUART_DMA_CHANNEL;                       //Clear all interrupt flags
    DMA->IEN = 1 << LEUART_DMA_CHANNEL;                       //Enable interrupt for DMA complete
}


void leuart_dma_done(unsigned int channel, bool primary, void *user)
{
    __disable_irq();
    DMA->CONFIG &= ~DMA_CONFIG_EN;                          //Disable DMA
    DMA->IFC = 1 << LEUART_DMA_CHANNEL;                     //Clear all interrupt flags
    __enable_irq();
}

#if 0
void leuart_dma_done_rx(unsigned int channel, bool primary, void *user)
{
	__disable_irq();
	DMA->CONFIG &= ~DMA_CONFIG_EN;                          //Disable DMA
	DMA->IFC = 1 << LEUART_DMA_CHANNEL;                     //Clear all interrupt flags
	__enable_irq();
}
#endif

