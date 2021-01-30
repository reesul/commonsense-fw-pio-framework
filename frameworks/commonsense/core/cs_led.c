#include "cs_led.h"

void cs_led_setup() {
    
    cs_pin_direction(LED_PORT, LED_PIN_RED, DIRECTION_OUTPUT);
    cs_pin_direction(LED_PORT, LED_PIN_GREEN, DIRECTION_OUTPUT);
    cs_pin_direction(LED_PORT, LED_PIN_BLUE, DIRECTION_OUTPUT);

    cs_pin_output(LED_PORT, LED_PIN_RED, LED_OFF);
    cs_pin_output(LED_PORT, LED_PIN_GREEN, LED_OFF);
    cs_pin_output(LED_PORT, LED_PIN_BLUE, LED_OFF);

}

void cs_led_on(uint8_t pin) {

    cs_pin_output(LED_PORT, pin, LED_ON);
    
}

void cs_led_off(uint8_t pin) {
    
    cs_pin_output(LED_PORT, pin, LED_OFF);
}
