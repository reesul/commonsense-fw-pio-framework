/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  SAMD51 support added by Adafruit - Copyright (c) 2018 Dean Miller for Adafruit Industries

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <sam.h>

#include <stdio.h>

#include "cs_clock.h"

// Constants for Clock generators
#define GENERIC_CLOCK_GENERATOR_MAIN      (0u)

#define GENERIC_CLOCK_GENERATOR_XOSC32K   (3u)
#define GENERIC_CLOCK_GENERATOR_48M		  (1u)
#define GENERIC_CLOCK_GENERATOR_48M_SYNC	GCLK_SYNCBUSY_GENCTRL1
#define GENERIC_CLOCK_GENERATOR_100M	  (2u)
#define GENERIC_CLOCK_GENERATOR_100M_SYNC	GCLK_SYNCBUSY_GENCTRL2
#define GENERIC_CLOCK_GENERATOR_12M       (4u)
#define GENERIC_CLOCK_GENERATOR_12M_SYNC   GCLK_SYNCBUSY_GENCTRL4

//USE DPLL0 for 120MHZ
#define MAIN_CLOCK_SOURCE				  GCLK_GENCTRL_SRC_DPLL0

#define GENERIC_CLOCK_GENERATOR_1M		  (5u)
//#define CRYSTALLESS


#define GENERIC_CLOCK_GENERATOR_OSC32K    (1u)
#define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)
// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)


void SystemInit( void )
{

  /* Use the LDO regulator by default */
  SUPC->VREG.bit.SEL = 0; //Using BUCK will cause issues w/ PLL stabilization
  
  NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);
  
  /* ----------------------------------------------------------------------------------------------
   * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
   */
  
  OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_ENABLE | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_CGM_XT | OSC32KCTRL_XOSC32K_XTALEN;
  
  while( (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY) == 0 );

  //software reset
  GCLK->CTRLA.bit.SWRST = 1;
  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST );
 
  /* ----------------------------------------------------------------------------------------------
   * 2) Put XOSC32K as source of Generic Clock Generator 3
   */
  // GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_XOSC32K].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K) | //generic clock gen 3
  //   GCLK_GENCTRL_GENEN;
 

  // while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL3 );
  cs_clock_init_gclk(3, GCLK_GENCTRL_SRC_XOSC32K, CLOCK_DIVIDER_UNITY); //

	
  /* ------------------------------------------------------------------------
  * 3. Set up the PLL
  */
  // OSCCTRL->Dpll[0].DPLLCTRLA.reg = 0;
	// while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.ENABLE);

  // //PLL0 set based on defined F_CPU value
  // uint32_t ldr_ratio = (F_CPU / XOSC32_FREQ) - 1;
  // uint32_t ldr_frac = 32 * (F_CPU - (XOSC32_FREQ * (ldr_ratio + 1)) ) / XOSC32_FREQ;
  // OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(ldr_frac) | OSCCTRL_DPLLRATIO_LDR(ldr_ratio);
  
  // while(OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);
  
  // //MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51
  // // OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
  // OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_XOSC32 | OSCCTRL_DPLLCTRLB_LBYPASS; //use 32kHz ext osc

  // OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;

  // while( OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0 );
  cs_clock_init_pll(0, F_CPU);

  /*---------------------------------------------------------------------
   * 4. Set up peripheral clock generators
   */
   cs_clock_init_gclk(4, GCLK_GENCTRL_SRC_DPLL0, F_CPU/F_GCLK4);


  /*---------------------------------------------------------------------
   * 5. Set up main clock
   */
  
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_MAIN].reg = GCLK_GENCTRL_SRC(MAIN_CLOCK_SOURCE) |
    GCLK_GENCTRL_IDC |
    //GCLK_GENCTRL_OE |
    GCLK_GENCTRL_GENEN;
  
  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0 );
  
  MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;

  /*---------------------------------------------------------------------
   * 6. Disable the DFLL to save power
   */
  
  // OSCCTRL->DFLLCTRLA.reg = 0;
  
  /* If desired, enable cache! */
#if defined(ENABLE_CACHE)
  __disable_irq();
  CMCC->CTRL.reg = 1;
  __enable_irq();
#endif

  /*---------------------------------------------------------------------
   * Start up the "Debug Watchpoint and Trace" unit, so that we can use
   * it's 32bit cycle counter for timing.
   */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


}

