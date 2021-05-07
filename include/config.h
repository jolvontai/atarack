/*  config.h

    Summary:
    Collection of defines with which device can be tweaked
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <math.h>
#include "notes.h"

// voltage range is (0-5) so change of 5 octaves
#define OCTAVES 5

// Max reading one can get from CV channel (this should be checked)
// In ideal world this should be 10 bit max value = 1023
#define ADC_MAX_VALUE 812

// Frequency starts from C1.5
// Define greater min value if larger scale is wanted
#define MIN_TONE_FREQ 49.05

// 5 Octaves higher than the minimum frequency 
#define MAX_TONE_FREQ MIN_TONE_FREQ * pow(2, OCTAVES)

// The smallest frequency we can output from Envelope (with 2MHz == 0.11hz)
// Define greater min value if larger scale is wanted
// #define MIN_ENV_FREQ  F_SND / 256.0 / 65535.0

// Envelope starts from 16.0 now, should we abandon the 1v/oct here?
#define MIN_ENV_FREQ  16.0

// 5 Octaves higher than the minimum frequency
#define MAX_ENV_FREQ MIN_TONE_FREQ * pow(2, OCTAVES)

// Smallest possible frequency for noise (around 16.07hz)
#define MIN_NOISE_FREQ  F_SND / 16.0 / 7777.0

#endif //CONFIG_H