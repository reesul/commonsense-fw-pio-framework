#include "cs_sleep.h"


// assume all configuration and saving is ready, and we just need to go into the sleep state. This will exit when an interrupt is received
void _cs_sleep_state() {

    __DSB();    //force processor to finish data-related instructions
    __ISB();    //force processor to finish control-related instructions
    __WFI();    //wait until an interrupt occurs

}

void _cs_sleep_idle_config(uint8_t mode) {

}

void _cs_sleep_standby_config() {
    PM->STDBYCFG.reg = PM_STDBYCFG_RAMCFG(0) | PM_STDBYCFG_FASTWKUP(2); //do fast wakeup for the main voltage regulator (opts 2,3) 
}

void _cs_sleep_hibernate_config(){

    PM->HIBCFG.reg = PM_HIBCFG_BRAMCFG(3) | PM_HIBCFG_RAMCFG(3); // maintain backup and system RAM

}
void  _cs_sleep_backup_config() {
    PM->BKUPCFG.reg = PM_BKUPCFG_BRAMCFG(0);
}

void  _cs_sleep_shutdown_config() {
    // for(;;){;}
}

void cs_sleep_set_settings(uint8_t mode) {
    switch (mode) {
        case 0:
        case 1:
        case 2:
            _cs_sleep_idle_config(mode);
            break;
        case 4:
            _cs_sleep_standby_config();
            break;
        case 5:
            _cs_sleep_hibernate_config();
            break;
        case 6:
            _cs_sleep_backup_config();
            break;
        case 7:
            _cs_sleep_shutdown_config();
            break;
        default:
            //are you sure you want to sleep? What mode?
            return;
    }

    PM->SLEEPCFG.bit.SLEEPMODE = mode;
    //takes a few cycles for the bits to be written
    while(PM->SLEEPCFG.bit.SLEEPMODE != mode);
}

void cs_sleep_reset_settings(){
    PM->BKUPCFG.reg = 0;
    PM->HIBCFG.reg = 0;
    PM->STDBYCFG.reg = 0;
}

void cs_sleep_prepare(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clock_settings) {


    cs_clock_save_settings(pll_settings, gclk_settings, periph_clock_settings);

    // cs_clock_disable_gclk(0);
    // cs_clock_init_gclk(1, GCLK_GENCTRL_SRC_XOSC32K_Val, 0);

    // GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K_Val) |
    //   GCLK_GENCTRL_IDC |
    //   //GCLK_GENCTRL_OE |
    //   GCLK_GENCTRL_GENEN;

    cs_clock_init_gclk(0, GCLK_GENCTRL_SRC_XOSC32K_Val, 0); //switch the main clock to use the 32kHz xtal
    while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0 );



    cs_clock_disable_pll(0);
    // cs_clock_disable_pll(1); //not in use at the time of this writing 

    //set regulator to BUCK mode
    SUPC->VREG.bit.SEL = SUPC_VREG_SEL_BUCK; //Using BUCK will cause issues w/ PLL stabilization, but saves power in sleep

}

void cs_sleep_restore(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clock_settings) {

    SUPC->VREG.bit.SEL = SUPC_VREG_SEL_LDO; //Switch back to LDO mode; BUCK is too unstable for the PLL to lock

    // cs_clock_restore_settings(&pll_settings, &gclk_settings, &periph_clock_settings);
    // cs_clock_enable_pll(0);
    cs_clock_init_pll(0, F_CPU);

    cs_clock_config_gclk(0, GCLK_GENCTRL_SRC_DPLL0, 0); //switch the main clock back to using the fast PLL


}

void cs_sleep(uint8_t mode) {

    //configure CS for the particular sleep mode we'll be using
    cs_sleep_set_settings(mode);

    pll_config_t pll_settings;
    gclk_config_t gclk_settings;
    periph_clk_config_t periph_clock_settings;

    cs_sleep_prepare(&pll_settings, &gclk_settings, &periph_clock_settings);

    _cs_sleep_state();

    cs_sleep_restore(&pll_settings, &gclk_settings, &periph_clock_settings);

}