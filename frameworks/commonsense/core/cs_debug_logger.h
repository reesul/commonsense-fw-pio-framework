#pragma once

// #include <commonsense.h>
#include <cs_pin_config.h>
#include <cs_pin_mapping.h>
#include <cs_stdio_redirect.h>

#include <utils.h>
#include <utils_assert.h>
#include <hal_usart_sync.h>

#define DEBUG_USART_BAUD 230400

// struct usart_sync_descriptor TARGET_IO; //STDIO will redirect all IO through this interface; default to SERCOM4 USRT (Synchronous!)


void cs_debug_init();
void cs_debug_usart_init();//replace with status/configuration return type
void cs_debug_usart_deinit();

void cs_debug_enable();
void cs_debug_disable();

int32_t usart_sync_write(struct io_descriptor *const io_descr, const uint8_t *const buf, const uint16_t length);
int32_t usart_sync_read(struct io_descriptor *const io_descr, uint8_t *const buf, const uint16_t length);


