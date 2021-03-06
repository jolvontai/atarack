/*  notes.h

    Summary:
    Collection of compiletime generated notes and useful macros to calculate soundchip register value from frequency
*/

#ifndef NOTES_H
#define NOTES_H

#include <inttypes.h>

#include "ym2149.h"

#ifndef F_SND
#error "F_SND not defined"
#endif

typedef struct
{
    uint16_t note;
    uint16_t delay;
} Note;

// Calculator for channel frequency
#define TONE(hz) (F_SND / (16.0 * hz))

// Calculator for envelope frequency
#define ENVELOPE(hz) (F_SND / (256.0 * hz))

#define C0 TONE(16.35)
#define Db0 TONE(17.32)
#define D0 TONE(18.35)
#define Eb0 TONE(19.45)
#define E0 TONE(20.60)
#define F0 TONE(21.83)
#define Gb0 TONE(23.12)
#define G0 TONE(24.50)
#define Ab0 TONE(25.96)
#define A0 TONE(27.50)
#define Bb0 TONE(29.14)
#define B0 TONE(30.87)

#define C1 TONE(32.70)
#define Db1 TONE(34.65)
#define D1 TONE(36.71)
#define Eb1 TONE(38.89)
#define E1 TONE(41.20)
#define F1 TONE(43.65)
#define Gb1 TONE(46.25)
#define G1 TONE(49.00)
#define Ab1 TONE(51.91)
#define A1 TONE(55.00)
#define Bb1 TONE(58.27)
#define B1 TONE(61.74)

#define C2 TONE(65.41)
#define Db2 TONE(69.30)
#define D2 TONE(73.42)
#define Eb2 TONE(77.78)
#define E2 TONE(82.41)
#define F2 TONE(87.31)
#define Gb2 TONE(92.50)
#define G2 TONE(98.0)
#define Ab2 TONE(103.83)
#define A2 TONE(110.00)
#define Bb2 TONE(116.54)
#define B2 TONE(123.47)

#define C3 TONE(130.81)
#define Db3 TONE(138.59)
#define D3 TONE(146.83)
#define Eb3 TONE(155.56)
#define E3 TONE(164.81)
#define F3 TONE(174.61)
#define Gb3 TONE(185.00)
#define G3 TONE(196.00)
#define Ab3 TONE(207.65)
#define A3 TONE(220.00)
#define Bb3 TONE(233.08)
#define B3 TONE(246.94)

#define C4 TONE(261.63)
#define Db4 TONE(277.18)
#define D4 TONE(293.66)
#define Eb4 TONE(311.13)
#define E4 TONE(329.63)
#define F4 TONE(349.23)
#define Gb4 TONE(369.99)
#define G4 TONE(392.00)
#define Ab4 TONE(415.30)
#define A4 TONE(440.00)
#define Bb4 TONE(466.16)
#define B4 TONE(493.88)

#define C5 TONE(523.25)
#define Db5 TONE(554.37)
#define D5 TONE(587.33)
#define Eb5 TONE(622.25)
#define E5 TONE(659.25)
#define F5 TONE(698.46)
#define Gb5 TONE(739.99)
#define G5 TONE(783.99)
#define Ab5 TONE(830.61)
#define A5 TONE(880.00)
#define Bb5 TONE(932.33)
#define B5 TONE(987.77)

#define C6 TONE(1046.50)
#define Db6 TONE(1108.73)
#define D6 TONE(1174.66)
#define Eb6 TONE(1244.51)
#define E6 TONE(1318.51)
#define F6 TONE(1396.91)
#define Gb6 TONE(1479.98)
#define G6 TONE(1567.98)
#define Ab6 TONE(1661.22)
#define A6 TONE(1760.00)
#define Bb6 TONE(1864.66)
#define B6 TONE(1975.53)

#define C7 TONE(2093.00)
#define Db7 TONE(2217.46)
#define D7 TONE(2349.32)
#define Eb7 TONE(2489.02)
#define E7 TONE(2637.02)
#define F7 TONE(2793.83)
#define Gb7 TONE(2959.96)
#define G7 TONE(3135.96)
#define Ab7 TONE(3322.44)
#define A7 TONE(3520.00)
#define Bb7 TONE(3729.31)
#define B7 TONE(3951.07)

#define C8 TONE(4186.01)
#define Db8 TONE(4434.92)
#define D8 TONE(4698.63)
#define Eb8 TONE(4978.03)
#define E8 TONE(5274.04)
#define F8 TONE(5587.65)
#define Gb8 TONE(5919.91)
#define G8 TONE(6271.93)
#define Ab8 TONE(6644.88)
#define A8 TONE(7040.00)
#define Bb8 TONE(7458.62)
#define B8 TONE(7902.13)

#endif // NOTES_H
