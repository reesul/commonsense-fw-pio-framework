/* Auto-generated config file hpl_tc_config.h */
#ifndef HPL_TC_CONFIG_H
#define HPL_TC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>

#ifndef CONF_TC2_ENABLE
#define CONF_TC2_ENABLE 1
#endif

// <h> Basic settings
// <o> Prescaler
// <0=> No division
// <1=> Divide by 2
// <2=> Divide by 4
// <3=> Divide by 8
// <4=> Divide by 16
// <5=> Divide by 64
// <6=> Divide by 256
// <7=> Divide by 1024
// <i> This defines the prescaler value
// <id> tc_prescaler
#ifndef CONF_TC2_PRESCALER
#define CONF_TC2_PRESCALER 0
#endif
// </h>

// <h> PWM Waveform Output settings
// <o> Waveform Period Value (uS) <0x00-0xFFFFFFFF>
// <i> The unit of this value is us.
// <id> tc_arch_wave_per_val
#ifndef CONF_TC2_WAVE_PER_VAL
#define CONF_TC2_WAVE_PER_VAL 0x3e8
#endif

// <o> Waveform Duty Value (0.1%) <0x00-0x03E8>
// <i> The unit of this value is 1/1000.
// <id> tc_arch_wave_duty_val
#ifndef CONF_TC2_WAVE_DUTY_VAL
#define CONF_TC2_WAVE_DUTY_VAL 0x1f4
#endif

/* Caculate pwm ccx register value based on WAVE_PER_VAL and Waveform Duty Value */
#if CONF_TC2_PRESCALER < TC_CTRLA_PRESCALER_DIV64_Val
#define CONF_TC2_CC0                                                                                                   \
	((uint32_t)(((double)CONF_TC2_WAVE_PER_VAL * CONF_GCLK_TC2_FREQUENCY) / 1000000 / (1 << CONF_TC2_PRESCALER) - 1))
#define CONF_TC2_CC1 ((CONF_TC2_CC0 * CONF_TC2_WAVE_DUTY_VAL) / 1000)

#elif CONF_TC2_PRESCALER == TC_CTRLA_PRESCALER_DIV64_Val
#define CONF_TC2_CC0 ((uint32_t)(((double)CONF_TC2_WAVE_PER_VAL * CONF_GCLK_TC2_FREQUENCY) / 64000000 - 1))
#define CONF_TC2_CC1 ((CONF_TC2_CC0 * CONF_TC2_WAVE_DUTY_VAL) / 1000)

#elif CONF_TC2_PRESCALER == TC_CTRLA_PRESCALER_DIV256_Val
#define CONF_TC2_CC0 ((uint32_t)(((double)CONF_TC2_WAVE_PER_VAL * CONF_GCLK_TC2_FREQUENCY) / 256000000 - 1))
#define CONF_TC2_CC1 ((CONF_TC2_CC0 * CONF_TC2_WAVE_DUTY_VAL) / 1000)

#elif CONF_TC2_PRESCALER == TC_CTRLA_PRESCALER_DIV1024_Val
#define CONF_TC2_CC0 ((uint32_t)(((double)CONF_TC2_WAVE_PER_VAL * CONF_GCLK_TC2_FREQUENCY) / 1024000000 - 1))
#define CONF_TC2_CC1 ((CONF_TC2_CC0 * CONF_TC2_WAVE_DUTY_VAL) / 1000)
#endif

// </h>

// <h> Advanced settings
// <y> Mode
// <TC_CTRLA_MODE_COUNT16_Val"> Counter in 16-bit mode
// <TC_CTRLA_MODE_COUNT32_Val"> Counter in 32-bit mode
// <i> These bits mode
// <id> tc_mode
#ifndef CONF_TC2_MODE
#define CONF_TC2_MODE TC_CTRLA_MODE_COUNT16_Val
#endif

/*  Unused in 16/32 bit PWM mode */
#ifndef CONF_TC2_PER
#define CONF_TC2_PER 0x32
#endif

