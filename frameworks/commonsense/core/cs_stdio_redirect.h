
#pragma once

#include <stdio.h>
#include <hal_io.h>
// #include <commonsense.h>
// #include <cs_debug_logger.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 *  \brief Initialize STDIO access
 *  \param[in] io Pointer to IO descriptor,
 *                NULL to discard R/W without any error.
 */
void stdio_io_init(struct io_descriptor *io);

/**
 *  \brief Change IO descriptor for terminal to R/W data
 *  \param[in] io Pointer to IO descriptor,
 *                NULL to discard R/W without any error.
 */
void stdio_io_set_io(struct io_descriptor *io);

/**
 *  \brief Read through specified terminal
 *  \param[out] buf Pointer to buffer to place read data
 *  \param[in] len Data length in number of bytes
 *  \return status
 *  \retval >=0 number of bytes read
 *  \retval <0 error
 */
int32_t stdio_io_read(uint8_t *buf, const int32_t len);

/**
 *  \brief Write through specified terminal
 *  \param[in] buf Pointer to buffer to place data to write
 *  \param[in] len Data length in number of bytes
 *  \return status
 *  \retval >=0 number of bytes read
 *  \retval <0 error
 */
int32_t stdio_io_write(const uint8_t *buf, const int32_t len);

int __attribute__((weak)) _read(int file, char *ptr, int len); /* Remove GCC compiler warning */

int __attribute__((weak)) _write(int file, char *ptr, int len); /* Remove GCC compiler warning */


#ifdef __cplusplus
}
#endif /* __cplusplus */