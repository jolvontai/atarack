#include "ym2149.h"
#include "notes.h"
#include "uart.h"
#include "buttons.h"
#include "stled316s.h"
#include "adc.h"
#include "state.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

void test_ym2149()
{
  // Initialize sound chip
  snd_init();

  // Initialize UART (currently only used in debugging)
  UART_init();

  // Initialize stled
  ldr_init();

  adc_init();
  
  sei();



  // Mute noise
  snd_write(SND_IO_MIXER,
            SND_MIXER_CHC_MUTE_NOISE | SND_MIXER_CHB_MUTE_NOISE | SND_MIXER_CHA_MUTE_NOISE);

  snd_write(SND_ENV_SHAPE, SND_ENV_SHAPE_CONT | SND_ENV_SHAPE_ALT);

  snd_write(SND_ENV_FINE_TONE, 0x1F);

  snd_write(SND_ENV_ROUGH_TONE, 0x1F);

  // Full blast
  snd_write(SND_CHA_LEVEL, 0xF);
  snd_write(SND_CHB_LEVEL, 0xF);
  snd_write(SND_CHC_LEVEL, 0xF);

  // A nice tune
  Note tune[] = {
      {A4, 250}, {B4, 250}, {D5, 250}, {B4, 250}, {Gb5, 300}, {0, 100}, {Gb5, 300}, {0, 100}, {E5, 450}, {0, 250}, {A4, 250}, {B4, 250}, {D5, 250}, {B4, 250}, {E5, 300}, {0, 100}, {E5, 300}, {0, 100}, {D5, 250}, {Db5, 250}, {B4, 250}, {A4, 250}, {B4, 250}, {D5, 250}, {B4, 250}, {D5, 500}, {E5, 250}, {Db5, 250}, {A4, 450}, {0, 50}, {A4, 250}, {A4, 250}, {E5, 250}, {D5, 500}, {0, 500}};

  // Start playing the tune in loop
  uint8_t i = 0;
  uint16_t delay = 0;
  uint32_t len = (sizeof(tune) / 4);

  uint16_t btn_states;

  btn_states = ldr_get_buttons();

  state_init(btn_states);

  uint16_t adc_values[5];

  while (1)
  {
    if (delay > tune[i].delay)
    {
      i = (i + 1) % len;
      delay = 0;
    }

    if(ldr_buttons_updated(&btn_states))
    {
      // Next led states
      uint8_t discreet_leds = (btn_states & 0xFF); // next discreet leds
      uint8_t dig_leds = (btn_states >> 8); // next dig leds

      uint8_t new_state;

      if(state_mixer_changed(btn_states, &new_state))
      {
        snd_write(SND_IO_MIXER, new_state);
      }

      if(state_env_changed(btn_states, &new_state))
      {
        snd_write(SND_ENV_SHAPE, new_state);
      }

      if(state_cha_changed(btn_states, &new_state))
      {
        snd_write(SND_CHA_LEVEL, new_state);
      }

      if(state_chb_changed(btn_states, &new_state))
      {
        snd_write(SND_CHB_LEVEL, new_state);
      }

      if(state_chc_changed(btn_states, &new_state))
      {
        snd_write(SND_CHC_LEVEL, new_state);
      }

      // Update ui
      ldr_set_discreet_leds(discreet_leds); // Update discreet leds
      ldr_set_dig_leds(dig_leds, 0); // Update dig leds
    }

   uint16_t new_adc_value = 0;

  // new_adc_value = adc_read_pin(PORTA0);

  // if(new_adc_value != adc_values[PORTA0])
  // {
  //   snd_write(SND_CHA_FINE_TONE, new_adc_value & 0xFF);
  //   snd_write(SND_CHA_ROUGH_TONE, (new_adc_value >> 8));

  //   adc_values[PORTA0] = new_adc_value;
  // }

  // _delay_us(250);

   //new_adc_value = adc_read_pin(PORTA1);

  // if(new_adc_value != adc_values[PORTA1])
  // {
  //   snd_write(SND_CHB_FINE_TONE, new_adc_value & 0xFF);
  //   snd_write(SND_CHB_ROUGH_TONE, (new_adc_value >> 8));

  //   adc_values[PORTA1] = new_adc_value;
  // }

  //UART_vsend("adc arvo: %d", new_adc_value);

    // int data = tune[i].note;

    // snd_write(SND_CHA_FINE_TONE, data & 0xff);
    // snd_write(SND_CHA_ROUGH_TONE, data >> 8);

    // snd_write(SND_CHB_FINE_TONE, data & 0xff);
    // snd_write(SND_CHB_ROUGH_TONE, data >> 8);

    // snd_write(SND_CHC_FINE_TONE, data & 0xff);
    // snd_write(SND_CHC_ROUGH_TONE, data >> 8);

    // delay += 50;
    // _delay_ms(50);
  }
}

int main(void)
{
  test_ym2149();
  return 0;
}