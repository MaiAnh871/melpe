#ifndef OMAP_L138_UART_H_INCLUDED
#define OMAP_L138_UART_H_INCLUDED

#include <string.h>
#include "LCDK_Support_DSP.h"

#define BAUD_RATE_UART 921600
#define UART_BUFFER_SIZE 125000

extern void uart_poolling();
extern void init_uart_buffer();
extern Uint8 is_uart_buffer_empty();
extern Uint8 is_full_uart_buffer();
extern void write_uart_buffer(Uint8 byte_in);
extern Uint8 read_uart_buffer();
extern void write_uart (Uint8 byte_in);

#endif /* OMAP_L138_UART_H_INCLUDED */
