/* Auto-generated config file spi_master_custom_cs_config.h */
#ifndef SPI_MASTER_CUSTOM_CS_CONFIG_H
#define SPI_MASTER_CUSTOM_CS_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Basic configuration

// <e> Enable delay
// <i> Enable slave select delay
// <id> spi_enable_delay_id
#ifndef SPI_CS_DELAY_EN
#define SPI_CS_DELAY_EN 0x1
#endif

// <o> delay (uS) <0-65535>
// <i> delay after slave select
// <id> spi_delay_id
#ifndef SPI_CS_DELAY
#define SPI_CS_DELAY 500
#endif
// </e>

// <o> SPI Slave type
// <0x00=>Active Low Slaves
// <0x01=>Active High Slaves
// <i> Specify types of slave devices connected
// <id> spi_slave_type
#ifndef SPI_SLAVE_TYPE
#define SPI_SLAVE_TYPE 0x0
#endif
// </h>

// <<< end of configuration section >>>

#endif // SPI_MASTER_CUSTOM_CS_CONFIG_H
