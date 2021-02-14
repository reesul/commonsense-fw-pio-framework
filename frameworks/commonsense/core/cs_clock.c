#include "cs_clock.h"

/**
 *  clock number is 0-11. Rate is in Hz. The src (source) is an oscillator ticking at X Hz, and the gclk will tick at X/div Hz (truncated)
 * */
void cs_clock_init_gclk(uint8_t clk_id, uint8_t src, uint8_t divider){

    GCLK->GENCTRL[clk_id].reg = GCLK_GENCTRL_SRC(src) |
        GCLK_GENCTRL_DIV(divider) | divider%2 << GCLK_GENCTRL_IDC_Pos |
        GCLK_GENCTRL_GENEN;

    //general form of data: GCLK_GENCTRL_DIV(CONF_GCLK_GEN_1_DIV) | (CONF_GCLK_GEN_1_RUNSTDBY << GCLK_GENCTRL_RUNSTDBY_Pos) | (CONF_GCLK_GEN_1_DIVSEL << GCLK_GENCTRL_DIVSEL_Pos) | (CONF_GCLK_GEN_1_OE << GCLK_GENCTRL_OE_Pos) | (CONF_GCLK_GEN_1_OOV << GCLK_GENCTRL_OOV_Pos) | (CONF_GCLK_GEN_1_IDC << GCLK_GENCTRL_IDC_Pos) | (CONF_GCLK_GENERATOR_1_CONFIG << GCLK_GENCTRL_GENEN_Pos) | CONF_GCLK_GEN_1_SOURCE);   //that's a lot of config options to pass! All besides source, divider are assumed as 0

    while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(clk_id));
 
}

void cs_clock_config_gclk(uint8_t clk_id, uint8_t src, uint8_t divider){


    GCLK->GENCTRL[clk_id].reg = GCLK_GENCTRL_SRC(src) |
        GCLK_GENCTRL_DIV(divider) | divider%2 << GCLK_GENCTRL_IDC_Pos |
        GCLK_GENCTRL_GENEN;

    //general form of data: GCLK_GENCTRL_DIV(CONF_GCLK_GEN_1_DIV) | (CONF_GCLK_GEN_1_RUNSTDBY << GCLK_GENCTRL_RUNSTDBY_Pos) | (CONF_GCLK_GEN_1_DIVSEL << GCLK_GENCTRL_DIVSEL_Pos) | (CONF_GCLK_GEN_1_OE << GCLK_GENCTRL_OE_Pos) | (CONF_GCLK_GEN_1_OOV << GCLK_GENCTRL_OOV_Pos) | (CONF_GCLK_GEN_1_IDC << GCLK_GENCTRL_IDC_Pos) | (CONF_GCLK_GENERATOR_1_CONFIG << GCLK_GENCTRL_GENEN_Pos) | CONF_GCLK_GEN_1_SOURCE);   //that's a lot of config options to pass! All besides source, divider are assumed as 0


    // GCLK->GENCTRL[clk_id].bit.IDC = divider%2;  //special bit needs to be set if the divisor is odd


}


void cs_clock_enable_gclk(uint8_t clk_id){
    GCLK->GENCTRL[clk_id].bit.GENEN = 1; //enable the clock

    while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(clk_id));
}

void cs_clock_disable_gclk(uint8_t clk_id){
    GCLK->GENCTRL[clk_id].bit.GENEN = 0; //enable the clock

}

void cs_clock_enable_pll(uint8_t pll_id) {
    OSCCTRL->Dpll[pll_id].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;

    //wait until the PLL is ready; should be locked and clock-ready (lock will always be one if LBYPASS set)
    while( OSCCTRL->Dpll[pll_id].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[pll_id].DPLLSTATUS.bit.LOCK == 0 );
 }

void cs_clock_config_pll(uint8_t pll_id, uint32_t rate) {
    //PLL0 set based on defined F_CPU value
    uint32_t ldr_ratio = (rate / XOSC32_FREQ) - 1;
    uint32_t ldr_frac = 32 * (rate - (XOSC32_FREQ * (ldr_ratio + 1)) ) / XOSC32_FREQ;
    OSCCTRL->Dpll[pll_id].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(ldr_frac) | OSCCTRL_DPLLRATIO_LDR(ldr_ratio);
    
    while(OSCCTRL->Dpll[pll_id].DPLLSYNCBUSY.bit.DPLLRATIO);
    
    //MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51; PLL will not lock on its own!
    OSCCTRL->Dpll[pll_id].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_XOSC32 | OSCCTRL_DPLLCTRLB_LBYPASS; //use 32kHz ext osc

    //extra options? fast wakeup?


}
void cs_clock_disable_pll(uint8_t pll_id) {
    OSCCTRL->Dpll[pll_id].DPLLCTRLA.reg &= ~OSCCTRL_DPLLCTRLA_ENABLE;
}


void cs_clock_init_pll(uint8_t pll_id, uint32_t rate) {

    //reset and wait for the PLL to be ready for register-writes
    OSCCTRL->Dpll[pll_id].DPLLCTRLA.reg = 0;
    while (OSCCTRL->Dpll[pll_id].DPLLSYNCBUSY.bit.ENABLE);

    cs_clock_config_pll(pll_id, rate);

    cs_clock_enable_pll(pll_id);

}

