#ifndef LED_DRIVER_H
#define LED_DRIVER_H



#include <avr/io.h>

// ------------------------
// Functional configuration
// ------------------------

#ifndef LDR_USE_CONSTANT_BRT
#define LDR_USE_CONSTANT_BRT 1
#endif

#ifndef LDR_NBR_OF_DIG
#define LDR_NBR_OF_DIG 1
#endif

// ------------------
// Port Configuration
// ------------------

#ifndef LDR_INT
#define LDR_INT PORTD2
#endif

#ifndef LDR_INT_REG
#define LDR_INT_REG PORTD
#endif

#ifndef LDR_DIN
#define LDR_DIN PORTB5
#endif

#ifndef LDR_DIN_REG
#define LDR_DIN_REG PORTB
#endif

#ifndef LDR_CLK
#define LDR_CLK PORTB4
#endif

#ifndef LDR_CLK_REG
#define LDR_CLK_REG PORTB
#endif

#ifndef LDR_STB
#define LDR_STB PORTB3
#endif

#ifndef LDR_STB_REG
#define LDR_STB_REG PORTB
#endif

// -----------------
// STLED316 Commands
// -----------------

#ifndef LDR_RD_COMMAND
#define LDR_RD_COMMAND 0x40
#endif

#ifndef LDR_WR_COMMAND
#define LDR_WR_COMMAND 0x00
#endif

#ifndef LDR_DISP_ON_COMMAND
#define LDR_DISP_ON_COMMAND 0x0D
#endif

#ifndef LDR_DISP_OFF_COMMAND
#define LDR_DISP_OFF_COMMAND 0x0E
#endif

// ----------------------------
// STLED316S Register addresses
// ----------------------------

#ifndef LDR_ADDR_LED_DATA
#define LDR_ADDR_LED_DATA 0x00
#endif

#ifndef LDR_ADDR_KEY_DATA_1
#define LDR_ADDR_KEY_DATA_1 0x01
#endif

#ifndef LDR_ADDR_KEY_DATA_2
#define LDR_ADDR_KEY_DATA_2 0x02
#endif

// ---------------
// STLED316S pages
// ---------------

#ifndef LDR_PAGE_READ
#define LDR_PAGE_READ 0x08
#endif

#ifndef LDR_PAGE_LED
#define LDR_PAGE_LED 0x08
#endif

#ifndef LDR_PAGE_DIG
#define LDR_PAGE_DIG 0x00
#endif

#ifndef LDR_PAGE_DIG_CONFIG
#define LDR_PAGE_DIG_CONFIG 0x10
#endif

#ifndef LDR_PAGE_LED_BRT
#define LDR_PAGE_LED_BRT 0x18
#endif

// -------------------------
// STLED316S command helpers
// -------------------------

#ifndef LDR_INC_ADDR
#define LDR_INC_ADDR 0x00
#endif

#ifndef LDR_FIXED_ADDR
#define LDR_FIXED_ADDR 0x20
#endif

#ifndef LDR_CONST_BRT 
#define LDR_CONST_BRT 0x18
#endif

#ifndef LDR_VAR_BRT
#define LDR_VAR_BRT 0x00
#endif

#ifndef LDR_FULL_POWER
#define LDR_FULL_POWER 0xE0
#endif

// ---------
// Functions
// ---------

void ldr_init(int digits);

void ldr_send_data(uint8_t* data, uint8_t length);

uint8_t ldr_read_data(uint8_t address);

void ldr_set_display_state(_Bool state);

void ldr_set_discreet_leds(uint8_t states);

void ldr_set_discreet_brightness(uint8_t brightness);

void ldr_set_dig_leds(uint8_t states, uint8_t dig);

void ldr_set_dig_brightness(uint8_t brightness);

uint16_t ldr_get_buttons();

#endif // LED_DRIVER_H