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
* emlib library of Silicon Labs for Leopard Gecko development board
* used in compliance with the licenses and copyrights.
******************************************************************************/

/*****************************************************
            * Include Statements *
 *****************************************************/
#include "keypad.h"



/* Setup and initialize all buttons of the keypad */
void keypad_setup(void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);       //Enable GPIO clock tree
    keypad_enable();                            //Enable keypad
    memset(keypad_input_buffer, 0, 6);          //Reset keypad buffer
    keypad_buffer_counter = 0;                  //Reset keypad counter
}




/*Enable all buttons of the keypad*/
void keypad_enable(void)
{
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


    /*EM4 setup*/
    GPIO->CTRL = GPIO_CTRL_EM4RET;                      //Enable EM4 retention
    GPIO->CMD = GPIO_CMD_EM4WUCLR;                      //Clear pending wake up requests
    GPIO->EM4WUEN = GPIO_EM4WUEN_EM4WUEN_F1 | GPIO_EM4WUEN_EM4WUEN_F1;      //Enable EM4 wake-up
    GPIO->EM4WUPOL = GPIO_EM4WUPOL_EM4WUPOL_F2;         //Set wake-up polarity as 1 for F2, 0 for others



    /*Set falling edge interrupts for all keys*/
    GPIO_IntConfig(KEY1Port, KEY1Pin, false, true, true);
    GPIO_IntConfig(KEY2Port, KEY2Pin, false, true, true);
    GPIO_IntConfig(KEY3Port, KEY3Pin, false, true, true);
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




/*Disable all keys of the keypad*/
void keypad_disable(void)
{
    /* Disable all the previously set pins*/
    GPIO_PinModeSet(KEY1Port, KEY1Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY2Port, KEY2Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY3Port, KEY3Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY4Port, KEY4Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY5Port, KEY5Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY6Port, KEY6Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY7Port, KEY7Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY8Port, KEY8Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY9Port, KEY9Pin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEY0Port, KEY0Pin, gpioModeDisabled, 1);
   // GPIO_PinModeSet(KEYOnPort, KEYOnPin, gpioModeDisabled, 1);
    GPIO_PinModeSet(KEYEnterPort, KEYEnterPin, gpioModeDisabled, 1);

    /* Disable interrupts for all pins*/
    GPIO_IntConfig(KEY1Port, KEY1Pin, false, false, false);
    GPIO_IntConfig(KEY2Port, KEY2Pin, false, false, false);
    GPIO_IntConfig(KEY3Port, KEY3Pin, false, false, false);
    GPIO_IntConfig(KEY4Port, KEY4Pin, false, false, false);
    GPIO_IntConfig(KEY5Port, KEY5Pin, false, false, false);
    GPIO_IntConfig(KEY6Port, KEY6Pin, false, false, false);
    GPIO_IntConfig(KEY7Port, KEY7Pin, false, false, false);
    GPIO_IntConfig(KEY8Port, KEY8Pin, false, false, false);
    GPIO_IntConfig(KEY9Port, KEY9Pin, false, false, false);
    GPIO_IntConfig(KEY0Port, KEY0Pin, false, false, false);
   // GPIO_IntConfig(KEYOnPort, KEYOnPin, false, false, false);
    GPIO_IntConfig(KEYEnterPort, KEYEnterPin, false, false, false);

    /* Clear Even pending interrupts and disable them*/
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_DisableIRQ(GPIO_EVEN_IRQn);

    /* Clear Odd pending interrupts and disable them*/
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
   // NVIC_DisableIRQ(GPIO_ODD_IRQn);
}




/*Function to display scrolling text on the display*/
void LCD_Scroll(char *text)
{
  int  i, len;              //Counter and total length variables
  char buffer[8];           //Temporary display buffer

  buffer[7] = 0x00;         //Initialize buffer to 0;
  len       = strlen(text); //Calculate length of the text.
  if (len < 7)              //If length less than 7, display text, cannot scroll
  {
      SegmentLCD_Write(text);
      return;
  }
  for (i = 0; i < (len - 7); i++)       //Write 7 chars at a time and shift them every 200 ms
  {
    memcpy(buffer, text + i, 7);
    SegmentLCD_Write(buffer);
    LETIMER_Delay(200);
  }
}



/*Function to display Pin*/
void PIN_Display(bool showlast)
{
    memset(keypad_display, 0 , 6);      //Clear the display string
    if(showlast)
    {
        memset(keypad_display, 'X', keypad_buffer_counter - 1); //Write stars except last bit
        keypad_display[keypad_buffer_counter] = keypad_input_buffer[keypad_buffer_counter] + 48;    //Write the ASCII of the last digit
    }
    else
        memset(keypad_display, 'X', keypad_buffer_counter); //Write stars for all bits
    SegmentLCD_Write(keypad_display);       //Display the pin
}





void GPIO_ODD_IRQHandler(void)
{
    __disable_irq();
    uint16_t intFlags;
    intFlags = GPIO_IntGet();

    if(intFlags & (1<<1)){
        key_pressed = keyOn;                        //Key Backspace is pressed
        if(keypad_buffer_counter>0)
            keypad_input_buffer[--keypad_buffer_counter] = 0;       //Go back one place if total elements >0
    }
    else if (keypad_buffer_counter < 6)                 //Ignore keypad inputs if buffer full
    {
        if(intFlags & (1<<3))
            key_pressed = key0;                         //Key 0 is pressed
        else if(intFlags & (1<<5))
            key_pressed = key5;                         //key 5 is pressed
        else if(intFlags & (1<<7))
            key_pressed = key6;                         //key 6 is pressed
        else if(intFlags & (1<<9))
            key_pressed = key1;                         //Key 1 is pressed
        else if(intFlags & (1<<11))
            key_pressed = key9;                         //key 9 is pressed

        keypad_input_buffer[keypad_buffer_counter++] = key_pressed;     //Write the key pressed to the buffer
    }
    LETIMER_Delay(400);                                     //Blocking delay for debouncing
    PIN_Display(false);
    GPIO_IntClear(intFlags);                                //Clear interrupt flags
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);                   //Clear pending interrupts occurred during debouncing
    __enable_irq();
}








