/*************************
 * 
 *  CommonSense Firmware
 * 
 * ***********************/

/***
 * 
 * sad51/inclue/pio/samd51p20a is very helpful here. Found in cmsis-atmel framework
 * 
 */
#include "cs_pin_config.h"

/***
 * Return 0 if the pin and port combo do not exist
 * */
uint8_t _is_valid_port_pin(uint8_t port, uint8_t pin) {
    if (port < 0  || port > 4) return 0;
    else if(pin < 0 || pin > 31) return 0;
    else return 1;
    //This is a simple, yet technically incorrect way to check. Some ports do not have specific pins. 
        //Regardless, they still have register values that can be written to, so technically no damage would be done. -Reese
}

/**
 * Configure port/pin with a few basic options. 
 * 
 *   If more complex operation is needed from the pin, then it may need custom configuration. If it for a peripheral, the alternative pin-config for peripheral muxing
 */
void cs_pin_config(uint8_t port, uint8_t pin, uint8_t mode) {
    if (!_is_valid_port_pin(port, pin)) return;

    switch (mode) {
        case MODE_INPUT: 
            cs_pin_direction(port, pin, DIRECTION_INPUT);
            PORT->Group[port].PINCFG[pin].reg = (uint8_t) PORT_PINCFG_INEN;
            break;
        case MODE_INPUT_PULLDOWN:
            cs_pin_direction(port, pin, DIRECTION_INPUT);
            PORT->Group[port].PINCFG[pin].reg = (uint8_t) (PORT_PINCFG_INEN | PORT_PINCFG_PULLEN);
            cs_pin_output(port, pin, PIN_OUTPUT_LOW);
            break;
        case MODE_INPUT_PULLUP:
            cs_pin_direction(port, pin, DIRECTION_INPUT);
            PORT->Group[port].PINCFG[pin].reg = (uint8_t) (PORT_PINCFG_INEN | PORT_PINCFG_PULLEN);
            cs_pin_output(port, pin, PIN_OUTPUT_HIGH);

            break;
        case MODE_OUTPUT:
            cs_pin_direction(port, pin, DIRECTION_OUTPUT);
            //INEN? State of pin cannot be read without this option
            break;
        default:
            cs_pin_reset(port, pin);

            break;

    }

}

void cs_pin_direction(uint8_t port, uint8_t pin, uint8_t direction) {

    if (direction != 0) {
        PORT->Group[port].DIRSET.reg = 1 << pin;
    } 
    else {
        PORT->Group[port].DIRCLR.reg = 1 << pin;
    }
}

/****
 * This function assumes the pins are already configured to be outputs. 
 *   If not, then this may have strange effects because the OUT register is mulitplexed for specifying pullup/down direction when pin is configured as input
 * */
void cs_pin_output(uint8_t port, uint8_t pin, uint8_t direction) {
    // if (!_is_valid_port_pin(port, pin)) return;

    if (direction != 0) {
        PORT->Group[port].OUTSET.reg = 1 << pin;
    } 
    else {
        PORT->Group[port].OUTCLR.reg = 1 << pin;
    }

}


void cs_pin_reset(uint8_t port, uint8_t pin) {
    cs_pin_direction(port, pin, DIRECTION_INPUT);
    PORT->Group[port].PINCFG[pin].reg = 0;
}

/****
 * Mux options are into 4-bits in 8 bit registers, where the top 4 bits are for odd numbers pins.
 *      A mux_option of 0 corresponds to peripheral function A, 1 to B, and so on
 * */
void cs_pin_set_pinmux(uint8_t port, uint8_t pin, uint8_t mux_option) {
    cs_pin_reset(port, pin);   //expecting the user to fully setup the pin whenever they need it, not relying on existing state
    PORT->Group[port].PINCFG[pin].reg = (uint8_t) (PORT_PINCFG_PMUXEN);

    uint8_t mask = 0xFF;

    if (pin % 2) {
        mask = mask << 4;
        mux_option = mux_option << 4;
    }

    PORT->Group[port].PMUX[pin/2].reg &= ~mask;
    PORT->Group[port].PMUX[pin/2].reg |= (mask & mux_option);

}