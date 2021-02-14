#pragma once

#include <compiler.h>
#include <utils_assert.h>

//TODO: add CRTIICAL_SECTION_ENTER/EXIT for these!  Interrupts may mess things up...

#define XOSC32_FREQ 32768

#define F_GCLK4 12000000 //peripheral clock, primarily for serial buses. downsampled from PLL or generated from a different PLL
#define F_GCLK3 XOSC32_FREQ   //slow clock; driven by the external crystal oscillator


#define CLOCK_DIVIDER_UNITY 0 //'1' is assumed when integer divider is allowed; '0' then means no division

/**
 *  Generic clocks are the basis for all peripherals in the processor, including the clock itself. Everything flows thorugh a GCLK. Note that GCLK 0 is used for the 'main' clock that runs the CPU.
 * 
 * 
 * GCLKs have a source oscillator, and may be divided by some integer to decrease the clock speed. 
 * 
 * For more careful configuration of the GCLK's, look at the datasheet and set the control-register's values uniquely for that configuratin. 
 */
void cs_clock_init_gclk(uint8_t clk_id, uint8_t src, uint8_t divider);
void cs_clock_config_gclk(uint8_t clk_id, uint8_t src, uint8_t divider);
void cs_clock_enable_gclk(uint8_t clk_id);
void cs_clock_disable_gclk(uint8_t clk_id);

static inline void cs_clock_config_gclk_ctrlreg(uint8_t clk_id, uint32_t reg_value) {
    GCLK->GENCTRL[clk_id].reg = reg_value;
}


/** 
 *  Each peripheral must have a clock provided before use. It is as simple as specifying which generic clock source to use.
 * 
 *  Note that many peripherals require an APB or AHB clock to be enabled to allow the CPU to write registers within th peripheral. See section 15 of the datasheet for this; it is simply a bit for a given peripheral that needs to be set.
 *   Example: To enable SERCOM channel 4, use command MCLK->APBDMASK.bit.SERCOM4_ = 1;
 */ 
static inline void cs_clock_enable_periph_clk(uint8_t periph_id) {
        GCLK->PCHCTRL[periph_id].reg |= GCLK_PCHCTRL_CHEN; 
}

static inline void cs_clock_disable_periph_clk(uint8_t periph_id) {
        GCLK->PCHCTRL[periph_id].reg &= ~GCLK_PCHCTRL_CHEN; 
}

static inline void cs_clock_init_periph_clk(uint8_t periph_id, uint8_t src_gclk_id) {
    cs_clock_disable_periph_clk(periph_id);
    GCLK->PCHCTRL[periph_id].reg = src_gclk_id;
    cs_clock_enable_periph_clk(periph_id);
}

/**
 * 
 * 2 PLLs present; pll_id in [0,1]
 * rate in Hz; assumed that the source is at 32.768kHz
 *
 * PLL 0 is assumed to be the source for the CPU clock at the max rate of 120 MHz
 */

void cs_clock_init_pll(uint8_t pll_id, uint32_t rate);
void cs_clock_enable_pll(uint8_t pll_id);
void cs_clock_config_pll(uint8_t pll_id, uint32_t rate);
void cs_clock_disable_pll(uint8_t pll_id);

/**
 * 
 * 
 * */
void cs_clock_init_dfll();
void cs_clock_enable_dfll();
void cs_clock_disable_dfll();


/**
 * When entering sleep mode, one of the most important things is to save the existng clock state such that it can be restored when exiting the low power mode. Leaving clocks on will consume power, and that power quadratically scales with the frequency of the clock!
 * 
 * The state of the following should be saved and restored: the oscillators (save 32kHz XTAL), the generic clocks, the main clock and AHB/APB bitmasks, and the peripheral clocks.
 * 
 *   This can optionally be saved in Flash, which may be useful if you want to totally power down s.t. the processor views its wakeup as a 'reset' (a consequence of extremely low power modes w/o RAM retention)
 * 
 *   This can also be further complicated by specific peripherals that are meant to be left on with the corresponding GCLK. That is a special case for now.
 * */

typedef struct pll_config {
    uint32_t pll0_ratio_reg;
    uint32_t pll0_ctrlb_reg;
    uint8_t pll0_ctrla_reg;
    uint32_t pll1_ratio_reg;
    uint32_t pll1_ctrlb_reg;
    uint8_t pll1_ctrla_reg;
} pll_config_t;

#define GCLKS 12

typedef struct gclk_config {
    uint32_t ctrl_reg[GCLKS]; //12 GCLKs; 0 has special purpose
} gclk_config_t;

#define PCLK_CHANNELS 48
#define ABUS_MASKS 5

typedef struct periph_clk_config {
    uint8_t ctrl_reg[PCLK_CHANNELS]; //48 peripheral clocks in ATSAMd51
    uint32_t bus_bitmask[ABUS_MASKS]; //5 registers store info about what is enabled/disabled for AHB, APB
} periph_clk_config_t;

void cs_clock_save_settings(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clk_settings) ;

void cs_clock_restore_settings(pll_config_t* pll_settings,  gclk_config_t* gclk_settings, periph_clk_config_t* periph_clk_settings) ;

