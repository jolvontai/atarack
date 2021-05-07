/*  ym2149.h

    Summary:
    Definitions for communication with ym2149 soundchip
*/

#ifndef YM2149_H
#define YM2149_H

#include <inttypes.h>

// Sound IC target frequency (2 MHz)
#define F_SND (2000000L)

// ---------
// REGISTERS
// ---------

#define SND_CHA_FINE_TONE   0x0
#define SND_CHA_ROUGH_TONE  0x1
#define SND_CHB_FINE_TONE   0x2
#define SND_CHB_ROUGH_TONE  0x3
#define SND_CHC_FINE_TONE   0x4
#define SND_CHC_ROUGH_TONE  0x5
#define SND_NOISE_FREQ      0x6
#define SND_IO_MIXER        0x7
#define SND_CHA_LEVEL       0x8
#define SND_CHB_LEVEL       0x9
#define SND_CHC_LEVEL       0xA
#define SND_ENV_FINE_TONE   0xB
#define SND_ENV_ROUGH_TONE  0xC
#define SND_ENV_SHAPE       0xD
#define SND_IO_PORTA        0xE
#define SND_IO_PORTB        0xF

// ----
// BITS
// ----

// SND_LEVEL bits

#define SND_LEVEL_MODE_ENV  0x10

// SND_MIXER bits

#define SND_MIXER_CHA_MUTE_TONE     0x01
#define SND_MIXER_CHB_MUTE_TONE     0x02
#define SND_MIXER_CHC_MUTE_TONE     0x04
#define SND_MIXER_CHA_MUTE_NOISE    0x08
#define SND_MIXER_CHB_MUTE_NOISE    0x10
#define SND_MIXER_CHC_MUTE_NOISE    0x20
#define SND_MIXER_IO_A              0x40
#define SND_MIXER_IO_B              0x80

// SND_ENV_SHAPE bits

#define SND_ENV_SHAPE_HOLD  0x01
#define SND_ENV_SHAPE_ALT   0x02
#define SND_ENV_SHAPE_ATT   0x04
#define SND_ENV_SHAPE_CONT  0x08

// ---------------------
// YM2149 register masks
// ---------------------

// Value masks

#define SND_MASK_FINE_TONE_VALUE 0xFF
#define SND_MASK_ROUGH_TONE_VALUE 0x0F
#define SND_MASK_ENV_ROUGH_TONE_VALUE 0xFF
#define SND_MASK_ENV_FINE_TONE_VALUE 0xFF
#define SND_MASK_NOISE_FREQ_VALUE 0xFF

// SND_IO_MIXER

#define SND_MASK_MIXER_TONE 0b00000111
#define SND_MASK_MIXER_NOISE 0b00111000
#define SND_MASK_MIXER_IO 0b11000000

// ------
// LIMITS
// ------

#define SND_MAX_VOLUME_LEVEL 0x0F

// ---------
// FUNCTIONS
// ---------

// Initialize sound chip
void snd_init();

// Write a value to a register
void snd_write(uint8_t reg, uint8_t value);

// Read value from a register
uint8_t snd_read(uint8_t reg);

#endif // YM2149_H
