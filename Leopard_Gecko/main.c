#include "main.h"

/******* Sleep Modes ********/
#define SEL_SLEEP_MODE sleepEM2

/* Function: Central_Clock_Setup(CMU_Osc_TypeDef osc_clk_type)
 * Parameters:
 *      osc_clk_type - the type of clock that you want to enable
 * Return:
 *      void
 * Description:
 *      - Use this function to enable any global or peripheral clocks
 */
void Central_Clock_Setup(CMU_Osc_TypeDef osc_clk_type)
{
	/* Setup the oscillator */
	CMU_OscillatorEnable(osc_clk_type, true, true);

	/* Enable peripheral clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);

	/* Select the low freq. clock */
	CMU_ClockSelectSet(cmuClock_LFA, osc_clk_type);

	/* Set the CORELE clock */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Set the list of clocks that you require after this! */
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_AES, true);

	return;
}


int main(void)
{
	/* Chip errata */
	CHIP_Init();

	ptr_decrypt = &decryptionKey[0];

	/* Initialize the LCD */
	SegmentLCD_Init(false);
#if 1
	SegmentLCD_ARing(0, true);
	SegmentLCD_ARing(1, true);
	LETIMER_Setup();                //LETMIER0 setup, 1 sec delay to stabilize oscillator

	SegmentLCD_ARing(2, true);
	SegmentLCD_ARing(3, true);
	LETIMER_Delay(100);

	/* Setup the LEUART */
	SegmentLCD_ARing(4, true);
	SegmentLCD_ARing(5, true);
	LETIMER_Delay(100);
	Setup_LEUART();

	/* Configure the AES engine */
	SegmentLCD_ARing(6, true);
	SegmentLCD_ARing(7, true);
	LETIMER_Delay(100);
	//CMU_ClockEnable(cmuClock_AES, true);
	//Config_AES();


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
#endif

	Central_Clock_Setup(cmuOsc_LFXO);
	AES_DecryptKey128(ptr_decrypt, exampleKey);

	/* Only send the command to gen. a new pin */
	packet_to_BG.command = set_new_pin;
	packet_to_BG.data_length = 0;
	create_packet(packet_to_BG);

	/* Select the sleep mode that you want to enter */
	blockSleepMode(SEL_SLEEP_MODE);

	keypad_setup();

	/* Infinite loop */
	while (1) {

		sleep();
	}
}
