/*  serial.h

    Summary:
    Definitions for UART serial connection
*/

#ifndef SERIAL_H
#define SERIAL_H

#include "serial_config.h"

#define BAUD_RATE ((F_CPU/16/BAUD) - 1)

void usart_init();

void usart_send(const char* szMessage);
void usart_recv(char* buffer, int maxLen);

#endif // SERIAL_H
