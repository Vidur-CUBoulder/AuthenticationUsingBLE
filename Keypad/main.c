
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "keypad.h"
#include "segmentlcd.h"
#include "em_cmu.h"
#include "letimer.h"
#include "bsp_trace.h"
#include "leuart.h"

int main(void)
{
    /* Chip errata */
    CHIP_Init();

    BSP_TraceProfilerSetup();
    /* Enable LCD without voltage boost */
    SegmentLCD_Init(false);

    /* Aring functions and delay put in between setups for boot-up display*/
    SegmentLCD_ARing(0, true);
    SegmentLCD_ARing(1, true);
    LETIMER_Setup();                //LETMIER0 setup, 1 sec delay to stabilize oscillator

    SegmentLCD_ARing(2, true);
    SegmentLCD_ARing(3, true);
    LETIMER_Delay(100);
    leuart_setup();                 //Setup leuart

    SegmentLCD_ARing(4, true);
    SegmentLCD_ARing(5, true);
    LETIMER_Delay(100);
    leuart_dma_setup();             //Setup DMA for UART

    SegmentLCD_ARing(6, true);
    SegmentLCD_ARing(7, true);
    LETIMER_Delay(100);

    /* Setups done. Disable the ring */
    SegmentLCD_ARing(0, false);
    SegmentLCD_ARing(1, false);
    SegmentLCD_ARing(2, false);
    SegmentLCD_ARing(3, false);
    SegmentLCD_ARing(4, false);
    SegmentLCD_ARing(5, false);
    SegmentLCD_ARing(6, false);
    SegmentLCD_ARing(7, false);



    SegmentLCD_Symbol(LCD_SYMBOL_GECKO, true);
    LETIMER_Delay(100);
    SegmentLCD_Write("Hello");
    LETIMER_Delay(500);
    /* Setup and initialize keypad functionality, execute this last*/
    keypad_setup();

    /* Infinite loop */
    while (1)
    {
        SegmentLCD_Number(keypad_input_buffer[keypad_buffer_counter-1]);
        LEUART0->TXDATA = keypad_input_buffer[keypad_buffer_counter-1];
        EMU_EnterEM2(true);
        if(keypad_buffer_counter == 6)
            DMA_ActivateBasic(LEUART_DMA_CHANNEL, true, false, (void*)&LEUART0->TXDATA,(void*)keypad_input_buffer, 5);
    }
}
