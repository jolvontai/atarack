#include "adc.h"

void adc_init()
{
    // AVCC ref 
    ADMUX = _BV(REFS0);

    // 128 prescaler, 
    ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

    // input
    DDRA = 0;

    // no pullup
    PORTA = 0;

    // enable adc
    ADCSRA |= _BV(ADEN);
}

uint16_t adc_read_pin(uint8_t pin)
{
    ADMUX = _BV(REFS0) | (pin & 0x07);

    ADCSRA |= _BV(ADSC);

    while(ADCSRA & _BV(ADSC));

    return ADC;
}