// <y> Prescaler and Counter Synchronization Selection
// <TC_CTRLA_PRESCSYNC_GCLK_Val"> Reload or reset counter on next GCLK
// <TC_CTRLA_PRESCSYNC_PRESC_Val"> Reload or reset counter on next prescaler clock
// <TC_CTRLA_PRESCSYNC_RESYNC_Val"> Reload or reset counter on next GCLK and reset prescaler counter
// <i> These bits select if on retrigger event, the Counter should be cleared or reloaded on the next GCLK_TCx clock or on the next prescaled GCLK_TCx clock.
// <id> tc_arch_presync
#ifndef CONF_TC2_PRESCSYNC
#define CONF_TC2_PRESCSYNC TC_CTRLA_PRESCSYNC_GCLK_Val
#endif

// <q> Run in standby
// <i> Indicates whether the will continue running in standby sleep mode or not
// <id> tc_arch_runstdby
#ifndef CONF_TC2_RUNSTDBY
#define CONF_TC2_RUNSTDBY 0
#endif

// <q> On-Demand
// <i> Indicates whether the TC2's on-demand mode is on or not
// <id> tc_arch_ondemand
#ifndef CONF_TC2_ONDEMAND
#define CONF_TC2_ONDEMAND 0
#endif

// <o> Auto Lock
// <0x0=>The Lock Update bit is not affected on overflow/underflow and re-trigger event
// <0x1=>The Lock Update bit is set on each overflow/underflow or re-trigger event
// <id> tc_arch_alock
#ifndef CONF_TC2_ALOCK
#define CONF_TC2_ALOCK 0
#endif

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_CAPTEN0 0
//#define CONF_TC2_CAPTEN1 0
//#define CONF_TC2_COPEN0  0
//#define CONF_TC2_COPEN1  0

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_DIR     0
//#define CONF_TC2_ONESHOT 0
//#define CONF_TC2_LUPD    0

// <q> Debug Running Mode
// <i> Indicates whether the Debug Running Mode is enabled or not
// <id> tc_arch_dbgrun
#ifndef CONF_TC2_DBGRUN
#define CONF_TC2_DBGRUN 0
#endif

// <e> Event control
// <id> timer_event_control
#ifndef CONF_TC2_EVENT_CONTROL_ENABLE
#define CONF_TC2_EVENT_CONTROL_ENABLE 0
#endif

// <q> Output Event On Match or Capture on Channel 0
// <i> Enable output of event on timer tick
// <id> tc_arch_mceo0
#ifndef CONF_TC2_MCEO0
#define CONF_TC2_MCEO0 0
#endif

// <q> Output Event On Match or Capture on Channel 1
// <i> Enable output of event on timer tick
// <id> tc_arch_mceo1
#ifndef CONF_TC2_MCEO1
#define CONF_TC2_MCEO1 0
#endif

// <q> Output Event On Timer Tick
// <i> Enable output of event on timer tick
// <id> tc_arch_ovfeo
#ifndef CONF_TC2_OVFEO
#define CONF_TC2_OVFEO 0
#endif

// <q> Event Input
// <i> Enable asynchronous input events
// <id> tc_arch_tcei
#ifndef CONF_TC2_TCEI
#define CONF_TC2_TCEI 0
#endif

// <q> Inverted Event Input
// <i> Invert the asynchronous input events
// <id> tc_arch_tcinv
#ifndef CONF_TC2_TCINV
#define CONF_TC2_TCINV 0
#endif

// <o> Event action
// <0=> Event action disabled
// <1=> Start, restart or re-trigger TC on event
// <2=> Count on event
// <3=> Start on event
// <4=> Time stamp capture
// <5=> Period captured in CC0, pulse width in CC1
// <6=> Period captured in CC1, pulse width in CC0
// <7=> Pulse width capture
// <i> Event which will be performed on an event
//<id> tc_arch_evact
#ifndef CONF_TC2_EVACT
#define CONF_TC2_EVACT 0
#endif
// </e>

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_WAVEGEN   TC_CTRLA_WAVEGEN_MFRQ_Val

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_INVEN0 0
//#define CONF_TC2_INVEN1 0

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_PERBUF 0

/* Commented intentionally. Timer uses fixed value. May be used by other abstractions based on TC. */
//#define CONF_TC2_CCBUF0 0
//#define CONF_TC2_CCBUF1 0

// </h>

// <<< end of configuration section >>>

#endif // HPL_TC_CONFIG_H