void SystemInit_deprecated( void )
{

  /* Use the LDO regulator by default */
  SUPC->VREG.bit.SEL = 0; //Using BUCK will cause issues w/ PLL stabilization
  
 
  NVMCTRL->CTRLA.reg |= NVMCTRL_CTRLA_RWS(0);
  
  /* ----------------------------------------------------------------------------------------------
   * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
   */
  
  OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_ENABLE | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_CGM_XT | OSC32KCTRL_XOSC32K_XTALEN;
  
  while( (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY) == 0 );

  //software reset
  GCLK->CTRLA.bit.SWRST = 1;
  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST );
 
  /* ----------------------------------------------------------------------------------------------
   * 2) Put XOSC32K as source of Generic Clock Generator 3
   */
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_XOSC32K].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K) | //generic clock gen 3
    GCLK_GENCTRL_GENEN;
 

  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL3 );

  /* ----------------------------------------------------------------------------------------------
   * 3) Put OSCULP32K as source for Generic Clock Generator 0
   */
  // GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSCULP32K) | GCLK_GENCTRL_GENEN;
  
  /* ----------------------------------------------------------------------------------------------
   * 4) Enable DFLL48M clock
   */
  
  // while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0 );

  // DFLL Configuration in Open Loop mode 
  
  // OSCCTRL->DFLLCTRLA.reg = 0;
  // //GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK3_Val);
  
  // OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP( 0x1 ) |
  //   OSCCTRL_DFLLMUL_FSTEP( 0x1 ) |
  //   OSCCTRL_DFLLMUL_MUL( 0 );
  
  // while ( OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLMUL );
  
  // OSCCTRL->DFLLCTRLB.reg = 0;
  // while ( OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_DFLLCTRLB );
  
  // OSCCTRL->DFLLCTRLA.reg |= OSCCTRL_DFLLCTRLA_ENABLE;
  // while ( OSCCTRL->DFLLSYNC.reg & OSCCTRL_DFLLSYNC_ENABLE );
  
  // OSCCTRL->DFLLVAL.reg = OSCCTRL->DFLLVAL.reg;
  // while( OSCCTRL->DFLLSYNC.bit.DFLLVAL );
  
  // OSCCTRL->DFLLCTRLB.reg = OSCCTRL_DFLLCTRLB_WAITLOCK |
  // OSCCTRL_DFLLCTRLB_CCDIS | OSCCTRL_DFLLCTRLB_USBCRM ;
  
  // while ( !OSCCTRL->STATUS.bit.DFLLRDY );
  
  // GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_1M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(48u);
  
  // while ( GCLK->SYNCBUSY.bit.GENCTRL5 );
  
	
  /* ------------------------------------------------------------------------
  * Set up the PLLs
  */
  OSCCTRL->Dpll[0].DPLLCTRLA.reg = 0;
	while (OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.ENABLE);

  /* original code for setting to DFLL output at 1MHz
  // GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK5_Val);
  
  // This rounds to nearest full-MHz increment; not currently using frac. Assumes 1 MHz reference
  // OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR((F_CPU - 500000) / 1000000);
  */

  //PLL0 set based on defined F_CPU value
  uint32_t ldr_ratio = (F_CPU / XOSC32_FREQ) - 1;
  uint32_t ldr_frac = 32 * (F_CPU - (XOSC32_FREQ * (ldr_ratio + 1)) ) / XOSC32_FREQ;
  OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(ldr_frac) | OSCCTRL_DPLLRATIO_LDR(ldr_ratio);
  
  while(OSCCTRL->Dpll[0].DPLLSYNCBUSY.bit.DPLLRATIO);
  
  //MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51
  // OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
  OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_XOSC32 | OSCCTRL_DPLLCTRLB_LBYPASS; //use 32kHz ext osc

  OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;

  while( OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK == 0 );

  /*
  //PLL1 is 100MHz
  GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL1].reg = (1 << GCLK_PCHCTRL_CHEN_Pos) | GCLK_PCHCTRL_GEN(GCLK_PCHCTRL_GEN_GCLK5_Val);
  
  OSCCTRL->Dpll[1].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0x00) | OSCCTRL_DPLLRATIO_LDR(99); //100 Mhz
  
  while(OSCCTRL->Dpll[1].DPLLSYNCBUSY.bit.DPLLRATIO);
  
  //MUST USE LBYPASS DUE TO BUG IN REV A OF SAMD51
  OSCCTRL->Dpll[1].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK_GCLK | OSCCTRL_DPLLCTRLB_LBYPASS;
  
  OSCCTRL->Dpll[1].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;
  
  while( OSCCTRL->Dpll[1].DPLLSTATUS.bit.CLKRDY == 0 || OSCCTRL->Dpll[1].DPLLSTATUS.bit.LOCK == 0 );
  */
  
  /* ------------------------------------------------------------------------
  * Set up the peripheral clocks
  */
  
  /*
  //48MHZ CLOCK FOR USB AND STUFF
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_48M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) |
    GCLK_GENCTRL_IDC |
    //GCLK_GENCTRL_OE |
    GCLK_GENCTRL_GENEN;
  
  while ( GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_48M_SYNC);
  
  //100MHZ CLOCK FOR OTHER PERIPHERALS
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_100M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DPLL1_Val) |
    GCLK_GENCTRL_IDC |
    //GCLK_GENCTRL_OE |
    GCLK_GENCTRL_GENEN;
  
  while ( GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_100M_SYNC);
  
  //12MHZ CLOCK FOR DAC
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_12M].reg = GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_DFLL_Val) |
    GCLK_GENCTRL_IDC |
    GCLK_GENCTRL_DIV(4) |
    //GCLK_GENCTRL_DIVSEL |
    //GCLK_GENCTRL_OE |
    GCLK_GENCTRL_GENEN;
  
  while ( GCLK->SYNCBUSY.reg & GENERIC_CLOCK_GENERATOR_12M_SYNC);

  */
  
  /*---------------------------------------------------------------------
   * Set up main clock
   */
  
  GCLK->GENCTRL[GENERIC_CLOCK_GENERATOR_MAIN].reg = GCLK_GENCTRL_SRC(MAIN_CLOCK_SOURCE) |
    GCLK_GENCTRL_IDC |
    //GCLK_GENCTRL_OE |
    GCLK_GENCTRL_GENEN;
  

  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0 );
  
  MCLK->CPUDIV.reg = MCLK_CPUDIV_DIV_DIV1;
  
  
  /* If desired, enable cache! */
#if defined(ENABLE_CACHE)
  __disable_irq();
  CMCC->CTRL.reg = 1;
  __enable_irq();
#endif

  /*---------------------------------------------------------------------
   * Start up the "Debug Watchpoint and Trace" unit, so that we can use
   * it's 32bit cycle counter for timing.
   */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


}
