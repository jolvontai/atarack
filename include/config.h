/*  config.h

    Summary:
    Collection of defines with which device can be tweaked
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <math.h>
#include "notes.h"

// override by defining before this
// else use monitor speed defined in the project settings
#ifndef BAUD
#define BAUD _UART_BAUD
#endif

// voltage range is (0-5) so change of 5 octaves
#define OCTAVES 5

// Max reading one can get from CV channel (this should be checked)
// In ideal world this should be 10 bit max value = 1023
#define ADC_MAX_VALUE 812

// Frequency starts from C1.5
// Define greater min value if larger scale is wanted
#define MIN_TONE_FREQ 49.05

// Highest possible tone frequency
#define MAX_TONE_FREQ F_SND / 16.0 / 1.0

// Envelope starts from 16.0 now, should we abandon the 1v/oct here?
#define MIN_ENV_FREQ  2.0

// Highest possible envelope freq
#define MAX_ENV_FREQ F_SND / 256.0 / 1

// Smallest possible frequency for noise (around 4032hz)
#define MIN_NOISE_FREQ  F_SND / 16.0 / 31.0

// HIghest possible frequency for noise (125Khz)
#define MAX_NOISE_FREQ F_SND / 16.0 / 1.0

// Channel volume (range: 0-15)
#define CHANNEL_VOLUME 0xF

// Led brightness (range: 0-7)
#define LED_BRIGHTNESS 7

#endif //CONFIG_H