/* Copyright 2016 Jack Humbert
 *
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

#include "print.h"
#include "process_combo.h"
#include "action_tapping.h"


#ifdef COMBO_COUNT
__attribute__((weak)) combo_t  key_combos[COMBO_COUNT];
uint16_t COMBO_LEN = COMBO_COUNT;
#else
extern combo_t  key_combos[];
extern uint16_t COMBO_LEN;
#endif

__attribute__((weak)) void process_combo_event(uint16_t combo_index, bool pressed) {}

#ifdef COMBO_MUST_HOLD_PER_COMBO
__attribute__((weak)) bool get_combo_must_hold(uint16_t index, combo_t *combo) { return false; }
#endif

<<<<<<< HEAD
#ifdef COMBO_TERM_PER_COMBO
__attribute__((weak)) uint16_t get_combo_term(uint16_t index, combo_t *combo) { return COMBO_TERM; }
#endif

#ifdef COMBO_PROCESS_KEY_RELEASE
__attribute__((weak)) bool process_combo_key_release(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) { return false; }
#endif

=======
#ifdef COMBO_MUST_TAP_PER_COMBO
__attribute__((weak)) bool get_combo_must_tap(uint16_t index, combo_t *combo) { return false; }
#endif

#ifdef COMBO_TERM_PER_COMBO
__attribute__((weak)) uint16_t get_combo_term(uint16_t index, combo_t *combo) { return COMBO_TERM; }
#endif

#ifdef COMBO_PROCESS_KEY_RELEASE
__attribute__((weak)) bool process_combo_key_release(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) { return false; }
#endif

>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
#ifndef COMBO_NO_TIMER
static uint16_t timer                 = 0;
#endif
static bool     b_combo_enable        = true;  // defaults to enabled
static uint16_t longest_term          = 0;

typedef struct {
    keyrecord_t record;
    uint16_t combo_index;
    uint16_t keycode;
} queued_record_t;
static uint8_t key_buffer_size = 0;
static queued_record_t key_buffer[COMBO_KEY_BUFFER_LENGTH];

typedef struct {
    uint16_t combo_index;
} queued_combo_t;
static uint8_t combo_buffer_write= 0;
static uint8_t combo_buffer_read = 0;
static queued_combo_t combo_buffer[COMBO_BUFFER_LENGTH];

#define INCREMENT_MOD(i) i = (i + 1) % COMBO_BUFFER_LENGTH

#define COMBO_KEY_POS ((keypos_t){.col=254, .row=254})

<<<<<<< HEAD
static inline void release_combo(uint16_t combo_index, combo_t *combo) {
    if (combo->keycode) {
        keyrecord_t record = {
            .event = {
                .key = COMBO_KEY_POS,
                .time = timer_read()|1,
                .pressed = false,
            },
            .keycode = combo->keycode,
        };
#ifndef NO_ACTION_TAPPING
        action_tapping_process(record);
#else
        process_record(record);
=======

#ifndef EXTRA_SHORT_COMBOS
/* flags are their own elements in combo_t struct. */
#    define COMBO_ACTIVE(combo)   (combo->active)
#    define COMBO_DISABLED(combo) (combo->disabled)
#    define COMBO_STATE(combo)    (combo->state)

#    define ACTIVATE_COMBO(combo)    do {combo->active = true;}while(0)
#    define DEACTIVATE_COMBO(combo)  do {combo->active = false;}while(0)
#    define DISABLE_COMBO(combo)     do {combo->disabled = true;}while(0)
#    define RESET_COMBO_STATE(combo) do { \
    combo->disabled = false; \
    combo->state = 0; \
}while(0)
#else
/* flags are at the two high bits of state. */
#    define COMBO_ACTIVE(combo)   (combo->state & 0x80)
#    define COMBO_DISABLED(combo) (combo->state & 0x40)
#    define COMBO_STATE(combo)    (combo->state & 0x3F)

