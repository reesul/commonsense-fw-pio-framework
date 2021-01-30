/*************************
 * 
 *  CommonSense Firmware
 * 
 * ***********************/

#pragma once

#include <sam.h>


#define PORT_BASE  (*(RwReg  *)0x41008000UL) 

#define PORT_OFFSET 0x80
#define PORT_A_OFFSET PORT_OFFSET * 0
#define PORT_B_OFFSET PORT_OFFSET * 1
#define PORT_C_OFFSET PORT_OFFSET * 2
#define PORT_D_OFFSET PORT_OFFSET * 3

#define MODE_INPUT           0
#define MODE_OUTPUT          1
#define MODE_INPUT_PULLUP    2
#define MODE_INPUT_PULLDOWN  3

#define DIRECTION_INPUT   0
#define DIRECTION_OUTPUT  1

#define PIN_OUTPUT_HIGH     1
#define PIN_OUTPUT_LOW      0


void cs_pin_config(uint8_t port, uint8_t pin, uint8_t mode);

void cs_pin_direction(uint8_t port, uint8_t pin, uint8_t direction);

void cs_pin_output(uint8_t port, uint8_t pin, uint8_t direction);

void cs_pin_reset(uint8_t port, uint8_t pin);

void cs_pin_set_pinmux(uint8_t port, uint8_t pin, uint8_t mux_option);