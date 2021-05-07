/*  util.h

    Summary:
    Collection of utility functions
*/

#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <math.h>

#include "config.h"

// Map value to range (int version)
long map_int(long value, long input_min, long input_max, long output_min, long output_max)
{
	if(value > input_max)
		value = input_max;
	else if(value < input_min)
		value = input_min;
		
	return (value - input_min) * (output_max - output_min) / (input_max	- input_min) + output_min;
}

// Map value to range (float version)
double map_dec(double value, double input_min, double input_max, double output_min, double output_max)
{
	if(value > input_max)
		value = input_max;
	else if(value < input_min)
		value = input_min;
		
	return (value - input_min) * (output_max - output_min) / (input_max	- input_min) + output_min;
}

// Transforms the adc value to soundchip ready tone value
uint16_t convert_adc_to_tone(uint16_t value)
{
	double percentage_value = (double)value / ADC_MAX_VALUE;

	if(percentage_value > 1.0)
		percentage_value = 1.0;

	double octaves = OCTAVES * percentage_value;

	// This is nearly perfect, but the halfway between octaves is off by a little
	double freq = MIN_TONE_FREQ * pow(2.0, octaves);

	if(freq > MAX_TONE_FREQ)
	{
		freq = MAX_TONE_FREQ;
	}

	return (uint16_t)(TONE(freq));
}

// Transforms the adc value to soundchip ready noise value
uint16_t convert_adc_to_noise(uint16_t value)
{
	double percentage_value = (double)value / ADC_MAX_VALUE;

	if(percentage_value > 1.0)
		percentage_value = 1.0;

	double octaves = OCTAVES * percentage_value;

	// This is nearly perfect, but the halfway between octaves is off by a little
	double freq = MIN_NOISE_FREQ * pow(2.0, octaves);

	// Clamp the frequency to highest possible
	if(freq > MAX_NOISE_FREQ)
	{
		freq = MAX_NOISE_FREQ;
	}

	return (uint16_t)(TONE(freq));
}

// Transforms the adc value to soundchip ready envelope value
uint16_t convert_adc_to_envelope(uint16_t value)
{
	double percentage_value = (double)value / ADC_MAX_VALUE;

	if(percentage_value > 1.0)
		percentage_value = 1.0;

	double octaves = OCTAVES * percentage_value;

	// This is nearly perfect, but the halfway between octaves is off by a little
	double freq = MIN_ENV_FREQ * pow(2.0, octaves);

	// Clamp the frequency to highest possible
	if(freq > MAX_ENV_FREQ)
	{
		freq = MAX_ENV_FREQ;
	}

	return (uint16_t)(ENVELOPE(freq));
}


#endif //UTIL_H
