#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void adc_init();

uint16_t adc_read_pin(uint8_t pin);

#endif //ADC_H