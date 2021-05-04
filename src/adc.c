#include "adc.h"
#include "uart.h"
#include <avr/interrupt.h>

/*
    PIN configuration:
    AVR                        UI
    ----+---------+--------------
    A0    <----->     ENV CV (J5)
    A1    <----->     CHA CV (J1)
    A2    <----->     CHB CV (J2)
    A3    <----->     CHC CV (J3)
    A4    <----->   NOISE Cv (J4)  
*/

typedef struct 
{
    uint16_t value;
    //uint8_t enabled; // TODO enable this if deemed necessary
} adc_channel;

volatile adc_channel _channel_data[5];

uint8_t _started = 0;

void adc_init()
{
    // AVCC ref 
    ADMUX = _BV(REFS0);

    // 128 prescaler, 
    ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    // Disable input (A0-A4)
    DDRA &= 0xE0;

    // No pullup (A0-A4)
    PORTA &= 0xE0;

    // Reset channel_data
    for(int i = 0; i < 5; i++)
    {
        //_channel_data[i].enabled = 0;
        _channel_data[i].value = 0;
    }

    // Enable auto-trigger and interrupt
    ADCSRA |= _BV(ADATE) | _BV(ADIE);

    // Enable adc
    ADCSRA |= _BV(ADEN);

    // Flag to allow the setting of the ADSC once later
    _started = 0;
}

void adc_set_channel_state(uint8_t channel, uint8_t enabled)
{
    // if(enabled && bit_is_clear(ADCSRA, ADSC))
    // {
    //     ADCSRA |= _BV(ADSC);
    // }
    
   // _channel_data[channel].enabled = enabled;
}

uint16_t adc_read_pin(uint8_t pin)
{
    if(_started == 0)
    {
        ADCSRA |= _BV(ADSC);
        _started = 1;
    }

    // UART_vsend("kanava 1: %d, kanava 2: %d, kanava 3: %d, kanava 4: %d, kanava 5: %d", 
    //     _channel_data[0].value,
    //     _channel_data[1].value,
    //     _channel_data[2].value,
    //     _channel_data[3].value,
    //     _channel_data[4].value);

    return _channel_data[pin].value;
}

uint8_t _current_index = 0;
ISR (ADC_vect)
{
    uint16_t adc_value;
    adc_value = ADC;

    _channel_data[_current_index].value = adc_value;//map(adc_value, 0, 0x3FF,0, 0xFFF); 

    // Increment index
    _current_index++;

    // Reset back to zero
    if(_current_index >= 5)
    {
        _current_index = 0;
    }

    // Change channel
    ADMUX = _BV(REFS0) | (_current_index & 0x07);
}