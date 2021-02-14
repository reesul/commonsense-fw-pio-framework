#include <compiler.h>

#include "cs_clock.h"

#define SLEEP_MODE_IDLE 2
#define SLEEP_MODE_STANDBY 4
#define SLEEP_MODE_HIBERNATE 5
#define SLEEP_MODE_BACKUP 6
#define SLEEP_MODE_OFF 7

//basic sleep mode. Assume going down to standby with all RAM retained
void cs_sleep(uint8_t mode);

void cs_sleep_prepare(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clock_settings)  ;

void cs_sleep_restore(pll_config_t* pll_settings, gclk_config_t* gclk_settings, periph_clk_config_t* periph_clock_settings)  ;

void cs_sleep_set_settings(uint8_t mode);

void cs_sleep_reset_settings();