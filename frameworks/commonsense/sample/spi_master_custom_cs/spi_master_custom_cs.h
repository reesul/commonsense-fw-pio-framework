/**
 * \file
 *
 * \brief SPI Master with custom chip select implementation.
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SPI_MASTER_CUSTOM_CS_H_INCLUDED
#define _SPI_MASTER_CUSTOM_CS_H_INCLUDED

#include "atmel_start.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Deselect all connected slaves
 */
void spi_deselect_all_slaves(void);

/**
 * \brief Initialize SPI master interface.
 */
void spi_master_init(void);

/**
 * \brief Deinitialize SPI master interface.
 */
void spi_master_deinit(void);

/**
 * \brief Enable slave.
 */
void spi_slave_enable(uint8_t channel_num);

/**
 * \brief Disable slave.
 */
void spi_slave_disable(uint8_t channel_num);

/** \brief send data over SPI interface with slave select control
 *  \param[in] slave channel number.
 *  \param[in] pointer to the buffer to send.
 *  \param[in] size of the data in number of characters.
 */
void spi_send_data(uint8_t channel_num, uint8_t *data, uint16_t length);

/** \brief read data over SPI interface with slave select control
 *  \param[in] slave channel number.
 *  \param[out] pointer to the buffer to hold read data.
 *  \param[in] size of the data in number of characters.
 */
void spi_read_data(uint8_t channel_num, uint8_t *data, uint16_t length);

/** \brief read write data over SPI interface
 *  \param[in] slave channel number.
 *  \param[in] pointer to the buffer to hold write data.
 *  \param[out] pointer to the buffer to hold read data.
 *  \param[in] size of the data in number of characters to write.
 *  \param[in] size of the data in number of characters to read.
 */
void spi_write_read(uint8_t channel_num, uint8_t *txbuff, uint8_t *rxbuff, uint16_t txlen, uint16_t rxlen);

/**
 *  \brief send data over SPI interface without slave select control
 *  \param[in] pointer to the buffer to send.
 *  \param[in] size of the data in number of characters.
 */
void spi_write(uint8_t *data, uint16_t length);

/**
 *  \brief read data over SPI interface without slave select control
 *  \param[out] pointer to the buffer to hold read data.
 *  \param[in] size of the data in number of characters.
 */
void spi_read(uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif // _SPI_MASTER_CUSTOM_CS_H_INCLUDED
