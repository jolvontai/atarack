#include "state.h"

// Previous state of buttons
uint16_t _old_btn_states = 0;

void state_init(uint16_t btn_states)
{
    _old_btn_states = btn_states;
}

uint8_t state_mixer_changed(uint16_t btn_states, uint8_t* new_state)
{
    uint8_t mixer_state = 0;

    uint16_t changed_buttons;
    changed_buttons = _old_btn_states ^ btn_states;

    if(changed_buttons == 0)
    {
        *new_state = 0;
        return 0;
    }

    if(btn_states & BTN_CHA_ENABLE)
    {
        mixer_state |= SND_MIXER_CHA_MUTE_TONE;
    }
    if(btn_states & BTN_CHA_NOISE)
    {
        mixer_state |= SND_MIXER_CHA_MUTE_NOISE;
    }

    if(btn_states & BTN_CHB_ENABLE)
    {
        mixer_state |= SND_MIXER_CHB_MUTE_TONE;
    }
    if(btn_states & BTN_CHB_NOISE)
    {
        mixer_state |= SND_MIXER_CHB_MUTE_NOISE;
    }

    if(btn_states & BTN_CHC_ENABLE)
    {
        mixer_state |= SND_MIXER_CHC_MUTE_TONE;
    }
    if(btn_states & BTN_CHC_NOISE)
    {
        mixer_state |= SND_MIXER_CHC_MUTE_NOISE;
    }

    *new_state = mixer_state;
    _old_btn_states = btn_states;

    return 1;
}

uint8_t state_env_changed(uint16_t btn_states, uint8_t* new_state)
{
    uint8_t envelope_state = 0;

    uint8_t changed_buttons;

    changed_buttons = _old_btn_states ^ btn_states;

    if(changed_buttons == 0)
    {
        *new_state = 0;
        return 0;
    }

    if(btn_states & BTN_ENV_CONT)
    {
        envelope_state |= SND_ENV_SHAPE_CONT;
    }

    if(btn_states & BTN_ENV_ATT)
    {
        envelope_state |= SND_ENV_SHAPE_ATT;
    }


    if(btn_states & BTN_ENV_ALT)
    {
        envelope_state |= SND_ENV_SHAPE_ALT;
    }

    if(btn_states & BTN_ENV_HOLD)
    {
        envelope_state |= SND_ENV_SHAPE_HOLD;
    }

    *new_state = envelope_state;

    return 1;
}

uint8_t state_cha_changed(uint16_t btn_states, uint8_t* new_state)
{
    uint8_t cha_state = SND_MAX_VOLUME_LEVEL;

    if((_old_btn_states ^ btn_states) == 0)
    {
        *new_state = 0;
        return 0;
    }

    if(btn_states & BTN_CHA_ENV)
    {
        cha_state |= SND_LEVEL_MODE_ENV;
    }

    *new_state = cha_state;
    return 1;
}

uint8_t state_chb_changed(uint16_t btn_states, uint8_t* new_state)
{
    uint8_t chb_state = SND_MAX_VOLUME_LEVEL;

    if((_old_btn_states ^ btn_states) == 0)
    {
        *new_state = 0;
        return 0;
    }

    if(btn_states & BTN_CHB_ENV)
    {
        chb_state |= SND_LEVEL_MODE_ENV;
    }

    *new_state = chb_state;
    return 1;
}

uint8_t state_chc_changed(uint16_t btn_states, uint8_t* new_state)
{
    uint8_t chc_state = SND_MAX_VOLUME_LEVEL;

    if((_old_btn_states ^ btn_states) == 0)
    {
        *new_state = 0;
        return 0;
    }

    if(btn_states & BTN_CHC_ENV)
    {
        chc_state |= SND_LEVEL_MODE_ENV;
    }

    *new_state = chc_state;
    return 1;
}
