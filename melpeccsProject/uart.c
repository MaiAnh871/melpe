#include "uart.h"

/* Local variable */
static Uint8 uart_buffer[UART_BUFFER_SIZE] = {0};
static Uint32 counter_read_uart_buffer;
static Uint32 counter_write_uart_buffer;

/* Define functions */
void init_uart_buffer()
{
    memset( (void *) uart_buffer, 0, sizeof(Uint8) * UART_BUFFER_SIZE);
    counter_read_uart_buffer = 0;
    counter_write_uart_buffer = 0;
}

Uint8 is_uart_buffer_empty()
{
    return (counter_read_uart_buffer == counter_write_uart_buffer);
}

Uint8 is_full_uart_buffer()
{
    return (counter_write_uart_buffer >= UART_BUFFER_SIZE);
}

void write_uart_buffer(Uint8 byte_in)
{
    /* Reset write counter for ring buffer */
    if (counter_write_uart_buffer > UART_BUFFER_SIZE)
    {
        return;
        //counter_write_uart_buffer = 0;
    }

    uart_buffer[counter_write_uart_buffer++] = byte_in;
}

Uint8 read_uart_buffer()
{
    if (!is_uart_buffer_empty())
    {
        /* Reset read counter for ring buffer */
        if (counter_read_uart_buffer > UART_BUFFER_SIZE)
        {
            return 0;
            //counter_read_uart_buffer = 0;
        }

        return uart_buffer[counter_read_uart_buffer++];
    }
    else
    {
        /* Show Error */
        return 'x';
    }
}

void write_uart (Uint8 byte_in)
{
    if (IsTxReady_UART2())
    {
        Write_UART2(byte_in);
    }
}

void uart_poolling()
{   
    /* For read data */
    if (IsDataReady_UART2())
    {
        Uint8 data = Read_UART2();
        write_uart_buffer(data);
    }
}
