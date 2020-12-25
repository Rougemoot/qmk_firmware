/* Vermoot's ortho grid keymap */


/* --------------- BLANK LAYOUT -------------- {{{

[] = LAYOUT_ortho_4x12( \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
)

---------------------------------------------- }}} */

// --------------- DEFINITIONS --------------- {{{

// ---- Options ---- {{{

#include QMK_KEYBOARD_H

// Home row mods options
#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD
#define TAPPING_TERM 160

// ----------------- }}}

// ---- Enums ---- {{{

enum layers {
  _BASE,
  _RAISE,
  _LOWER,
  _NAV,
  _NUM,
  _MEDIA,
  _FUNCT
};

enum custom_keycodes {
  // Undead characters
  UD_APO = SAFE_RANGE,
  UD_GRV,
  UD_TLD,
  UD_CIRC,

  // Accented letters
  E_ACUTE,
  E_GRV,
  E_CIRC,
  A_GRV,
  U_GRV,
};

// --------------- }}}

// ---- Custom functions ---- {{{

bool undead(uint16_t  character, bool pressed) {
  if (pressed) {
    tap_code16(character);
    tap_code(KC_SPC);
  }
  return false;
}

bool accented_letter(uint16_t accent, uint16_t letter, bool pressed, uint8_t modifiers) {
  if (pressed) {          // On press:
    unregister_mods(MOD_MASK_SHIFT);    // Unpress shift if needed
    tap_code16(accent);                 // Tap the (dead) accent character
    register_mods(modifiers);           // Restore initial mod_state (so re-press shift if needed)
    register_code(letter);              // Press letter.
  } else {                              // On release: (this is for repeats on hold down)
    unregister_code(letter);            // Release letter
  }                                     // If shift is pressed it'll be released by the user
  return false;                         // Don't continue with the key handling.
}

// -------------------------- }}}

// ---- Custom keys ---- {{{

// Modtaps
#define ESC_FN MT(KC_APPLE_FN, KC_ESC)
#define TAB_MEH MEH_T(KC_TAB)
#define SFT_ENT RSFT_T(KC_ENT)

// Home row mods
#define SFT_A MT(KC_LSFT, KC_A)
#define SFT_SC MT(KC_RSFT, KC_SCLN)

// Layer keys
#define RAISE MO(_RAISE) 
#define LOWER MO(_LOWER) 
#define NAV   MO(_NAV) 
#define NUM   MO(_NUM) 
#define MEDIA MO(_MEDIA)
#define FUNCT  MO(_FUNCT) 

// Accented letters
#define C_CED   RALT(KC_C)
#define DED_CIR RALT(KC_I)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint8_t mod_state = get_mods();
  switch (keycode) {

    // Undead characters
    case UD_APO:
      return undead(KC_QUOT, record->event.pressed);
    case UD_GRV:
      return undead(KC_GRV, record->event.pressed);
    case UD_TLD:
      return undead(S(KC_GRV), record->event.pressed);
    case UD_CIRC:
      return undead(S(KC_6), record->event.pressed);

    // Accented letters
    case E_ACUTE:
      return accented_letter(KC_QUOT, KC_E, record->event.pressed, mod_state);
    case E_GRV:
      return accented_letter(KC_GRV, KC_E, record->event.pressed, mod_state);
    case E_CIRC:
      return accented_letter(S(KC_6), KC_E, record->event.pressed, mod_state);
    case A_GRV:
      return accented_letter(KC_GRV, KC_A, record->event.pressed, mod_state);
    case U_GRV:
      return accented_letter(KC_GRV, KC_U, record->event.pressed, mod_state);
    default:
      return true;

    break;
  }
}

// --------------------- }}}

// ------------------------------------------- }}}

// --------------- LAYERS -------------------- {{{

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// ---- BASE LAYER ---- {{{
[_BASE] = LAYOUT_ortho_4x12( \
  TAB_MEH, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MEDIA,
  ESC_FN,  SFT_A,   KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    SFT_SC,  UD_APO,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_ENT,
  KC_LCTL, KC_LALT, KC_LGUI, NUM,     NAV,     LOWER,   RAISE,   KC_SPC,  KC_BSPC, FUNCT,   _______, _______
),
// -------------------- }}}

// ---- RAISE ---- {{{
[_RAISE] = LAYOUT_ortho_4x12( \
  _______, A_GRV,   _______, E_ACUTE, _______, _______, _______, _______, DED_CIR, KC_UNDS, KC_PLUS, _______,
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, UD_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______, UD_GRV,  _______, C_CED,   _______, _______, _______, _______, KC_LCBR, KC_RCBR, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ 
),
// --------------- }}}

// ---- LOWER ---- {{{
[_LOWER] = LAYOUT_ortho_4x12( \
  _______, _______, _______, E_GRV,   _______, _______, _______, U_GRV,   E_CIRC,  KC_MINS, KC_EQL,  _______,
  _______, KC_1,    KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_BSLS,
  _______, UD_TLD,  _______, _______, _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),
// --------------- }}}

// ---- NAV ---- {{{
[_NAV] = LAYOUT_ortho_4x12( \
  _______, _______, _______, _______, _______, _______, _______, KC_PGDN, KC_PGUP, _______, _______, _______,
  _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, KC_ENT,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  _______, _______, _______
),
// --------------- }}}

// ---- NUM ---- {{{
[_NUM] = LAYOUT_ortho_4x12( \
  _______, _______, _______, _______, _______, _______, KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PMNS, KC_BSPC, \
  _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______, KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, _______, \
  _______, _______, _______, _______, _______, _______, KC_PEQL, KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______, \
  _______, _______, _______, _______, _______, _______, KC_SPC,  KC_P0,   KC_PCMM, KC_PDOT, _______, _______ \
),
// --------------- }}}

// ---- MEDIA ---- {{{
[_MEDIA] = LAYOUT_ortho_4x12( \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BRIU, KC_VOLU, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BRID, KC_VOLD, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MUTE, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MFFD, KC_MPLY, KC_MRWD \
),
// --------------- }}}

// ---- FUNCT ---- {{{
[_FUNCT] = LAYOUT_ortho_4x12( \
  KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  KC_F7,   KC_F8,   KC_F9,   _______, _______, _______, _______, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT, _______, \
  KC_F4,   KC_F5,   KC_F6,   _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  KC_F1,   KC_F2,   KC_F3,   _______, _______, _______, _______, _______, _______, _______, _______, _______ \
)
// --------------- }}}

};

// ------------------------------------------- }}}
