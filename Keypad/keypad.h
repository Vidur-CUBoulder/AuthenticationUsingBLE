/******************************************************************************
* File: keypad.h
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

#ifndef KEYPAD_H
#define KEYPAD_H




/*****************************************************
            * Include Statements *
 *****************************************************/
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_lcd.h"
#include "segmentlcd.h"
#include "em_emu.h"


/*****************************************************
            * Define Statements *
 *****************************************************/
#define KEY1Port    gpioPortB           //Keypad 1 pin 4
#define KEY1Pin     9
#define KEY2Port    gpioPortC           //Keypad 2 pin 4
#define KEY2Pin     4
#define KEY3Port    gpioPortC           //Keypad 3 pin 1
#define KEY3Pin     6
#define KEY4Port    gpioPortD           //Keypad 1 pin 3
#define KEY4Pin     2
#define KEY5Port    gpioPortC           //Keypad 2 pin 3
#define KEY5Pin     5
#define KEY6Port    gpioPortD           //Keypad 3 pin 2
#define KEY6Pin     7
#define KEY7Port    gpioPortB           //Keypad 1 pin 2
#define KEY7Pin     10
#define KEY8Port    gpioPortC           //Keypad 2 pin 2
#define KEY8Pin     0
#define KEY9Port    gpioPortB           //Keypad 3 pin 3
#define KEY9Pin     11
#define KEY0Port    gpioPortC           //Keypad 2 pin 1
#define KEY0Pin     3
#define KEYOnPort   gpioPortF           //Keypad 1 pin 1
#define KEYOnPin    1
#define KEYEnterPort    gpioPortB       //Keypad 3 pin 4
#define KEYEnterPin     12



/*****************************************************
            * Global Variables *
 *****************************************************/
typedef enum keypad_t
{
    clear = 0,
    key1 = 1,
    key2,
    key3,
    key4,
    key5,
    key6,
    key7,
    key8,
    key9,
    key0,
    keyOn,
    keyEnter
}keypad;


unsigned int key_pressed;                       //Variable to store the key pressed
bool oddstate;                                  //To debug odd irq handler
bool evenstate;                                 //To debug even irq handler


/************************************************************************
* Setup and initialize all buttons of the keypad
*
* Input variables: None
*
* Global variables: None
*
* Returned variables: None
**************************************************************************/
void keypad_setup(void);


#endif /* KEYPAD_H */

