#include "ym2149.h"
#include "notes.h"
#include "uart.h"
#include "buttons.h"
#include "stled316s.h"
#include "adc.h"
#include "state.h"
#include "util.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

typedef struct
{
  uint8_t mixer_state;
  uint8_t envelope_state;
  uint8_t cha_state;
  uint8_t chb_state;
  uint8_t chc_state;
} current_states;

#define RICK_ROLL 0

#if(RICK_ROLL)
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

  //snd_write(SND_ENV_SHAPE, SND_ENV_SHAPE_CONT | SND_ENV_SHAPE_ALT);

  //snd_write(SND_ENV_FINE_TONE, 0x1F);

  //snd_write(SND_ENV_ROUGH_TONE, 0x1F);

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
        UART_vsend("ny kirjotetaa mixerii");
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

  new_adc_value = adc_read_pin(PORTA0);

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
#endif

void run_atarack(void)
{
    // Initialize sound chip
  snd_init();

  // Initialize UART (currently only used in debugging)
  UART_init();

  // Initialize stled
  ldr_init();

  // Initialize adc 
  adc_init();
  
  // Enable interrupts
  sei();

  // Full blast
  snd_write(SND_CHA_LEVEL, 0xF);
  snd_write(SND_CHB_LEVEL, 0xF);
  snd_write(SND_CHC_LEVEL, 0xF);

  // Initialize state machine
  uint16_t btn_states = ldr_get_buttons();
  state_init(btn_states);

  // Local holder for current states
  current_states states;

  // Fill the states with starting values
  state_mixer_changed(btn_states, &states.mixer_state);
  state_cha_changed(btn_states, &states.cha_state);
  state_chb_changed(btn_states, &states.chb_state);
  state_chc_changed(btn_states, &states.chc_state);
  state_env_changed(btn_states, &states.envelope_state);

  // ADC value helper
  uint16_t new_adc_value = 0;

  // Soundchip value helper
  uint16_t new_snd_value = 0;

  while (1)
  {
    if(ldr_buttons_updated(&btn_states))
    {
      // Next led states
      uint8_t discreet_leds = (btn_states & 0xFF); // Next discreet leds
      uint8_t dig_leds = (btn_states >> 8); // Next dig leds

      // Check mixer buttons (channel enables, noise enables)
      if(state_mixer_changed(btn_states, &states.mixer_state))
      {
        snd_write(SND_IO_MIXER, states.mixer_state);
      }

      // Check envelope buttons (ATT, ALT, CONT...)
      if(state_env_changed(btn_states, &states.envelope_state))
      {
        snd_write(SND_ENV_SHAPE, states.envelope_state);
      }

      // Check channel A envelope enabled
      if(state_cha_changed(btn_states, &states.cha_state))
      {
        snd_write(SND_CHA_LEVEL, states.cha_state);
      }

      // Check channel B envelope enabled
      if(state_chb_changed(btn_states, &states.chb_state))
      {
        snd_write(SND_CHB_LEVEL, states.chb_state);
      }

      // Check channel C envelope enabled
      if(state_chc_changed(btn_states, &states.chc_state))
      {
        snd_write(SND_CHC_LEVEL, states.chc_state);
      }

      // Update the old reference button now that we've compared all states
      state_update_old_buttons(btn_states);

      // Update ui
      ldr_set_discreet_leds(discreet_leds); // Update discreet leds
      ldr_set_dig_leds(dig_leds, 0); // Update dig leds

      // UART_vsend("mixer: %d, envelope: %d, CHA: %d, CHB: %d, CHC: %d",
      //   states.mixer_state,
      //   states.envelope_state,
      //   states.cha_state,
      //   states.chb_state,
      //   states.chc_state);
    }

    // CHA TONE enabled
    if((states.mixer_state & SND_MIXER_CHA_MUTE_TONE) == 0)
    {
      // Read the adc value
      new_adc_value = adc_read_pin(PORTA1) + 1;

      // Get the tone value from the adc value
      new_snd_value = convert_adc_to_tone(new_adc_value);

      // UART_vsend("adc value: %d, CHA value: %d, min tone: %f, max tone: %f", 
      //   new_adc_value, 
      //   new_freq_value,
      //   MIN_TONE_FREQ,
      //   MAX_TONE_FREQ);

      // Write 12 bits
      snd_write(SND_CHA_FINE_TONE, new_snd_value & 0xFF);
      snd_write(SND_CHA_ROUGH_TONE, ((new_snd_value >> 8) & 0xF));
      
    }

    // CHB TONE enabled
    if((states.mixer_state & SND_MIXER_CHB_MUTE_TONE) == 0)
    {
      // Read the adc value
      new_adc_value = adc_read_pin(PORTA2);

      // Get the tone value from the adc value
      new_snd_value = convert_adc_to_tone(new_adc_value);

      // UART_vsend("adc value: %d, CHB value: %d, min tone: %f, max tone: %f", 
      //   new_adc_value, 
      //   new_freq_value,
      //   MIN_TONE_FREQ,
      //   MAX_TONE_FREQ);

      // Write 12 bits
      snd_write(SND_CHB_FINE_TONE, new_snd_value & 0xFF);
      snd_write(SND_CHB_ROUGH_TONE, ((new_snd_value >> 8) & 0xF));
    }

    // CHC TONE enabled
    if((states.mixer_state & SND_MIXER_CHC_MUTE_TONE) == 0)
    {
      // Read the adc value
      new_adc_value = adc_read_pin(PORTA3);

      // Get the tone value from the adc value
      new_snd_value = convert_adc_to_tone(new_adc_value);

      // UART_vsend("adc value: %d, CHC value: %d, min tone: %f, max tone: %f", 
      //   new_adc_value, 
      //   new_freq_value,
      //   MIN_TONE_FREQ,
      //   MAX_TONE_FREQ);

      // Write 12 bits
      snd_write(SND_CHA_FINE_TONE, new_snd_value & 0xFF);
      snd_write(SND_CHA_ROUGH_TONE, ((new_snd_value >> 8) & 0xF));
    }

    // Noise enabled if it is enabled in any of the channels
    if((states.mixer_state & SND_MASK_MIXER_NOISE) != SND_MASK_MIXER_NOISE)
    {
      // Read the adc value
      new_adc_value = adc_read_pin(PORTA4);

      // Get the noise value from the adc value
      new_snd_value = convert_adc_to_noise(new_adc_value);

      

      // Write only the first 5 bits
      snd_write(SND_NOISE_FREQ, new_snd_value & 0x1F);
    }

    // Update envelope only if it is enabled on any of the channels
    if((states.cha_state & SND_LEVEL_MODE_ENV) | (states.chb_state & SND_LEVEL_MODE_ENV) | (states.chc_state & SND_LEVEL_MODE_ENV))
    {
      // Read the adc value
      new_adc_value = adc_read_pin(PORTA0);

      // UART_vsend("adc value: %d, ENV value: %d, min env: %f, max env: %f", 
      //   new_adc_value, 
      //   new_freq_value,
      //   MIN_ENV_FREQ,
      //   MIN_ENV_FREQ);

      // Get the envelope value from the adc value
      new_snd_value = convert_adc_to_envelope(new_adc_value);

      // Write 16 bits
      snd_write(SND_ENV_FINE_TONE, new_snd_value & 0xFF);
      snd_write(SND_ENV_ROUGH_TONE, (new_snd_value >> 8));
    }
  }
}

int main(void)
{
#if(RICK_ROLL)
  test_ym2149();
#else
  run_atarack();
#endif

  return 0;
}