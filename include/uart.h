/*  uart.h

    Summary:
    Definitions for UART serial connection
*/

#ifndef UART_H
#define UART_H

#include "config.h"

#include <inttypes.h>

// UART baud rate
#define BAUD_RATE ((F_CPU/16/BAUD) - 1)

// Initialize UART serial connection
void UART_init();

// Send a byte via UART
void UART_tx(uint8_t data);

// Poll a byte via UART
uint8_t UART_rx();

// Send a line via UART
void UART_send(const char* szMessage);

// Send a formatted string via UART
void UART_vsend(const char* fmt, ...);

#endif // UART_H
