#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "em_device.h"
#include "em_chip.h"
#include "sleep_modes.h"
#include "em_aes.h"
#include "leuart.h"
#include "em_gpio.h"
#include "aes_cbc_128_dma.h"
#include "keypad.h"
#include "letimer.h"
#include "circular_buffer.h"

#include "em_dma.h"
#include "dmactrl.h"



const uint8_t exampleKey[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                               0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

const uint8_t initVector[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};



