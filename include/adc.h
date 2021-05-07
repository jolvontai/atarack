/*  adc.h

    Summary:
    Definitions for usage of adc
*/

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

// ----------------
// Port definitions 
// ----------------

#define ADC_CHA_PORT   PORTA1
#define ADC_CHB_PORT   PORTA2
#define ADC_CHC_PORT   PORTA3
#define ADC_NOISE_PORT PORTA0
#define ADC_ENV_PORT   PORTA4

// ---------
// Functions
// ---------

void adc_init();

//void adc_set_channel_state(uint8_t channel, uint8_t enabled);

uint16_t adc_read_pin(uint8_t pin);

#endif //ADC_H