void GPIO_EVEN_IRQHandler(void)
{
    __disable_irq();
    uint16_t intFlags;
    intFlags = GPIO_IntGet();

    if(intFlags & (1<<12))
    {
        key_pressed = keyEnter;                     //Key Enter is pressed
        if(keypad_buffer_counter == 6)              //If 6 digits of pin are entered then start encryption and send it via UART
        {
            LEUART0_Send_Byte(0xA);
            LCD_Scroll("AUTHENTICATING  ");         //Display Authenticating message on LCD
            keypad_disable();                       //Disable keypad, its work is done
            EMU_EnterEM4();
        }
        else
        {
            LCD_Scroll("Error! ENTER PIN AGAIN  "); //Less than 6 digits entered, display error
            memset(keypad_input_buffer, 0, 6);      //Reset the buffer and counter
            keypad_buffer_counter = 0;
        }
    }
    else if(keypad_buffer_counter < 6)              //Ignore keypad inputs if buffer full
    {
        if(intFlags & (1<<0))
            key_pressed = key8;                         //Key 8 is pressed
        else if(intFlags & (1<<2))
            key_pressed = key4;                         //Key 4 is pressed
        else if(intFlags & (1<<4))
            key_pressed = key2;                         //Key 2 is pressed
        else if(intFlags & (1<<6))
            key_pressed = key3;                         //Key 3 is pressed
        else if(intFlags & (1<<10))
            key_pressed = key7;                         //Key 7 is pressed

        keypad_input_buffer[keypad_buffer_counter++] = key_pressed;     //Write key pressed to the buffer
    }
    LETIMER_Delay(400);                                     //Blocking delay for debouncing
    PIN_Display(false);
    GPIO_IntClear(intFlags);                            //Clear interrupt flags
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);               //Clear pending interrupts occurred by debouncing
    __enable_irq();
}

