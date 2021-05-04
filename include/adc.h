#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void adc_init();

void adc_set_channel_state(uint8_t channel, uint8_t enabled);

uint16_t adc_read_pin(uint8_t pin);

#endif //ADC_H