#include "ym2149.h"

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#error "F_CPU is not defined"
#endif

/*
    PIN configuration:
    AVR                YM2149
    -------+---------+-------
    PC0~7    <----->   DA0~7
    PB0      <----->     BC1
    PB1      <----->    BDIR
    PB3/OC0  <----->   CLOCK
*/

void _snd_sel_reg(uint8_t reg);
uint8_t _snd_read();
void _snd_write(uint8_t value);

void snd_init()
{
    // CLOCK
    DDRB |= (1 << PORTB3);          // Clock output pin
    TCNT0 = 0;                      // Initialize counter
    OCR0 = (F_CPU / F_SND / 2) - 1; // Output compare match
    TCCR0 |= (1 << COM00);          // Toggle when timer match
    TCCR0 |= (1 << WGM01);          // CTC Mode
    TCCR0 |= (1 << CS00);           // No prescaling

    // BUS CONTROL
    DDRB |= (1 << PORTB1); // BDIR out
    DDRB |= (1 << PORTB0); // BC1 out

    // Reset sound chip registers
    for (uint8_t i = 0; i < 16; i++)
    {
        snd_write(i, 0);
    }
}

void snd_write(uint8_t reg, uint8_t value)
{
    _snd_sel_reg(reg);
    _snd_write(value);
}

uint8_t snd_read(uint8_t reg)
{
    _snd_sel_reg(reg);
    return _snd_read();
}

void _snd_sel_reg(uint8_t reg)
{
    // Set port as output
    DDRC |= 0xFF;

    // Address mode
    PORTB = (PORTB & 0xFC) | (1 << PORTB0) | (1 << PORTB1);

    // Write register value to IO
    PORTC = reg;

    // Wait write to take place
    _delay_us(1.0);

    // Disable bus
    PORTB = (PORTB & 0xFC);

    _delay_us(1.0);
}

uint8_t _snd_read()
{
    // Set port as input
    DDRC &= ~0xFF;

    // Read mode
    PORTB = (PORTB & 0xFC) | (1 << PORTB0);

    _delay_us(1.0);

    uint8_t data = PINC;

    // Disable bus
    PORTB = (PORTB & 0xFC);

    _delay_us(1.0);

    return data;
}

void _snd_write(uint8_t value)
{
    // Set port as output
    DDRC |= 0xFF;

    // Write data to IO
    PORTC = value;

    // Write mode
    PORTB = (PORTB & 0xFC) | (1 << PORTB1);

    _delay_us(1.0);

    // Disable bus
    PORTB = (PORTB & 0xFC);

    _delay_us(1.0);
}
