#include "led_driver.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "serial.h"

#include <stdio.h>

volatile uint16_t _buttons;

volatile uint8_t _digits;

volatile uint8_t _buttons_updated = 0;

void ldr_init(int digits)
{

    // Reset buttons
    _buttons = 0;

    // Set the number of digits we have
    _digits = digits;


        // Enable interrupt on INT0, Active low
        GICR |= (1<<INT0);

        // Set as input
        DDRD |= (1<<PORTD2);

        // enable pullup
        PORTD |= (1<<PORTD2) | (1<<PORTD3);

        // set din, clk, and stb to output
        DDRB |= (1<<LDR_DIN) | (1<<LDR_CLK) | (1<<LDR_STB);
        
        // set data default to 0
        PORTB &= ~(_BV(LDR_DIN));

        // set clk default to 1
        PORTB |= _BV(LDR_CLK);

        // set stb default to 1
        PORTB |= _BV(LDR_STB);



    _delay_us(200);
    ldr_set_discreet_brightness(1);
    ldr_set_discreet_leds(0);
    ldr_set_dig_leds(0, 0);
    ldr_set_dig_brightness(7);
    ldr_set_display_state(1);
}

void _ldr_send_byte(uint8_t data)
{
    uint8_t i;

    for(i=0; i < 8;i++)
    {
        // Write 1
        if(data & (1<<i))
        {
            LDR_DIN_REG |= (1<<LDR_DIN);
        }
        else // write 0
        {
            LDR_DIN_REG &= ~(1<<LDR_DIN);   
        }

        // Simulate clock cycling
        LDR_CLK_REG &=  ~(1<<LDR_CLK);
        _delay_us(1);
        LDR_CLK_REG |= (1<<LDR_CLK);
        _delay_us(1);
    }
}

void _ldr_set_constant_brigthness(uint8_t brightness)
{
    uint8_t data[2];

    data[0] = LDR_WR_COMMAND | LDR_PAGE_DIG_CONFIG | LDR_INC_ADDR;

    //  MSB 5-7 = constant brightness value for all
    //  LSB 3,MSB 4 = 11 == constant brightness, 00 == variable brightness
    //  LSB 0-2 = number of digits-1 
    //data[1] = LDR_VAR_BRT | (_digits-1);
    // constant value of FULL POWER
    data[1] = (brightness << 5) | LDR_CONST_BRT | (_digits-1);

    ldr_send_data(&data[0], 2);
}

uint16_t ldr_get_buttons()
{
    uint16_t output = (ldr_read_data(LDR_ADDR_KEY_DATA_2)<<8) | ldr_read_data(LDR_ADDR_KEY_DATA_1);

    return output;
}

uint8_t ldr_read_data(uint8_t address)
{
    uint8_t i;
    uint8_t command = LDR_RD_COMMAND | LDR_PAGE_READ | address;
    uint8_t output = 0;

    //Initialize transaction
    LDR_CLK_REG |= _BV(LDR_CLK);
    LDR_STB_REG &= ~(_BV(LDR_STB));
    _delay_us(2);

    _ldr_send_byte(command);

    // set data as input
    DDRB &= ~(_BV(LDR_DIN));

    // enable pullup
    PORTB |= _BV(LDR_DIN);
    //PORTB &= ~(_BV(LDR_DIN));

    for(i = 0; i < 8; i++)
    {
        PORTB &= ~(_BV(LDR_CLK));

        _delay_us(2);

        PORTB |= _BV(LDR_CLK);

        if(PINB &  (1<<LDR_DIN))
        {
            output |= _BV(i);
            
        }
        _delay_us(2);
    }

    _delay_us(2);

    PORTB |= _BV(LDR_STB);
    DDRB |= _BV(LDR_DIN);
    PORTB |= _BV(LDR_DIN);


    return output;
}

void ldr_send_data(uint8_t* data, uint8_t length)
{
    uint8_t itr;
    uint8_t* pCUrrByte = data;
    uint8_t copyByte;

    // we should disable interrupts here

    LDR_CLK_REG |= (1<<LDR_CLK);
    LDR_STB_REG &= ~(1<<LDR_STB);
    _delay_us(2);
    for(itr = 0; itr < length;itr++)
    {
        copyByte = *pCUrrByte;
        _ldr_send_byte(copyByte);
        pCUrrByte++;
    }
    _delay_us(2);
    LDR_STB_REG |= (1<<LDR_STB);

    // Possibly re-enable  interrupts here
}

void ldr_set_display_state(_Bool state)
{
    uint8_t data = state ? LDR_DISP_ON_COMMAND : LDR_DISP_OFF_COMMAND;

    ldr_send_data(&data, 1);
}

void ldr_set_discreet_leds(uint8_t states)
{
    uint8_t data[2];

    data[0] = LDR_WR_COMMAND | LDR_PAGE_LED;
    data[1] = states;
    ldr_send_data(&data[0], 2);
}

void ldr_set_discreet_brightness(uint8_t brightness)
{
#if ( LDR_USE_CONSTANT_BRT )

    _ldr_set_constant_brigthness(brightness);

#else

    uint8_t data[5];

    data[0] = LDR_WR_COMMAND | LDR_PAGE_LED_BRT | LDR_INC_ADDR;
    data[1] = (brightness <<4) | brightness;
    data[2] = data[1];
    data[3] = data[1];
    data[4] = data[1];

    ldr_send_data(&data[0],5);

#endif

}

void ldr_set_dig_leds(uint8_t states, uint8_t dig)
{
    if(_digits == 0)
        return;

    uint8_t data[2];

    data[0] = LDR_WR_COMMAND | LDR_PAGE_DIG | LDR_FIXED_ADDR | dig;
    data[1] = states;

    ldr_send_data(&data[0], 2);
}

void ldr_set_dig_brightness(uint8_t brightness)
{
#if( LDR_USE_CONSTANT_BRT )

    _ldr_set_constant_brigthness(brightness);

#else

    uint8_t data[5];

    data[0] = LDR_WR_COMMAND | LDR_PAGE_DIG_CONFIG | LDR_INC_ADDR;
    data[1] = LDR_VAR_BRT | (_digits-1);
    for(uint8_t i = 2; i < 5;i++)
    {
        data[i] = (brightness<<4) | brightness;
    }

    ldr_send_data(&data[0], 5);

#endif
}

uint8_t ldr_buttons_updated(uint16_t* new_buttons)
{
    if(_buttons_updated)
        _buttons_updated = 0;

    *new_buttons = _buttons;

    return 1;
}

volatile uint16_t _buttons_state = 0;
ISR (INT0_vect)
{
    _buttons_state = ldr_get_buttons();

    // Inverse button states
    _buttons ^= _buttons_state;

    _buttons_updated = 1;
}