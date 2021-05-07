/*  state.h

    Summary:
    Used to keep track of states of the soundchip
*/

#ifndef STATE_H
#define STATE_H

#include <inttypes.h>

#include "buttons.h"
#include "ym2149.h"

void state_update_old_buttons(uint16_t btn_states);

uint8_t state_mixer_changed(uint16_t btn_states, uint8_t* new_state);

uint8_t state_env_changed(uint16_t btn_states, uint8_t* new_state);

uint8_t state_cha_changed(uint16_t btn_states, uint8_t* new_state);

uint8_t state_chb_changed(uint16_t btn_states, uint8_t* new_state);

uint8_t state_chc_changed(uint16_t btn_states, uint8_t* new_state);

#endif //STATE_H