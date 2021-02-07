#pragma once

#include <compiler.h>

#define F_GCLK4 12000000
#define F_GCLK3 32768

void cs_init_gclk(uint8_t clk_id, uint8_t src, uint8_t divider);
void cs_config_gclk(uint8_t clk_id, uint8_t src, uint8_t divider);
void cs_enable_gclk(uint8_t clk_id);
void cs_disable_gclk(uint8_t clk_id);

static inline void cs_config_gclk_ctrlreg(uint8_t clk_id, uint32_t reg_value) {
    GCLK->GENCTRL[clk_id].reg = reg_value;
}