#    define ACTIVATE_COMBO(combo)    do {combo->state |= 0x80;}while(0)
#    define DEACTIVATE_COMBO(combo)  do {combo->state &= ~0x80;}while(0)
#    define DISABLE_COMBO(combo)     do {combo->state |= 0x40;}while(0)
#    define RESET_COMBO_STATE(combo) do {combo->state &= ~0x7F;}while(0)
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
#endif
    } else {
        process_combo_event(combo_index, false);
    }
    combo->active = false;
}

<<<<<<< HEAD
=======
static inline void release_combo(uint16_t combo_index, combo_t *combo) {
    if (combo->keycode) {
        keyrecord_t record = {
            .event = {
                .key = COMBO_KEY_POS,
                .time = timer_read()|1,
                .pressed = false,
            },
            .keycode = combo->keycode,
        };
#ifndef NO_ACTION_TAPPING
        action_tapping_process(record);
#else
        process_record(&record);
#endif
    } else {
        process_combo_event(combo_index, false);
    }
    DEACTIVATE_COMBO(combo);
}

>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
static inline bool _get_combo_must_hold(uint16_t combo_index, combo_t *combo) {
#ifdef COMBO_NO_TIMER
    return false;
#elif defined(COMBO_MUST_HOLD_PER_COMBO)
    return get_combo_must_hold(combo_index, combo);
#elif defined(COMBO_MUST_HOLD_MODS)
<<<<<<< HEAD
    return KEYCODE_IS_MOD(combo->keycode);
=======
    return (KEYCODE_IS_MOD(combo->keycode) ||
            (combo->keycode >= QK_MOMENTARY && combo->keycode <= QK_MOMENTARY_MAX));
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
#endif
    return false;
}

<<<<<<< HEAD
=======
static inline uint16_t _get_wait_time(uint16_t combo_index, combo_t *combo ) {
    if (_get_combo_must_hold(combo_index, combo)
#ifdef COMBO_MUST_TAP_PER_COMBO
            || get_combo_must_tap(combo_index, combo)
#endif
       ) {
        if (longest_term < COMBO_HOLD_TERM) {
            return COMBO_HOLD_TERM;
        }
    }

    return longest_term;
}

>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
static inline uint16_t _get_combo_term(uint16_t combo_index, combo_t *combo) {

#if defined(COMBO_TERM_PER_COMBO)
        return get_combo_term(combo_index, combo);
<<<<<<< HEAD
#else
        if (_get_combo_must_hold(combo_index, combo)) {
            return COMBO_MOD_TERM;
        }
=======
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
#endif

    return COMBO_TERM;
}

