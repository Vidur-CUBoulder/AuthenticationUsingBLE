/******************************************************************************
* File: keypad.c
*
* Created on: 05-Apr-2017
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
*
* The functions that use this library are:
* 1.
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "keypad.h"


/************************************************************************
* Setup and initialize all buttons of the keypad
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
*
* IP
**************************************************************************/
void keypad_setup(void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);       //Enable GPIO clock tree

    /* Set all required pins as input with a pull-up resistor*/
    GPIO_PinModeSet(KEY1Port, KEY1Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY2Port, KEY2Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY3Port, KEY3Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY4Port, KEY4Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY5Port, KEY5Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY6Port, KEY6Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY7Port, KEY7Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY8Port, KEY8Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY9Port, KEY9Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEY0Port, KEY0Pin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEYOnPort, KEYOnPin, gpioModeInputPull, 1);
    GPIO_PinModeSet(KEYEnterPort, KEYEnterPin, gpioModeInputPull, 1);


    /*Set falling edge interrupts for all keys*/

    GPIO_IntConfig(KEY1Port, KEY1Pin, false, true, true);
    GPIO_IntConfig(KEY2Port, KEY2Pin, false, true, true);
#if 0
    GPIO_IntConfig(KEY3Port, KEY3Pin, false, true, true);
#endif
    GPIO_IntConfig(KEY4Port, KEY4Pin, false, true, true);
    GPIO_IntConfig(KEY5Port, KEY5Pin, false, true, true);
    GPIO_IntConfig(KEY6Port, KEY6Pin, false, true, true);
    GPIO_IntConfig(KEY7Port, KEY7Pin, false, true, true);
    GPIO_IntConfig(KEY8Port, KEY8Pin, false, true, true);
    GPIO_IntConfig(KEY9Port, KEY9Pin, false, true, true);
    GPIO_IntConfig(KEY0Port, KEY0Pin, false, true, true);
    GPIO_IntConfig(KEYOnPort, KEYOnPin, false, true, true);
    GPIO_IntConfig(KEYEnterPort, KEYEnterPin, false, true, true);

    /*Clear and enable even gpio interrupts*/
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);

    /*Clear and enable odd gpio interrupts*/
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);

}


void GPIO_ODD_IRQHandler(void)
{
    __disable_irq();
    uint16_t intFlags;
    intFlags = GPIO_IntGet();

    if(intFlags & (1<<1))
    {
        if((GPIO_PortInGet(gpioPortF) & (1<<1)) == 0)     //Pin F1
            key_pressed = keyOn;                        //Key On is pressed
        if((GPIO_PortInGet(gpioPortD) & (1<<1)) == 0)          //Pin D1
            key_pressed = key1;                         //Key 1 is pressed
    }
    else if(intFlags & (1<<3))
    {
        if((GPIO_PortInGet(gpioPortC) & (1<<3)) == 0)          //Pin C3
            key_pressed = key0;                         //Key 0 is pressed
    }
    else if(intFlags & (1<<5))
    {
        if((GPIO_PortInGet(gpioPortC) & (1<<5)) == 0)   //Pin C5
            key_pressed = key5;                         //key 5 is pressed
    }
    else if(intFlags & (1<<7))
    {
        if((GPIO_PortInGet(gpioPortD) & (1<<7)) == 0)          //Pin D7
            key_pressed = key6;                         //key 6 is pressed
    }
    else if(intFlags & (1<<11))
    {
        if((GPIO_PortInGet(gpioPortB) & (1<<11)) == 0)         //Pin B11
            key_pressed = key9;                         //key 9 is pressed
    }
    if(oddstate == true)
        oddstate = false;
    else
        oddstate = true;
    SegmentLCD_EnergyMode(0, oddstate);
    GPIO_IntClear(intFlags);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    __enable_irq();
}



void GPIO_EVEN_IRQHandler(void)
{
    __disable_irq();
    uint16_t intFlags;
    intFlags = GPIO_IntGet();

    if(intFlags & (1<<0))
    {
        if((GPIO_PortInGet(gpioPortC) & (1<<0)) == 0)     //Pin C0
            key_pressed = key8;                         //Key 8 is pressed
        else if((GPIO_PortInGet(gpioPortD) & (1<<0)) == 0)          //Pin D0
            key_pressed = key7;                         //Key 7 is pressed
    }
    else if(intFlags & (1<<2))
    {
        if((GPIO_PortInGet(gpioPortD) & (1<<2)) == 0)          //Pin D2
            key_pressed = key4;                         //Key 4 is pressed
    }
    else if(intFlags & (1<<4))
    {
        if((GPIO_PortInGet(gpioPortC) & (1<<4)) == 0)          //Pin C4
            key_pressed = key2;                         //Key 2 is pressed
    }
    else if(intFlags & (1<<6))
    {
        if((GPIO_PortInGet(gpioPortC) & (1<<6)) == 0)          //Pin C6
            key_pressed = key3;                         //Key 3 is pressed
    }
    else if(intFlags & (1<<12))
    {
        if((GPIO_PortInGet(gpioPortB) & (1<<12)) == 0)         //Pin B12
            key_pressed = keyEnter;                         //Key Enter is pressed
    }

    if(evenstate == true)
        evenstate = false;
    else
        evenstate = true;
    SegmentLCD_Symbol(LCD_SYMBOL_EFM32, evenstate);
    GPIO_IntClear(intFlags);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    __enable_irq();
}

