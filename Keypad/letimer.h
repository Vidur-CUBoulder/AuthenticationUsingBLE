/******************************************************************************
* File: letimer.h
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

#ifndef LETIMER_H
#define LETIMER_H


/*****************************************************
            * Include Statements *
 *****************************************************/
#include "em_letimer.h"
#include "em_cmu.h"
#include <stdbool.h>


/*****************************************************
            * Define Statements *
 *****************************************************/
#define MinimumEnergyMode EM2       //Minimum energy mode the system can enter, change this to change the minimum energy mode
#define LETIMER_MAX_COUNT 0xFFFF    //Max value of LETIMER counter

int CurrentLFAFreq;                              //Variable to store current tree clock frequency



/************************************************************************
* Setup and initialize the LETIMER
*
* Input variables: None
*
* Global variables: CurrentLFAFreq
*
* Returned variables: None
**************************************************************************/
void LETIMER_Setup(void);


void LETIMER_Delay(uint32_t milliseconds);


#endif /* SRC_LETIMER_H_ */
