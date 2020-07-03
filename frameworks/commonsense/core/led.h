/*************************
 * 
 *  CommonSense Firmware
 * 
 * ***********************/

#pragma once

#include <sam.h>

#include "pinConfig.h"

#ifdef __CS_V_2_0__

#define LED_ON 0
#define LED_OFF 1
#define LED_PORT 1 //port B
#define LED_PIN_RED 26
#define LED_PIN_RED_MASK PORT_PB26
#define LED_PIN_GREEN 27
#define LED_PIN_GREEN_MASK PORT_PB27
#define LED_PIN_BLUE 28
#define LED_PIN_BLUE_MASK PORT_PB28

#endif //__CS_V_2_0__


void led_setup();
void led_on(uint8_t pin);
void led_off(uint8_t pin);

