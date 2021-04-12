#include "ym2149.h"
#include "notes.h"

#include <util/delay.h>

void test_ym2149()
{
  // Initialize sound chip
  snd_init();

  // Reset sound chip registers
  for (uint8_t i = 0; i < 16; i++)
  {
    snd_write(i, 0);
  }

  // Mute noise
  snd_write(SND_IO_MIXER,
            SND_MIXER_CHC_MUTE_NOISE | SND_MIXER_CHB_MUTE_NOISE | SND_MIXER_CHA_MUTE_NOISE);

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
  while (1)
  {
    if (delay > tune[i].delay)
    {
      i = (i + 1) % len;
      delay = 0;
    }

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