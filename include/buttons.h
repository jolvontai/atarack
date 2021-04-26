#ifndef BUTTONS_H
#define BUTTONS_H

// Channel A
#define BTN_CHA_ENABLE  (1<<2)
#define BTN_CHA_NOISE   (1<<5)
#define BTN_CHA_ENV   (1<<13)

// Channel B
#define BTN_CHB_ENABLE  (1<<1)
#define BTN_CHB_NOISE   (1<<4)
#define BTN_CHB_ENV   (1<<12)

// Channel C
#define BTN_CHC_ENABLE  (1<<0)
#define BTN_CHC_NOISE   (1<<3)
#define BTN_CHC_ENV   (1<<11)

// Envelope
#define BTN_ENV_CONT    (1<<14)          
#define BTN_ENV_ATT     (1<<10)
#define BTN_ENV_ALT     (1<<9)
#define BTN_ENV_HOLD    (1<<8)

#endif // BUTTONS_H
