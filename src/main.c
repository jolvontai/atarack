#include "ym2149.h"
#include "notes.h"

#include "serial.h"

#include "led_driver.h"

#include <util/delay.h>

#include <avr/interrupt.h>

#define USE_BUTTONS_IN_TEST 1

void test_ym2149()
{
  // Initialize sound chip
  snd_init();

  uart_init();

  // Initialize stled
  ldr_init(1);

  

  sei();

  // Reset sound chip registers
  for (uint8_t i = 0; i < 16; i++)
  {
    snd_write(i, 0);
  }

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

  uint16_t new_button_states = 0;

  uint16_t old_button_states = 0;

  old_button_states = ldr_get_buttons();

  // DISABLE NOISE, DISABLE TONE
  uint8_t mixer_state = 0x3F;

  uint8_t envelope_mode = 0x00;

  uint8_t envelope_changed = 0;

  while (1)
  {
    if (delay > tune[i].delay)
    {
      i = (i + 1) % len;
      delay = 0;
    }
#if(USE_BUTTONS_IN_TEST)
    if(ldr_buttons_updated(&new_button_states))
    {
      uint8_t discreet_leds = (new_button_states & 0xFF);
      uint8_t dig_leds = (new_button_states >> 8);
      // if state is changed
      if((old_button_states & BTN_CHA_ENABLE) ^ (new_button_states & BTN_CHA_ENABLE))
      {
        if(new_button_states & BTN_CHA_ENABLE)
        {
          mixer_state &= ~SND_MIXER_CHA_MUTE_TONE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHA_MUTE_TONE;
        }
      }

      if((old_button_states & BTN_CHB_ENABLE) ^ (new_button_states & BTN_CHB_ENABLE))
      {
        if(new_button_states & BTN_CHB_ENABLE)
        {
          mixer_state &= ~SND_MIXER_CHB_MUTE_TONE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHB_MUTE_TONE;
        }
      }

      if((old_button_states & BTN_CHC_ENABLE) ^ (new_button_states & BTN_CHC_ENABLE))
      {
        if(new_button_states & BTN_CHC_ENABLE)
        {
          mixer_state &= ~SND_MIXER_CHC_MUTE_TONE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHC_MUTE_TONE;
        }
      }

      if((old_button_states & BTN_CHA_NOISE) ^ (new_button_states & BTN_CHA_NOISE))
      {
        if(new_button_states & BTN_CHA_NOISE)
        {
          mixer_state &= ~SND_MIXER_CHA_MUTE_NOISE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHA_MUTE_NOISE;
        }
      }

      if((old_button_states & BTN_CHB_NOISE) ^ (new_button_states & BTN_CHB_NOISE))
      {
        if(new_button_states & BTN_CHB_NOISE)
        {
          mixer_state &= ~SND_MIXER_CHB_MUTE_NOISE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHB_MUTE_NOISE;
        }
      }

      if((old_button_states & BTN_CHC_NOISE) ^ (new_button_states & BTN_CHC_NOISE))
      {
        if(new_button_states & BTN_CHC_NOISE)
        {
          mixer_state &= ~SND_MIXER_CHC_MUTE_NOISE;
        }
        else
        {
          mixer_state |= SND_MIXER_CHC_MUTE_NOISE;
        }
      }

      if((old_button_states & BTN_CHA_ENVEL) ^ (new_button_states & BTN_CHA_ENVEL))
      {
        if(new_button_states & BTN_CHA_ENVEL)
        {
          snd_write(SND_CHA_LEVEL, SND_MAX_VOLUME_LEVEL | SND_LEVEL_MODE_ENV);
        }
        else
        {
          snd_write(SND_CHA_LEVEL, SND_MAX_VOLUME_LEVEL);
        }
      }

      if((old_button_states & BTN_CHB_ENVEL) ^ (new_button_states & BTN_CHB_ENVEL))
      {
        if(new_button_states & BTN_CHB_ENVEL)
        {
          snd_write(SND_CHB_LEVEL, SND_MAX_VOLUME_LEVEL | SND_LEVEL_MODE_ENV);
        }
        else
        {
          snd_write(SND_CHB_LEVEL, SND_MAX_VOLUME_LEVEL);
        }
      }

      if((old_button_states & BTN_CHC_ENVEL) ^ (new_button_states & BTN_CHC_ENVEL))
      {
        if(new_button_states & BTN_CHC_ENVEL)
        {
          snd_write(SND_CHC_LEVEL, SND_MAX_VOLUME_LEVEL | SND_LEVEL_MODE_ENV);
        }
        else
        {
          snd_write(SND_CHC_LEVEL, SND_MAX_VOLUME_LEVEL);
        }
      }

      if((old_button_states & BTN_ENV_CONT) ^ (new_button_states & BTN_ENV_CONT))
      {
        if(new_button_states & BTN_ENV_CONT)
        {
          envelope_mode |= SND_ENV_SHAPE_CONT;
        }
        else
        {
          envelope_mode &= ~SND_ENV_SHAPE_CONT;
        }
        envelope_changed = 1;
      }

      if((old_button_states & BTN_ENV_ATT) ^ (new_button_states & BTN_ENV_ATT))
      {
        if(new_button_states & BTN_ENV_ATT)
        {
          envelope_mode |= SND_ENV_SHAPE_ATT;
        }
        else
        {
          envelope_mode &= ~SND_ENV_SHAPE_ATT;
        }
        envelope_changed = 1;
      }

      if((old_button_states & BTN_ENV_ALT) ^ (new_button_states & BTN_ENV_ALT))
      {
        if(new_button_states & BTN_ENV_ALT)
        {
          envelope_mode |= SND_ENV_SHAPE_ALT;
        }
        else
        {
          envelope_mode &= ~SND_ENV_SHAPE_ALT;
        }
        envelope_changed = 1;
      }

      if((old_button_states & BTN_ENV_HOLD) ^ (new_button_states & BTN_ENV_HOLD))
      {
        if(new_button_states & BTN_ENV_HOLD)
        {
          envelope_mode |= SND_ENV_SHAPE_HOLD;
        }
        else
        {
          envelope_mode &= ~SND_ENV_SHAPE_HOLD;
        }
        envelope_changed = 1;
      }

      snd_write(SND_IO_MIXER, mixer_state);

      if(envelope_changed)
      {
        envelope_changed = 0;
        snd_write(SND_ENV_SHAPE, envelope_mode);
      }
     

      ldr_set_discreet_leds(discreet_leds);
      ldr_set_dig_leds(dig_leds, 0);

      old_button_states = new_button_states;
    }
#endif

    int data = tune[i].note;

    snd_write(SND_CHA_FINE_TONE, data & 0xff);
    snd_write(SND_CHA_ROUGH_TONE, data >> 8);

    snd_write(SND_CHB_FINE_TONE, data & 0xff);
    snd_write(SND_CHB_ROUGH_TONE, data >> 8);

    snd_write(SND_CHC_FINE_TONE, data & 0xff);
    snd_write(SND_CHC_ROUGH_TONE, data >> 8);

    delay += 50;
    _delay_ms(50);
  }
}

int main(void)
{
  test_ym2149();
  return 0;
}