void clear_combos(void) {
    uint16_t index = 0;
    longest_term = 0;
    for (index = 0; index < COMBO_LEN; ++index) {
        combo_t *combo = &key_combos[index];
<<<<<<< HEAD
        if (!combo->active) {
            combo->disabled = false;
            combo->state = 0;
=======
        if (!COMBO_ACTIVE(combo)) {
            RESET_COMBO_STATE(combo);
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
        }
    }
}

static inline void dump_key_buffer(void) {
    if (key_buffer_size == 0) {
        return;
    }

    for (uint8_t key_buffer_i = 0; key_buffer_i < key_buffer_size; key_buffer_i++) {

        queued_record_t *qrecord = &key_buffer[key_buffer_i];
        keyrecord_t *record = &qrecord->record;

        if (IS_NOEVENT(record->event)) {
            continue;
        }

        if (!record->keycode && qrecord->combo_index != (uint16_t)-1) {
            process_combo_event(qrecord->combo_index, true);
        } else {
#ifndef NO_ACTION_TAPPING
        action_tapping_process(*record);
#else
        process_record(record);
#endif
        }
        record->event.time = 0;
<<<<<<< HEAD
=======
        clear_weak_mods();
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
    }

    key_buffer_size = 0;
}

<<<<<<< HEAD
#define NO_COMBO_KEYS_ARE_DOWN (0 == combo->state)
=======
#define NO_COMBO_KEYS_ARE_DOWN (0 == COMBO_STATE(combo))
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
#define ALL_COMBO_KEYS_ARE_DOWN(state, key_count) (((1 << key_count) - 1) == state)
#define ONLY_ONE_KEY_IS_DOWN(state) !(state & (state - 1))
#define KEY_NOT_YET_RELEASED(state, key_index) ((1 << key_index) & state)
#define KEY_STATE_DOWN(state, key_index) \
    do {                                 \
        state |= (1 << key_index);       \
    } while (0)
#define KEY_STATE_UP(state, key_index) \
    do {                               \
        state &= ~(1 << key_index);    \
    } while (0)

static inline void _find_key_index_and_count(const uint16_t *keys, uint16_t keycode, uint16_t *key_index, uint8_t *key_count) {
    while (true) {
        uint16_t key = pgm_read_word(&keys[*key_count]);
        if (keycode == key) *key_index = *key_count;
        if (COMBO_END == key) break;
        (*key_count)++;
<<<<<<< HEAD
    }
}

void drop_combo_from_buffer(uint16_t combo_index) {
    /* Mark a combo as processed from the buffer. I the buffer is in the
     * beginning of the buffer, drop it.  */
    uint8_t i = combo_buffer_read;
    while (i != combo_buffer_write) {
        queued_combo_t *qcombo = &combo_buffer[i];

        if (qcombo->combo_index == combo_index) {
            combo_t *combo = &key_combos[combo_index];
            combo->disabled = true;

            if (i == combo_buffer_read) {
                INCREMENT_MOD(combo_buffer_read);
            }
            break;
        }
        INCREMENT_MOD(i);
    }
}

void apply_combo(uint16_t combo_index, combo_t *combo) {
    /* Apply combo's result keycode to the last chord key of the combo and
     * disable the other keys. */

    // state to check against so we find the last key of the combo from the buffer
#if defined(EXTRA_EXTRA_LONG_COMBOS)
    uint32_t state = 0;
#elif defined(EXTRA_LONG_COMBOS)
    uint16_t state = 0;
#else
    uint8_t state = 0;
#endif

    for (uint8_t key_buffer_i = 0; key_buffer_i < key_buffer_size; key_buffer_i++) {

        queued_record_t *qrecord = &key_buffer[key_buffer_i];
        keyrecord_t *record = &qrecord->record;
        uint16_t keycode = qrecord->keycode;

        combo_t *combo = &key_combos[combo_index];
        if (combo->disabled) {
            continue;
        }

        uint8_t key_count = 0;
        uint16_t key_index = -1;
        _find_key_index_and_count(combo->keys, keycode, &key_index, &key_count);

        if (-1 == (int16_t)key_index) {
            // key not part of this combo
            continue;
        }

        KEY_STATE_DOWN(state, key_index);
        if (ALL_COMBO_KEYS_ARE_DOWN(state, key_count)) {
            // this in the end executes the combo when the key_buffer is dumped.
            record->keycode = combo->keycode;
            record->event.key = COMBO_KEY_POS;

            qrecord->combo_index = combo_index;
            combo->active = true;
=======
    }
}

void drop_combo_from_buffer(uint16_t combo_index) {
    /* Mark a combo as processed from the buffer. If the buffer is in the
     * beginning of the buffer, drop it.  */
    uint8_t i = combo_buffer_read;
    while (i != combo_buffer_write) {
        queued_combo_t *qcombo = &combo_buffer[i];

        if (qcombo->combo_index == combo_index) {
            combo_t *combo = &key_combos[combo_index];
            DISABLE_COMBO(combo);

            if (i == combo_buffer_read) {
                INCREMENT_MOD(combo_buffer_read);
            }
            break;
        }
        INCREMENT_MOD(i);
    }
}

void apply_combo(uint16_t combo_index, combo_t *combo) {
    /* Apply combo's result keycode to the last chord key of the combo and
     * disable the other keys. */

    if (COMBO_DISABLED(combo)) { return; }

    // state to check against so we find the last key of the combo from the buffer
#if defined(EXTRA_EXTRA_LONG_COMBOS)
    uint32_t state = 0;
#elif defined(EXTRA_LONG_COMBOS)
    uint16_t state = 0;
#else
    uint8_t state = 0;
#endif

    for (uint8_t key_buffer_i = 0; key_buffer_i < key_buffer_size; key_buffer_i++) {

        queued_record_t *qrecord = &key_buffer[key_buffer_i];
        keyrecord_t *record = &qrecord->record;
        uint16_t keycode = qrecord->keycode;

        uint8_t key_count = 0;
        uint16_t key_index = -1;
        _find_key_index_and_count(combo->keys, keycode, &key_index, &key_count);

        if (-1 == (int16_t)key_index) {
            // key not part of this combo
            continue;
        }

        KEY_STATE_DOWN(state, key_index);
        if (ALL_COMBO_KEYS_ARE_DOWN(state, key_count)) {
            // this in the end executes the combo when the key_buffer is dumped.
            record->keycode = combo->keycode;
            record->event.key = COMBO_KEY_POS;

            qrecord->combo_index = combo_index;
            ACTIVATE_COMBO(combo);
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497

            break;
        } else {
            // key was part of the combo but not the last one, "disable" it
            // by making it a TICK event.
            record->event.time = 0;
        }

    }
    drop_combo_from_buffer(combo_index);
}

<<<<<<< HEAD
void apply_combos(void) {
=======
static inline void apply_combos(void) {
    // Apply all buffered normal combos.
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
    for (uint8_t i = combo_buffer_read;
            i != combo_buffer_write;
            INCREMENT_MOD(i)) {

        queued_combo_t *buffered_combo = &combo_buffer[i];
        combo_t *combo = &key_combos[buffered_combo->combo_index];

<<<<<<< HEAD
=======
#ifdef COMBO_MUST_TAP_PER_COMBO
        if (get_combo_must_tap(buffered_combo->combo_index, combo)) {
            // Tap-only combos are applied on key release only, so let's drop 'em here.
            drop_combo_from_buffer(buffered_combo->combo_index);
            continue;
        }
#endif
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
        apply_combo(buffered_combo->combo_index, combo);
    }
    dump_key_buffer();
    clear_combos();
}

combo_t* overlaps(combo_t *combo1, combo_t *combo2) {
    /* Checks if the combos overlap and returns the combo that should be
     * dropped from the combo buffer.
     * The combo that has less keys will be dropped. If they have the same
     * amount of keys, drop combo1. */

    uint8_t idx1 = 0, idx2 = 0;
    uint16_t key1, key2;
    bool overlaps = false;

    while ((key1 = pgm_read_word(&combo1->keys[idx1])) != COMBO_END) {
        idx2 = 0;
        while ((key2 = pgm_read_word(&combo2->keys[idx2])) != COMBO_END) {
            if (key1 == key2) overlaps = true;
            idx2 += 1;
        }
        idx1 += 1;
    }

    if (!overlaps) return NULL;
    if (idx2 < idx1) return combo2;
    return combo1;
}

static bool process_single_combo(combo_t *combo, uint16_t keycode, keyrecord_t *record, uint16_t combo_index) {
    uint8_t key_count = 0;
    uint16_t key_index = -1;
    _find_key_index_and_count(combo->keys, keycode, &key_index, &key_count);

    /* Continue processing if key isn't part of current combo. */
    if (-1 == (int16_t)key_index) {
        return false;
    }

<<<<<<< HEAD
    bool key_is_part_of_combo = !combo->disabled;

    if (record->event.pressed && !combo->disabled) {
        uint16_t time = _get_combo_term(combo_index, combo);
        if (!combo->active) {
=======
    bool key_is_part_of_combo = !COMBO_DISABLED(combo);

    if (record->event.pressed && !COMBO_DISABLED(combo)) {
        uint16_t time = _get_combo_term(combo_index, combo);
        if (!COMBO_ACTIVE(combo)) {
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
            KEY_STATE_DOWN(combo->state, key_index);
            if (longest_term < time) {
                longest_term = time;
            }
        }
<<<<<<< HEAD
        if (ALL_COMBO_KEYS_ARE_DOWN(combo->state, key_count)) {
=======
        if (ALL_COMBO_KEYS_ARE_DOWN(COMBO_STATE(combo), key_count)) {
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
            /* Combo was fully pressed */
            /* Buffer the combo so we can fire it after COMBO_TERM */

#ifndef COMBO_NO_TIMER
            /* Don't buffer this combo if its combo term has passed. */
            if (timer && timer_elapsed(timer) > time) {
<<<<<<< HEAD
                combo->disabled = true;
=======
                DISABLE_COMBO(combo);
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                return true;
            } else
#endif
            {

                // disable readied combos that overlap with this combo
                combo_t *drop = NULL;
                for (uint8_t combo_buffer_i = combo_buffer_read;
                        combo_buffer_i != combo_buffer_write;
                        INCREMENT_MOD(combo_buffer_i)) {

                    queued_combo_t *qcombo = &combo_buffer[combo_buffer_i];
                    combo_t *buffered_combo = &key_combos[qcombo->combo_index];

                    if ((drop = overlaps(buffered_combo, combo))) {
<<<<<<< HEAD
                        drop->disabled = true;
=======
                        DISABLE_COMBO(drop);
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                        if (drop == combo) {
                            // stop checking for overlaps if dropped combo was current combo.
                            break;
                        } else if (combo_buffer_i == combo_buffer_read && drop == buffered_combo) {
                            /* Drop the disabled buffered combo from the buffer if
                             * it is in the beginning of the buffer. */
                            INCREMENT_MOD(combo_buffer_read);
                        }
                    }

                }

                if (drop != combo) {
                    // save this combo to buffer
                    combo_buffer[combo_buffer_write] = (queued_combo_t){
                        .combo_index=combo_index,
                    };
                    INCREMENT_MOD(combo_buffer_write);
<<<<<<< HEAD
=======

                    // get possible longer waiting time for tap-/hold-only combos.
                    longest_term = _get_wait_time(combo_index, combo);
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                }
            } // if timer elapsed end

        }
    } else {
        // chord releases
<<<<<<< HEAD
        if (!combo->active && ALL_COMBO_KEYS_ARE_DOWN(combo->state, key_count)) {
            /* First key quickly released */
            if (combo->disabled || _get_combo_must_hold(combo_index, combo)) {
=======
        if (!COMBO_ACTIVE(combo) && ALL_COMBO_KEYS_ARE_DOWN(COMBO_STATE(combo), key_count)) {
            /* First key quickly released */
            if (COMBO_DISABLED(combo) || _get_combo_must_hold(combo_index, combo)) {
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                // combo wasn't tappable, disable it and drop it from buffer.
                drop_combo_from_buffer(combo_index);
                key_is_part_of_combo = false;
            }
<<<<<<< HEAD
        } else if (combo->active
                && ONLY_ONE_KEY_IS_DOWN(combo->state)
                && KEY_NOT_YET_RELEASED(combo->state, key_index)
=======
#ifdef COMBO_MUST_TAP_PER_COMBO
            else if (get_combo_must_tap(combo_index, combo)) {
                // immediately apply tap-only combo
                apply_combo(combo_index, combo);
                apply_combos(); // also apply other prepared combos and dump key buffer
#    ifdef COMBO_PROCESS_KEY_RELEASE
                if (process_combo_key_release(combo_index, combo, key_index, keycode)) {
                    release_combo(combo_index, combo);
                }
#    endif
            }
#endif
        } else if (COMBO_ACTIVE(combo)
                && ONLY_ONE_KEY_IS_DOWN(COMBO_STATE(combo))
                && KEY_NOT_YET_RELEASED(COMBO_STATE(combo), key_index)
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                ) {
            /* last key released */
            release_combo(combo_index, combo);
            key_is_part_of_combo = true;

#ifdef COMBO_PROCESS_KEY_RELEASE
            process_combo_key_release(combo_index, combo, key_index, keycode);
#endif
<<<<<<< HEAD
        } else if (combo->active
                && KEY_NOT_YET_RELEASED(combo->state, key_index)
=======
        } else if (COMBO_ACTIVE(combo)
                && KEY_NOT_YET_RELEASED(COMBO_STATE(combo), key_index)
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
                ) {
            /* first or middle key released */
            key_is_part_of_combo = true;

#ifdef COMBO_PROCESS_KEY_RELEASE
            if (process_combo_key_release(combo_index, combo, key_index, keycode)) {
                release_combo(combo_index, combo);
            }
#endif
        } else {
            /* The released key was part of an incomplete combo */
            key_is_part_of_combo = false;
        }

        KEY_STATE_UP(combo->state, key_index);
    }

    return key_is_part_of_combo;
}

bool process_combo(uint16_t keycode, keyrecord_t *record) {
    bool is_combo_key          = false;
    bool no_combo_keys_pressed = true;

    if (keycode == CMB_ON && record->event.pressed) {
        combo_enable();
        return true;
    }

    if (keycode == CMB_OFF && record->event.pressed) {
        combo_disable();
        return true;
    }

    if (keycode == CMB_TOG && record->event.pressed) {
        combo_toggle();
        return true;
    }

    if (!is_combo_enabled()) {
        return true;
    }

<<<<<<< HEAD
    for (uint16_t idx = 0; idx < COMBO_LEN; ++idx) {
        combo_t *combo = &key_combos[idx];
        is_combo_key |= process_single_combo(combo, keycode, record, idx);
        no_combo_keys_pressed = no_combo_keys_pressed && (NO_COMBO_KEYS_ARE_DOWN || combo->active || combo->disabled);
=======
#ifdef COMBO_ONLY_FROM_LAYER
    /* Only check keycodes from one layer. */
    keycode = keymap_key_to_keycode(COMBO_ONLY_FROM_LAYER, record->event.key);
#endif

    for (uint16_t idx = 0; idx < COMBO_LEN; ++idx) {
        combo_t *combo = &key_combos[idx];
        is_combo_key |= process_single_combo(combo, keycode, record, idx);
        no_combo_keys_pressed = no_combo_keys_pressed && (NO_COMBO_KEYS_ARE_DOWN || COMBO_ACTIVE(combo) || COMBO_DISABLED(combo));
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
    }

    if (record->event.pressed && is_combo_key) {
#ifndef COMBO_NO_TIMER
#   ifdef COMBO_STRICT_TIMER
        if (!timer) {
            // timer is set only on the first key
            timer = timer_read();
        }
#   else
        timer = timer_read();
#   endif
#endif

        if (key_buffer_size < COMBO_KEY_BUFFER_LENGTH) {
            key_buffer[key_buffer_size++] = (queued_record_t){
                .record = *record,
                .keycode = keycode,
                .combo_index = -1, // this will be set when applying combos
            };
        }
    } else {
        if (combo_buffer_read != combo_buffer_write) {
            // some combo is prepared
            apply_combos();
        } else {
            // reset state if there are no combo keys pressed at all
            dump_key_buffer();
#ifndef COMBO_NO_TIMER
            timer = 0;
#endif
            clear_combos();
        }
    }
    return !is_combo_key;
}

void matrix_scan_combo(void) {
    if (!b_combo_enable) {
        return;
    }

#ifndef COMBO_NO_TIMER
    if (timer && timer_elapsed(timer) > longest_term) {
        if (combo_buffer_read != combo_buffer_write) {
            apply_combos();
            longest_term = 0;
            timer = 0;
        } else {
            dump_key_buffer();
            timer = 0;
            clear_combos();
        }
    }
#endif
}

void combo_enable(void) { b_combo_enable = true; }

void combo_disable(void) {
#ifndef COMBO_NO_TIMER
    timer                      = 0;
#endif
    b_combo_enable = false;
    combo_buffer_read = combo_buffer_write;
<<<<<<< HEAD
    dump_key_buffer();
=======
>>>>>>> 9495ed61a7ca41c9479c3adf1e43f1ee7e37d497
}

void combo_toggle(void) {
    if (b_combo_enable) {
        combo_disable();
    } else {
        combo_enable();
    }
}

bool is_combo_enabled(void) { return b_combo_enable; }
