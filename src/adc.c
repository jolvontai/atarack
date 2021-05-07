#include <avr/interrupt.h>

#include "adc.h"

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

// holds the read data
volatile adc_channel _channel_data[5];

// Flag to allow the setting of the ADSC once later
uint8_t _started = 0;

void adc_init()
{
    // AVCC ref 
    ADMUX = (1 << REFS0);

    // 128 prescaler, 
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

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
    ADCSRA |= (1 << ADATE) | (1 << ADIE);

    // Enable adc
    ADCSRA |= (1 << ADEN);
}

// void adc_set_channel_state(uint8_t channel, uint8_t enabled)
// {
//     // if(enabled && bit_is_clear(ADCSRA, ADSC))
//     // {
//     //     ADCSRA |= _BV(ADSC);
//     // }
    
//    // _channel_data[channel].enabled = enabled;
// }

uint16_t adc_read_pin(uint8_t pin)
{
    if(_started == 0)
    {
        // start conversions if hasn't been started yet
        ADCSRA |= (1 << ADSC);
        _started = 1;
    }

    return _channel_data[pin].value;
}

uint8_t _current_index = 0;
ISR (ADC_vect)
{
    uint16_t adc_value;

    // Read conversion result
    adc_value = ADC; 

    // Store to channel data
    _channel_data[_current_index].value = adc_value;

    // Increment index
    _current_index++;

    // Reset back to zero
    if(_current_index >= 5)
    {
        _current_index = 0;
    }

    // Change channel
    ADMUX = (1 << REFS0) | (_current_index & 0x07);
}