void cs_clock_init_dfll() {

}

void cs_clock_enable_dfll() {
    
}

void cs_clock_disable_dfll() {

}

/**
 * Note; this isn't considering any clocks that may need to continue running in some form of standby. Saves settings
 */
void cs_clock_save_settings(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clk_settings) {


    //save state of PCLKs
    //gclk side
    for (int i = 0; i<PCLK_CHANNELS; i++) {
        periph_clk_settings->ctrl_reg[i] = GCLK->PCHCTRL[i].reg & 0xFF; //only last 8 bits do anything
        // GCLK->PCHCTRL[i].reg &= ~GCLK_PCHCTRL_CHEN;
    }
    //main side; bus bit masks
    periph_clk_settings->bus_bitmask[0] = MCLK->AHBMASK.reg;
    // MCLK->AHBMASK.reg = 0;
    periph_clk_settings->bus_bitmask[1] = MCLK->APBAMASK.reg;
    // MCLK->APBAMASK.reg = 0;
    periph_clk_settings->bus_bitmask[2] = MCLK->APBBMASK.reg;
    // MCLK->APBBMASK.reg = 0;
    periph_clk_settings->bus_bitmask[3] = MCLK->APBCMASK.reg;
    // MCLK->APBCMASK.reg = 0;
    periph_clk_settings->bus_bitmask[4] = MCLK->APBDMASK.reg;
    // MCLK->APBDMASK.reg = 0;


    //Save state of GCLKs; disable (besides 0)
    gclk_settings->ctrl_reg[0] = GCLK->GENCTRL[0].reg; 

    for (int i = 1; i<GCLKS; i++) {
        gclk_settings->ctrl_reg[i] = GCLK->GENCTRL[i].reg; 
        // GCLK->GENCTRL[i].reg &= ~GCLK_GENCTRL_GENEN; //turn off generic clock
    }
    
    //MCLK

    //set MCLK/GCLK 0 to XTAL

    //save settings and disable PLLs
    pll_settings->pll0_ctrla_reg = OSCCTRL->Dpll[0].DPLLCTRLA.reg;
    pll_settings->pll0_ctrlb_reg = OSCCTRL->Dpll[0].DPLLCTRLB.reg;
    pll_settings->pll0_ratio_reg = OSCCTRL->Dpll[0].DPLLRATIO.reg;
    pll_settings->pll1_ctrla_reg = OSCCTRL->Dpll[1].DPLLCTRLA.reg;
    pll_settings->pll1_ctrlb_reg = OSCCTRL->Dpll[1].DPLLCTRLB.reg;
    pll_settings->pll1_ratio_reg = OSCCTRL->Dpll[1].DPLLRATIO.reg;


    // OSCCTRL->Dpll[0].DPLLCTRLA.reg &= OSCCTRL_DPLLCTRLA_ENABLE; //disable the PLL
    // OSCCTRL->Dpll[1].DPLLCTRLA.reg &= OSCCTRL_DPLLCTRLA_ENABLE; //disable the PLL

}


void cs_clock_restore_settings(pll_config_t* pll_settings,  gclk_config_t* gclk_settings, periph_clk_config_t* periph_clk_settings) {

    //inverse of saving. 

    //save info about peripheral clocks
    for (int i = 0; i<PCLK_CHANNELS; i++) {
        GCLK->PCHCTRL[i].reg = periph_clk_settings->ctrl_reg[i];
    }

    //bus bit masks for AHB, APB
    MCLK->AHBMASK.reg = periph_clk_settings->bus_bitmask[0];
    MCLK->APBAMASK.reg = periph_clk_settings->bus_bitmask[1];
    MCLK->APBBMASK.reg = periph_clk_settings->bus_bitmask[2];
    MCLK->APBCMASK.reg = periph_clk_settings->bus_bitmask[3];
    MCLK->APBDMASK.reg = periph_clk_settings->bus_bitmask[4];


    //Save state of GCLKs; disable (besides 0)
    gclk_settings->ctrl_reg[0] = GCLK->GENCTRL[0].reg; 
    GCLK->GENCTRL[0].reg = gclk_settings->ctrl_reg[0];

    for (int i = 1; i<GCLKS; i++) {
        GCLK->GENCTRL[i].reg = gclk_settings->ctrl_reg[i];
        // if(GCLK->GENCTRL[i].reg & GCLK_GENCTRL_GENEN) {
        //     cs_clock_enable_gclk(i);
        // }
    }

    //restore settings and disable PLLs
    OSCCTRL->Dpll[0].DPLLCTRLA.reg = pll_settings->pll0_ctrla_reg;
    OSCCTRL->Dpll[0].DPLLCTRLB.reg =  pll_settings->pll0_ctrlb_reg;
    OSCCTRL->Dpll[0].DPLLRATIO.reg = pll_settings->pll0_ratio_reg;

    OSCCTRL->Dpll[1].DPLLCTRLA.reg = pll_settings->pll1_ctrla_reg;
    OSCCTRL->Dpll[1].DPLLCTRLB.reg =  pll_settings->pll1_ctrlb_reg;
    OSCCTRL->Dpll[1].DPLLRATIO.reg = pll_settings->pll1_ratio_reg;
    

}
