/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include QMK_KEYBOARD_H

#include <print.h>

enum planck_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _FN,
    _MOVE,
    _ALTNUM,
};

#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)
#define SPCLT(LAYER) LT(LAYER,KC_SPC)

typedef struct {
    bool is_press_action;
    int state;
} tap;

enum tap_names {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5,
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
};

enum tap_dances {
    TDLFN = 0,
    TDRAISE,
    TDCTRLS,
    TDALTNUM,
    TDGUIALT,
};

void lower_fn_finished (qk_tap_dance_state_t *state, void *user_data);
void lower_fn_reset (qk_tap_dance_state_t *state, void *user_data);

void raise_td_finished (qk_tap_dance_state_t *state, void *user_data);
void raise_td_reset (qk_tap_dance_state_t *state, void *user_data);

void ctrls_finished (qk_tap_dance_state_t *state, void *user_data);
void ctrls_reset (qk_tap_dance_state_t *state, void *user_data);

void alt_num_finished (qk_tap_dance_state_t *state, void *user_data);
void alt_num_reset (qk_tap_dance_state_t *state, void *user_data);

void guialt_finished (qk_tap_dance_state_t *state, void *user_data);
void guialt_reset (qk_tap_dance_state_t *state, void *user_data);

void _register_code(uint8_t code);
void _unregister_code(uint8_t code);

qk_tap_dance_action_t tap_dance_actions[] = {
    [TDLFN] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, lower_fn_finished, lower_fn_reset),
    [TDRAISE] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, raise_td_finished, raise_td_reset),
    [TDCTRLS] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ctrls_finished, ctrls_reset),
    [TDALTNUM] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, alt_num_finished, alt_num_reset),
    [TDGUIALT] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, guialt_finished, guialt_reset), 
    };

enum custom_keycodes {
    KC_PDEREF = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_ortho_4x12(
            KC_TAB,       KC_Q,    KC_W,    KC_E,         KC_R,      KC_T,         KC_Y,      KC_U,    KC_I,     KC_O,    KC_P,    KC_BSPC,
            TD(TDCTRLS),  KC_A,    KC_S,    KC_D,         KC_F,      KC_G,         KC_H,      KC_J,    KC_K,     KC_L,    KC_SCLN, LCTL_T(KC_QUOT),
            KC_LSFT,      KC_Z,    KC_X,    KC_C,         KC_V,      KC_B,         KC_N,      KC_M,    KC_COMM,  KC_DOT,  KC_SLSH, RSFT_T(KC_ENT),
            TD(TDALTNUM), KC_TILD, XXXXXXX, TD(TDGUIALT), TD(TDLFN), SPCLT(_MOVE), KC_SPC,    RAISE,   KC_PAUSE, XXXXXXX, KC_DEL,  KC_RALT
            ),

    [_LOWER] = LAYOUT_ortho_4x12(
            KC_GRV,       KC_EXLM, KC_AT,   KC_HASH,      KC_DLR,    XXXXXXX,      KC_PDEREF, KC_MINS, KC_PPLS,  KC_PAST, KC_EQL,  _______,
            TD(TDCTRLS),  KC_1,    KC_2,    KC_3,         KC_4,      KC_5,         KC_6,      KC_7,    KC_8,     KC_9,    KC_0,    KC_BSLS,
            KC_LSFT,      KC_PERC, KC_CIRC, KC_AMPR,      KC_PIPE,   KC_LBRC,      KC_RBRC,   KC_UNDS, KC_COMM,  KC_DOT,  KC_PSLS, RSFT_T(KC_ENT),
            _______,      _______, _______, _______,      _______,   KC_SPC,       KC_SPC,    RAISE,   _______,  _______, _______, _______
            ),

    [_RAISE] = LAYOUT_ortho_4x12(
            XXXXXXX,     KC_QUOT, KC_DQT,  KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, _______,
            TD(TDCTRLS), KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, XXXXXXX, _______,
            KC_LSFT,     XXXXXXX, XXXXXXX, KC_LT,   KC_GT,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_DEL,  KC_RSFT,
            _______,     _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______
            ),

    [_FN] = LAYOUT_ortho_4x12(
            XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F4,   XXXXXXX, XXXXXXX, XXXXXXX, KC_INSERT, XXXXXXX, KC_PSCR, _______,
            KC_LCTL, KC_F5,   KC_F6,   KC_F7,   KC_F8,   XXXXXXX, KC_MPRV, KC_VOLD, KC_VOLU,   KC_MNXT, KC_MPLY, _______,
            KC_LSFT, KC_F9,   KC_F10,  KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, KC_MUTE, XXXXXXX,   XXXXXXX, XXXXXXX, KC_RSFT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______
            ),

    [_MOVE] = LAYOUT_ortho_4x12(
            XXXXXXX, XXXXXXX, KC_HOME, KC_UP,   KC_END,  XXXXXXX, XXXXXXX, KC_BTN1, KC_MS_U, KC_BTN2, XXXXXXX, XXXXXXX,
            _______, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, _______,
            _______, XXXXXXX, KC_PGUP, XXXXXXX, KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
            ),

    [_ALTNUM] = LAYOUT_ortho_4x12(
            XXXXXXX, LALT(KC_1), LALT(KC_2), LALT(KC_3), LALT(KC_4), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
            KC_LCTL, LALT(KC_5), LALT(KC_6), LALT(KC_7), LALT(KC_8), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RCTL,
            KC_LSFT, XXXXXXX,    XXXXXXX,    LALT(KC_9), LALT(KC_0), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
            _______, XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
            ),
};

