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

  /* Initialize the LCD */
  SegmentLCD_Init(false);

  /* Do the clock config. now */
  Central_Clock_Setup(cmuSelect_LFXO);

  /* Setup the LEUART */
  Setup_LEUART();

  /* Configure the AES engine */
  Config_AES();

  /* Select the sleep mode that you want to enter */
  blockSleepMode(SEL_SLEEP_MODE);
  
  /* Only send the command to gen. a new pin */
  packet_to_BG.command = set_new_pin;
  packet_to_BG.data_length = 0;
  create_packet(packet_to_BG);

  /* Infinite loop */
  while (1) {
	  sleep();
  }
}
