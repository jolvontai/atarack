/*  util.h

    Summary:
    Collection of utility functions
*/

#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>

// Map value to range
long map(long value, long input_min, long input_max, long output_min, long output_max)
{
	if(value > input_max)
		value = input_max;
	else if(value < input_min)
		value = input_min;
		
	return (value - input_min) * (output_max - output_min) / (input_max	- input_min) + output_min;
}

#endif //UTIL_H
