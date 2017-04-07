#include "main.h"

/******* Sleep Modes ********/
#define SEL_SLEEP_MODE sleepEM2

#define AES_DATA_SIZE 64

#define SIZEOF_DMA_BLOCK (sizeof(dataBuffer) / (sizeof(dataBuffer[0]) * 16))

#define delay(X) for(int i=0; i<X; i++)

#define ENABLE_AES_INTERRUPT  {\
							   AES->IFC = AES_IFC_DONE;\
  	  	  	  	  	  	  	   AES->IEN = AES_IEN_DONE;\
  	  	  	  	  	  	  	   NVIC_EnableIRQ(AES_IRQn);\
							  }


/************************ END OF DEFINES *********************/
/********************** Global Variables ******************/
uint8_t aes_byte_counter = 0;
uint8_t rx_test_buffer[64] = {0};

uint8_t aes_data_counter = 0;
uint8_t ret_data;

/* Store the AES data in a circular buffer */
c_buf aes_buffer;


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

	/* Select the low freq. clock */
	CMU_ClockSelectSet(cmuClock_LFA, osc_clk_type);

	/* Set the CORELE clock */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Set the list of clocks that you require after this! */
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_AES, true);

	return;
}

void GPIO_ODD_IRQHandler(void)
{
	INT_Disable();

	/* Get the pending interrupt */
	uint32_t pending_GPIO_interrupt = GPIO_IntGet();

	/* Clear the pending interrupt flag */
	GPIO_IntClear(pending_GPIO_interrupt);

	/* Clear the interrupt flag */
	AES->IFC = AES_IFC_DONE;

	/* Free any previously allocated circular buffer */
	free_buffer(aes_buffer.buf_start);

	/* Allocate a new buffer to store data in */
	Alloc_Buffer(&aes_buffer, AES_DATA_SIZE);

	/* Trigger the AES interrupt */
	AesCBC128DmaEncrypt(exampleKey,\
			exampleData,\
			dataBuffer,\
			SIZEOF_DMA_BLOCK,\
			initVector);

	INT_Enable();

	return;
}

void Config_GPIO(void)
{

  /* Configure PB9 as input */
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);

  /* Clear all the GPIO interrupt flags */
  GPIO->IFC = 0xFFFFFFFF;

  /* Set falling edge interrupt */
  GPIO_IntConfig(gpioPortB, 9, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);

  /* Enable the interrupt hadler for GPIO */
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  return;
}

void AES_IRQHandler(void)
{
	INT_Disable();

	/* Add the encrypted data to the aes_buffer */
	add_to_buffer(&aes_buffer, &dataBuffer, AES_DATA_SIZE);

	/* Clear the interrupt flag */
	AES->IFC = AES_IFC_DONE;

	/* Reset the aes counter */
	aes_byte_counter = 0;

	/* Do the transfer of the encrypted data via LEUART from here! */

	/* Remove a byte of data from the circular buffer and put it on the
	 * LEUART bus.
	 */
	remove_from_buffer(&aes_buffer, &ret_data, sizeof(uint8_t));
    LEUART0->TXDATA = ret_data;

	INT_Enable();
}

void Config_AES(void)
{
	/* Get the decryption key from the original key, needs to be done once for each key */
	AES_DecryptKey128(decryptionKey, exampleKey);

	/* Enable the AES to hit the interrupt handler on completion
	 * of encryption/decryption operations.
	 */
	ENABLE_AES_INTERRUPT;
}

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Do the clock config. now */
  Central_Clock_Setup(cmuSelect_LFXO);

  /* Setup the LEUART */
  Setup_LEUART();

  /* Do the GPIO config here */
  Config_GPIO();

  Config_AES();

  /* Select the sleep mode that you want to enter */
  blockSleepMode(SEL_SLEEP_MODE);
  
  /* Infinite loop */
  while (1) {
    sleep();
  }
}
