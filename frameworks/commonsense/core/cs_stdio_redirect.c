#include "cs_stdio_redirect.h"


/** IO descriptor for STDIO access. */
static struct io_descriptor *stdio_io = NULL;

void stdio_io_init(struct io_descriptor *io)
{
#if defined(__GNUC__)
	/* Specify that stdout and stdin should not be buffered. */
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	/* Note: Already the case in IAR's Normal DLIB default configuration
	 * and AVR GCC library:
	 * - printf() emits one character at a time.
	 * - getchar() requests only 1 byte to exit.
	 */
#endif
	stdio_io = io;
}

void stdio_io_set_io(struct io_descriptor *io)
{
	stdio_io = io;
}

int32_t stdio_io_read(uint8_t *buf, const int32_t len)
{
	if (stdio_io == NULL) {
		return 0;
	}
	return io_read(stdio_io, buf, len);
}

int32_t stdio_io_write(const uint8_t *buf, const int32_t len)
{
	if (stdio_io == NULL) {
		return 0;
	}
	return io_write(stdio_io, buf, len);
}

int __attribute__((weak)) _read(int file, char *ptr, int len)
{
	int n = 0;

	if (file != 0) {
		return -1;
	}

	n = stdio_io_read((uint8_t *)ptr, len);
	if (n < 0) {
		return -1;
	}

	return n;
}



int __attribute__((weak)) _write(int file, char *ptr, int len)
{
	int n = 0;

	if ((file != 1) && (file != 2) && (file != 3)) {
		return -1;
	}

	n = stdio_io_write((const uint8_t *)ptr, len);
	if (n < 0) {
		return -1;
	}

	return n;
}