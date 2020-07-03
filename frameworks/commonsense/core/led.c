#include "led.h"

void led_setup() {
    
    pin_direction(LED_PORT, LED_PIN_RED, DIRECTION_OUTPUT);
    pin_direction(LED_PORT, LED_PIN_GREEN, DIRECTION_OUTPUT);
    pin_direction(LED_PORT, LED_PIN_BLUE, DIRECTION_OUTPUT);

    pin_output(LED_PORT, LED_PIN_RED, LED_OFF);
    pin_output(LED_PORT, LED_PIN_GREEN, LED_OFF);
    pin_output(LED_PORT, LED_PIN_BLUE, LED_OFF);

}

void led_on(uint8_t pin) {

    pin_output(LED_PORT, pin, LED_ON);
    
}

void led_off(uint8_t pin) {
    
    pin_output(LED_PORT, pin, LED_OFF);
}
