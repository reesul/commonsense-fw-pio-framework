/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "spi_master_custom_cs_start.h"
#include "spi_master_custom_cs.h"

static uint8_t data[12] = "Hello World!";

/**
 * \brief Initialize SPI master custom chip select
 */
void spi_master_custom_cs_init(void)
{
	spi_deselect_all_slaves();
	spi_master_init();
}

/**
 * \brief SPI Master with custom chip select example
 */
void spi_master_custom_cs_example(void)
{
	spi_send_data(0, data, sizeof(data));
#ifdef CS_1_PIN
	spi_send_data(1, data, sizeof(data));
#endif
}
