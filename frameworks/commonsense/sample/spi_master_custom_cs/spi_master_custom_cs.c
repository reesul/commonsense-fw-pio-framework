/**
 * \file
 *
 * \brief SPI Master with custom chip select implementation.
 *
 * Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "spi_master_custom_cs.h"
#include "atmel_start.h"
#include "spi_master_custom_cs_config.h"

struct io_descriptor *io;
bool                  slave_enable = (bool)SPI_SLAVE_TYPE;

/**
 * \brief Deselect all connected slaves
 */
void spi_deselect_all_slaves(void)
{
	gpio_set_pin_level(CS_0_PIN, !slave_enable);
#ifdef CS_1_PIN
	gpio_set_pin_level(CS_1_PIN, !slave_enable);
#endif
#ifdef CS_2_PIN
	gpio_set_pin_level(CS_2_PIN, !slave_enable);
#endif
#ifdef CS_3_PIN
	gpio_set_pin_level(CS_3_PIN, !slave_enable);
#endif
#ifdef CS_4_PIN
	gpio_set_pin_level(CS_4_PIN, !slave_enable);
#endif
#ifdef CS_5_PIN
	gpio_set_pin_level(CS_5_PIN, !slave_enable);
#endif
}

/**
 * \brief Initialize SPI master interface.
 */
void spi_master_init(void)
{
	spi_m_sync_get_io_descriptor(&SPI_MASTER, &io);
	spi_m_sync_enable(&SPI_MASTER);
}

/**
 * \brief Deinitialize SPI master interface.
 */
void spi_master_deinit(void)
{
	spi_deselect_all_slaves();
	spi_m_sync_disable(&SPI_MASTER);
}

/**
 * \brief Enable slave.
 */
void spi_slave_enable(uint8_t channel_num)
{
	switch (channel_num) {
	case 0:
		gpio_set_pin_level(CS_0_PIN, slave_enable);
		break;
#ifdef CS_1_PIN
	case 1:
		gpio_set_pin_level(CS_1_PIN, slave_enable);
		break;
#endif
#ifdef CS_2_PIN
	case 2:
		gpio_set_pin_level(CS_2_PIN, slave_enable);
		break;
#endif
#ifdef CS_3_PIN
	case 3:
		gpio_set_pin_level(CS_3_PIN, slave_enable);
		break;
#endif
#ifdef CS_4_PIN
	case 4:
		gpio_set_pin_level(CS_4_PIN, slave_enable);
		break;
#endif
#ifdef CS_5_PIN
	case 5:
		gpio_set_pin_level(CS_5_PIN, slave_enable);
		break;
#endif
	default:
		break;
	}
#if SPI_CS_DELAY_EN == 1
	delay_us(SPI_CS_DELAY);
#endif
}

/**
 * \brief Disable slave.
 */
void spi_slave_disable(uint8_t channel_num)
{
	switch (channel_num) {
	case 0:
		gpio_set_pin_level(CS_0_PIN, !slave_enable);
		break;
#ifdef CS_1_PIN
	case 1:
		gpio_set_pin_level(CS_1_PIN, !slave_enable);
		break;
#endif
#ifdef CS_2_PIN
	case 2:
		gpio_set_pin_level(CS_2_PIN, !slave_enable);
		break;
#endif
#ifdef CS_3_PIN
	case 3:
		gpio_set_pin_level(CS_3_PIN, !slave_enable);
		break;
#endif
#ifdef CS_4_PIN
	case 4:
		gpio_set_pin_level(CS_4_PIN, !slave_enable);
		break;
#endif
#ifdef CS_5_PIN
	case 5:
		gpio_set_pin_level(CS_5_PIN, !slave_enable);
		break;
#endif
	default:
		break;
	}
#if SPI_CS_DELAY_EN == 1
	delay_us(SPI_CS_DELAY);
#endif
}

/**
 *  \brief send data over SPI interface
 */
void spi_send_data(uint8_t channel_num, uint8_t *data, uint16_t length)
{
	spi_slave_enable(channel_num);
	io_write(io, data, length);
	spi_slave_disable(channel_num);
}

/**
 *  \brief read data over SPI interface
 */
void spi_read_data(uint8_t channel_num, uint8_t *data, uint16_t length)
{
	spi_slave_enable(channel_num);
	io_read(io, data, length);
	spi_slave_disable(channel_num);
}

/**
 *  \brief read write data over SPI interface
 */
void spi_write_read(uint8_t channel_num, uint8_t *txbuff, uint8_t *rxbuff, uint16_t txlen, uint16_t rxlen)
{
	spi_slave_enable(channel_num);
	if (txbuff != NULL && txlen > 0) {
		io_write(io, txbuff, txlen);
	}
	if (rxbuff != NULL && rxlen > 0) {
		io_read(io, rxbuff, rxlen);
	}
	spi_slave_disable(channel_num);
}

/**
 *  \brief send data over SPI interface without slave select control
 */
void spi_write(uint8_t *data, uint16_t length)
{
	io_write(io, data, length);
}

/**
 *  \brief read data over SPI interface without slave select control
 */
void spi_read(uint8_t *data, uint16_t length)
{
	io_read(io, data, length);
}
