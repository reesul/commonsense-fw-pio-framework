#include "cs_clock.h"

/**
 *  clock number is 0-11. Rate is in Hz. The src (source) is an oscillator ticking at X Hz, and the gclk will tick at X/div Hz (truncated)
 * */
void cs_init_gclk(uint8_t clk_id, uint8_t src, uint8_t divider){

    cs_config_gclk(clk_id,  src, divider);
    cs_enable_gclk(clk_id);
 
}

void cs_config_gclk(uint8_t clk_id, uint8_t src, uint8_t divider){

    GCLK->GENCTRL[clk_id].reg = GCLK_GENCTRL_SRC(src) |
        GCLK_GENCTRL_DIV(divider);

    GCLK->GENCTRL[clk_id].bit.IDC = divider%2;  //special bit needs to be set if the divisor is odd



    //general form of data: GCLK_GENCTRL_DIV(CONF_GCLK_GEN_1_DIV) | (CONF_GCLK_GEN_1_RUNSTDBY << GCLK_GENCTRL_RUNSTDBY_Pos) | (CONF_GCLK_GEN_1_DIVSEL << GCLK_GENCTRL_DIVSEL_Pos) | (CONF_GCLK_GEN_1_OE << GCLK_GENCTRL_OE_Pos) | (CONF_GCLK_GEN_1_OOV << GCLK_GENCTRL_OOV_Pos) | (CONF_GCLK_GEN_1_IDC << GCLK_GENCTRL_IDC_Pos) | (CONF_GCLK_GENERATOR_1_CONFIG << GCLK_GENCTRL_GENEN_Pos) | CONF_GCLK_GEN_1_SOURCE);   //that's a lot of config options to pass! All besides source, divider assumed 0
}


void cs_enable_gclk(uint8_t clk_id){
    GCLK->GENCTRL[clk_id].bit.GENEN = 1; //enable the clock

    while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(clk_id));
}

void cd_disable_gclk(uint8_t clk_id){

}