int non_delay_hold_dance (qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed)  return SINGLE_TAP;
        else return SINGLE_HOLD;
    }
    else if (state->count == 2) {
        if (state->pressed) 
            return DOUBLE_HOLD;
        else 
            return DOUBLE_TAP;
    } else if (state->count == 3) {
        if (state->pressed) 
            return TRIPLE_HOLD;
        else 
            return TRIPLE_TAP;
    }

    return 42;
}

static tap lower_fn_tap_state = {
    .is_press_action = true,
    .state = 0
};

void lower_fn_finished (qk_tap_dance_state_t *state, void *user_data) {
    lower_fn_tap_state.state = non_delay_hold_dance(state);
    switch (lower_fn_tap_state.state) {
        case SINGLE_HOLD: 
            layer_on(_LOWER); 
            break;
        case DOUBLE_HOLD: 
            layer_on(_FN);
            break;
    }
}

void lower_fn_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (lower_fn_tap_state.state==SINGLE_HOLD) {
        layer_off(_LOWER);
    } else if (lower_fn_tap_state.state == DOUBLE_HOLD) {
        layer_off(_FN);
    }
    lower_fn_tap_state.state = 0;
}

static tap raise_td_tap_state = {
    .is_press_action = true,
    .state = 0
};

void raise_td_finished (qk_tap_dance_state_t *state, void *user_data) {
    raise_td_tap_state.state = non_delay_hold_dance(state);
    switch (raise_td_tap_state.state) {
        case SINGLE_HOLD: 
            layer_on(_RAISE); 
            break;
        case SINGLE_TAP: 
    		_register_code(KC_LSFT);
    		_register_code(KC_MINS);
            break;
    }
}

void raise_td_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (raise_td_tap_state.state==SINGLE_HOLD) {
        layer_off(_RAISE);
    } else if (raise_td_tap_state.state == SINGLE_TAP) {
    	_unregister_code(KC_MINS);
    	_unregister_code(KC_LSFT);
    }
    raise_td_tap_state.state = 0;
}

static tap ctrls_tap_state = {
    .is_press_action = true,
    .state = 0
};

void ctrls_finished (qk_tap_dance_state_t *state, void *user_data) {
    ctrls_tap_state.state = non_delay_hold_dance(state);
    switch (ctrls_tap_state.state) {
        case SINGLE_TAP:
            _register_code(KC_ESC);
            break;
        case SINGLE_HOLD: 
            _register_code(KC_LCTL);
            break;
        case DOUBLE_HOLD: 
            _register_code(KC_LCTL);
            _register_code(KC_LSFT);
            break;
    }
}

void ctrls_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (ctrls_tap_state.state==SINGLE_TAP) {
        _unregister_code(KC_ESC);
    } else if (ctrls_tap_state.state==SINGLE_HOLD) {
        _unregister_code(KC_LCTL);
    } else if (ctrls_tap_state.state == DOUBLE_HOLD) {
        _unregister_code(KC_LCTL);
        _unregister_code(KC_LSFT);
    }
    ctrls_tap_state.state = 0;
}

static tap alt_num_tap_state = {
    .is_press_action = true,
    .state = 0
};

void alt_num_finished (qk_tap_dance_state_t *state, void *user_data) {
    alt_num_tap_state.state = non_delay_hold_dance(state);
    switch (alt_num_tap_state.state) {
    	case SINGLE_TAP:
        case SINGLE_HOLD:
            _register_code(KC_LALT);
            break;
        case DOUBLE_HOLD: 
            layer_on(_ALTNUM);
            break;
        case TRIPLE_HOLD: 
            _register_code(KC_LALT);
            layer_on(_FN);
            break;
    }
}

void alt_num_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (alt_num_tap_state.state == SINGLE_HOLD || alt_num_tap_state.state == SINGLE_TAP) {
        _unregister_code(KC_LALT);
    } else if (alt_num_tap_state.state == DOUBLE_HOLD) {
        layer_off(_ALTNUM);
    } else if (alt_num_tap_state.state == TRIPLE_HOLD) {
        _unregister_code(KC_LALT);
        layer_off(_FN);
    }
    alt_num_tap_state.state = 0;
}

static tap guialt_tap_state = {
    .is_press_action = true,
    .state = 0
};

void guialt_finished (qk_tap_dance_state_t *state, void *user_data) {
    guialt_tap_state.state = non_delay_hold_dance(state);
    switch (guialt_tap_state.state) {
        case SINGLE_TAP: 
        case SINGLE_HOLD: 
            _register_code(KC_LGUI);
            break;
        case DOUBLE_HOLD: 
            _register_code(KC_LGUI);
            _register_code(KC_LALT);
            break;
        case TRIPLE_HOLD:
            _register_code(KC_LGUI);
            _register_code(KC_LALT);
            _register_code(KC_LCTL);
    }
}

void guialt_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (guialt_tap_state.state==SINGLE_HOLD || guialt_tap_state.state == SINGLE_TAP) {
        _unregister_code(KC_LGUI);
    } else if (guialt_tap_state.state == DOUBLE_HOLD) {
        _unregister_code(KC_LGUI);
        _unregister_code(KC_LALT);
    } else if (guialt_tap_state.state == TRIPLE_HOLD) {
        _unregister_code(KC_LGUI);
        _unregister_code(KC_LALT);
        _unregister_code(KC_LCTL);
    }

    guialt_tap_state.state = 0;
}

void _register_code(uint8_t code)
{
#ifdef CONSOLE_ENABLE
    /* uprintf("reg: %d\n", code); */
#endif
    register_code(code);
}

void _unregister_code(uint8_t code)
{
#ifdef CONSOLE_ENABLE
    uprintf("unreg: %d\n", code);
#endif
    unregister_code(code);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
#endif 
    switch (keycode) {
        case KC_PDEREF:
            if (!record->event.pressed) {
                SEND_STRING("->");
            }
            return false; 
        default:
            return true; 
    }
}
