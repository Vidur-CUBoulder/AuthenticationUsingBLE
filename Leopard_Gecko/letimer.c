/******************************************************************************
* File: letimer.c
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
#include "letimer.h"

/************************************************************************
* Setup and initialize the LETIMER
*
* Input variables: None
*
* Global variables: CurrentLFAFreq
*
* Returned variables: None
**************************************************************************/
void LETIMER_Setup(void)
{
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      //Enable LXF0 clock oscillator
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   //Select LFXO as the clock for LFA clock tree to the letimer
    CMU_ClockEnable(cmuClock_CORELE, true);                 //Enable clock to Low energy peripheral clock tree
    CMU_ClockEnable(cmuClock_LETIMER0, true);               //Enable clock to letimer0


    const LETIMER_Init_TypeDef letimerInit =
    {
        .enable         = false,                //Start counting when init completed
        .debugRun       = false,                //Counter shall not keep running during debug halt
        .rtcComp0Enable = false,                //Don't start counting on RTC COMP0 match
        .rtcComp1Enable = false,                //Don't start counting on RTC COMP1 match
        .comp0Top       = false,                //Don't load COMP0 register into CNT when counter underflows.
        .bufTop         = false,                //Don't load COMP1 into COMP0 when REP0 reaches 0
        .out0Pol        = 0,                    //Idle value for output 0.
        .out1Pol        = 0,                    //Idle value for output 1.
        .ufoa0          = letimerUFOANone,      //PWM output on output 0
        .ufoa1          = letimerUFOANone,      //Pulse output on output 1
        .repMode        = letimerRepeatOneshot  //Count until stopped
    };

    LETIMER_Init(LETIMER0, &letimerInit);                       //Initilize LETIMER0
    while(LETIMER0->SYNCBUSY);                                  //Wait till SYNCBUSY is cleared
    LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1); //Clear all interrupts
    LETIMER_IntEnable(LETIMER0 , LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);    //Enable interrupts for COMP0 and COMP1
    NVIC_EnableIRQ(LETIMER0_IRQn);                              //Enable global interrupts for LETIMER0
}



void LETIMER_Delay(uint32_t milliseconds)
{
    float Period;                               //Variable for LETIMER period
    uint8_t LETIMER0_Prescaler;
    CurrentLFAFreq = CMU_ClockFreqGet(cmuClock_LFA);        //Get current frequency of LFA clock tree
    Period = (CurrentLFAFreq * milliseconds);               //Else, calculate period for LETIMER0
    Period = Period / 1000;
    LETIMER0_Prescaler = 0;                                     //Initialize prescaler to 0
    while(Period > LETIMER_MAX_COUNT)                           //Till the period exceeds the max value of LETIMER counter
    {
        LETIMER0_Prescaler++;                                   //Increment the value of prescaler
        Period/=2;                                              //Divide period by the power of 2
    }
    CMU->LFAPRESC0 = (LETIMER0_Prescaler<<8);                   //Set the LFA LETIMER prescaler value with the prescaler value
    LETIMER0->CNT = Period;
    LETIMER_Enable(LETIMER0, true);
    while(LETIMER0->CNT);
    LETIMER_Enable(LETIMER0, false);
}



/************************************************************************
* Interrupt handler routine for LETIMER0
*
* Input variables: None
*
* Global variables: ACMPstatus, periodCount
*
* Returned variables: None
**************************************************************************/
void LETIMER0_IRQHandler(void)
{
    LETIMER0->IFC = LETIMER_IntGet(LETIMER0);    //Get the interrupting flags

}
