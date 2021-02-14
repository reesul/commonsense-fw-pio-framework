#include "cs_debug_logger.h"

struct usart_sync_descriptor TARGET_IO; //STDIO will redirect all IO through this interface; default to SERCOM4 USRT (Synchronous!)


void _config_debug_usart() {
    const void* hw = SERCOM4;

    // usart_configuration_t cx = SERCOM_CONFIGURATION(4);
	uint32_t mode = SERCOM_USART_CTRLA_MODE(1); //use internal clock


    if (!hri_sercomusart_is_syncing(hw, SERCOM_USART_SYNCBUSY_SWRST)) {

		if (hri_sercomusart_get_CTRLA_reg(hw, SERCOM_USART_CTRLA_ENABLE)) {
			hri_sercomusart_clear_CTRLA_ENABLE_bit(hw);
			hri_sercomusart_wait_for_sync(hw, SERCOM_USART_SYNCBUSY_ENABLE);
		}
		hri_sercomusart_write_CTRLA_reg(hw, SERCOM_USART_CTRLA_SWRST | mode);
	}
	hri_sercomusart_wait_for_sync(hw, SERCOM_USART_SYNCBUSY_SWRST);

	uint32_t ctrla = mode | SERCOM_USART_CTRLA_RXPO(1) | SERCOM_USART_CTRLA_DORD;//enable bit??
	uint32_t ctrlb = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN;
	uint32_t ctrlc = SERCOM_USART_CTRLC_GTIME(0x2) | SERCOM_USART_CTRLC_MAXITER(0x7);


	hri_sercomusart_write_CTRLA_reg(hw, ctrla);
	hri_sercomusart_write_CTRLB_reg(hw, ctrlb);
	hri_sercomusart_write_CTRLC_reg(hw, ctrlc);


	uint16_t baud = 65536 - ((65536 * 16.0f * DEBUG_USART_BAUD) / 12000000);//2^16-3774; equation changes under different SAMPR values
	// uint16_t baud = 65536 - ((65536 * 16.0f * F_GCLK4) / 12000000);
	uint16_t fractional = SERCOM_USART_BAUD_FRAC_BAUD(0);


	if ((ctrla & SERCOM_USART_CTRLA_SAMPR(0x1)) || (ctrla & SERCOM_USART_CTRLA_SAMPR(0x3))) {
		((Sercom *)hw)->USART.BAUD.FRAC.BAUD = baud;
		((Sercom *)hw)->USART.BAUD.FRAC.FP   = fractional;
	} else {
		hri_sercomusart_write_BAUD_reg(hw, baud);
	}

	uint8_t rxpl = 0;
	hri_sercomusart_write_RXPL_reg(hw, rxpl);
	uint8_t dbgctrl = SERCOM_USART_DBGCTRL_DBGSTOP;
	hri_sercomusart_write_DBGCTRL_reg(hw, dbgctrl);

}

void cs_debug_init() {

	cs_debug_usart_init();

}

void cs_debug_usart_init() {//replace with status/configuration return type
    //Assume GCLK 4 at 12MHz!!

    //Configure peripheral clocks PCHCTRL for general clock interface
    //peripheral clock SERCOM4_CORE= 34; SLOW=3. SLOW not needed, for now.
	//TODO: replace with calls to clock library
    GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK4 | GCLK_PCHCTRL_CHEN; 
    GCLK->PCHCTRL[SERCOM4_GCLK_ID_SLOW].reg = GCLK_PCHCTRL_GEN_GCLK3 | GCLK_PCHCTRL_CHEN; 

    //enable main clock controller for APB (shared bus)
    MCLK->APBDMASK.bit.SERCOM4_ = 1;

        //INIT USART/SERCOM MODULE
    usart_sync_init(&TARGET_IO, SERCOM4, (void *)NULL);
	TARGET_IO.io.read  = usart_sync_read;
	TARGET_IO.io.write = usart_sync_write;
	_config_debug_usart();

    //init pins for SERCOM
    cs_pin_set_pinmux(DEBUG_SERIAL_PORT, DEBUG_SERIAL_PIN_TX, PINMUX_PB08D_SERCOM4_PAD0);
    cs_pin_set_pinmux(DEBUG_SERIAL_PORT, DEBUG_SERIAL_PIN_RX, PINMUX_PB09D_SERCOM4_PAD1);
    
 

    // return STATUS_OK;
}

void cs_debug_usart_deinit(){
	__BKPT(3);
	//TODO
	MCLK->APBDMASK.bit.SERCOM4_ = 0;


}

void cs_debug_enable() {
	usart_sync_enable(&TARGET_IO);
	stdio_io_init(&TARGET_IO.io);

}

void cs_debug_disable() {
	__BKPT(3);

	//TODO
	usart_sync_disable(&TARGET_IO);

}

/*
 * \internal Write the given data to usart interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written.
 */
int32_t usart_sync_write(struct io_descriptor *const io_descr, const uint8_t *const buf, const uint16_t length)
{
	uint32_t                      offset = 0;
	struct usart_sync_descriptor *descr  = CONTAINER_OF(io_descr, struct usart_sync_descriptor, io);

	ASSERT(io_descr && buf && length);
	while (!_usart_sync_is_ready_to_send(&descr->device))
		;
	do {
		_usart_sync_write_byte(&descr->device, buf[offset]);
		while (!_usart_sync_is_ready_to_send(&descr->device))
			;
	} while (++offset < length);
	while (!_usart_sync_is_transmit_done(&descr->device))
		;
	return (int32_t)offset;
}

/*
 * \internal Read data from usart interface
 *
 * \param[in] descr The pointer to an io descriptor
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
int32_t usart_sync_read(struct io_descriptor *const io_descr, uint8_t *const buf, const uint16_t length)
{
	uint32_t                      offset = 0;
	struct usart_sync_descriptor *descr  = CONTAINER_OF(io_descr, struct usart_sync_descriptor, io);

	ASSERT(io_descr && buf && length);
	do {
		while (!_usart_sync_is_byte_received(&descr->device))
			;
		buf[offset] = _usart_sync_read_byte(&descr->device);
	} while (++offset < length);

	return (int32_t)offset;
}