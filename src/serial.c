#include "serial.h"

#include <stdarg.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init()
{
    // Baud rate
    UBRRH = (uint8_t)(BAUD_RATE >> 8);
    UBRRL = (uint8_t)BAUD_RATE;

    // Enable RX and TX
    UCSRB = (1 << RXEN) | (1 << TXEN);

    //  Write to UCSRC, Frame format: 8 bits for data, 1 for stop
    UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

void uart_tx(uint8_t data)
{
    // Wait empty buffer
    while (!(UCSRA & (1 << UDRE)))
        ;

    // Place data into buffer
    UDR = data;
}

uint8_t uart_rx()
{
    // Wait data
    while (!(UCSRA & (1 << RXC)))
        ;
    return UDR;
}

void uart_send(const char *szMessage)
{
    int i = 0;
    while (szMessage[i] != '\0')
    {
        uart_tx(szMessage[i]);
        i++;
    }

    uart_tx('\r');
    uart_tx('\n');
}

void uart_vsend(const char* fmt, ...)
{
    static char buff[256] = {};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, 255, fmt, args);
    uart_send(buff);
    va_end(args);
}
