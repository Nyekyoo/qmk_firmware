#include QMK_KEYBOARD_H

#include "oneshot.h"

#define LA_SYM MO(SYM)
#define LA_MOD MO(MOD)
#define LA_NUM MO(NUM)

#define HSV_NYEKYOO_RED    240, 232, 120
#define HSV_NYEKYOO_PURPLE 207, 232, 120
#define HSV_NYEKYOO_BLUE   149, 255, 120
#define HSV_NYEKYOO_YELLOW  46, 255, 120
#define HSV_NYEKYOO_GREEN   97, 255, 80
#define HSV_NYEKYOO_GREEN_ACCENT   110, 255, 120

#define OS_CTRL_RGB_INDEX 53
#define OS_ALT_RGB_INDEX 52
#define OS_WIN_RGB_INDEX 51
#define OS_SHFT_RGB_INDEX 50

#define UP_RGB_INDEX 11
#define LEFT_RGB_INDEX 17
#define DOWN_RGB_INDEX 18
#define RIGHT_RGB_INDEX 19

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_win_state = os_up_unqueued;

enum layers {
    DEF,
    SYM,
    MOD,
    NUM,
};

enum keycodes {
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_WIN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT(
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                               KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       XXXXXXX,    
        KC_TAB,     KC_B,       KC_Y,       KC_O,       KC_U,       KC_QUOT,                            KC_SCLN,    KC_L,       KC_D,       KC_W,       KC_V,       KC_Z, 
        KC_ESC,     KC_C,       KC_I,       KC_E,       KC_A,       KC_COMM,                            KC_DOT,     KC_H,       KC_T,       KC_S,       KC_N,       KC_Q, 
        KC_LSFT,    KC_G,       KC_X,       KC_J,       KC_K,       KC_MINS,    KC_DEL,     XXXXXXX,    KC_SLSH,    KC_R,       KC_M,       KC_F,       KC_P,       KC_RSFT, 
                                                        LA_MOD,     KC_SPACE,   KC_BSPC,    LA_SYM,     KC_ENTER,   LA_NUM
    ),

    [SYM] = LAYOUT(
        QK_BOOT,    KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                               KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       XXXXXXX,    
        XXXXXXX,    KC_QUOT,    KC_LT,      KC_GT,      KC_DQUO,    KC_GRV,                             KC_TILD,    KC_LBRC,    KC_RBRC,    KC_AMPR,    KC_PERC,    XXXXXXX, 
        XXXXXXX,    KC_EXLM,    KC_MINS,    KC_PLUS,    KC_EQL,     KC_HASH,                            KC_PIPE,    KC_LPRN,    KC_RPRN,    KC_COLN,    KC_SCLN,    KC_DLR, 
        XXXXXXX,    KC_CIRC,    KC_SLASH,   KC_ASTR,    KC_BSLS,    KC_DOT,     KC_DEL,     XXXXXXX,    KC_UNDS,    KC_LCBR,    KC_RCBR,    KC_QUES,    KC_AT,      XXXXXXX, 
                                                        XXXXXXX,    KC_SPACE,   KC_BSPC,    XXXXXXX,    KC_ENTER,   XXXXXXX
    ),

    [MOD] = LAYOUT(
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,                            XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    
        XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_UP,      XXXXXXX,    XXXXXXX,                            XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
        XXXXXXX,    XXXXXXX,    KC_LEFT,    KC_DOWN,    KC_RIGHT,   XXXXXXX,                            XXXXXXX,    OS_CTRL,    OS_ALT,     OS_WIN,     OS_SHFT,    XXXXXXX, 
        XXXXXXX,    LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    LSFT(KC_TAB), KC_LALT,  KC_TAB,     KC_LWIN,    XXXXXXX, 
                                                        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
    ),

    [NUM] = LAYOUT(
        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,                            XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    
        XXXXXXX,    XXXXXXX,    KC_KP_1,    KC_KP_2,    KC_KP_3,    KC_NUM,                             XXXXXXX,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      XXXXXXX, 
        XXXXXXX,    XXXXXXX,    KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_KP_0,                            XXXXXXX,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      XXXXXXX, 
        XXXXXXX,    XXXXXXX,    KC_KP_7,    KC_KP_8,    KC_KP_9,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_F9,      KC_F10,     KC_F11,     KC_F12,     XXXXXXX, 
                                                        XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX
    ),
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_t green_accent = hsv_to_rgb((hsv_t){HSV_NYEKYOO_GREEN_ACCENT});
    rgb_t blue = hsv_to_rgb((hsv_t){HSV_NYEKYOO_BLUE});
    rgb_t yellow = hsv_to_rgb((hsv_t){HSV_NYEKYOO_YELLOW});
    rgb_t green = hsv_to_rgb((hsv_t){HSV_NYEKYOO_GREEN});
    rgb_t red = hsv_to_rgb((hsv_t){HSV_NYEKYOO_RED});

    for (uint8_t i = led_min; i < led_max; i++) {
        if (
            (i == OS_CTRL_RGB_INDEX && os_ctrl_state != os_up_unqueued) || 
            (i == OS_ALT_RGB_INDEX && os_alt_state != os_up_unqueued) ||
            (i == OS_WIN_RGB_INDEX && os_win_state != os_up_unqueued) ||
            (i == OS_SHFT_RGB_INDEX && os_shft_state != os_up_unqueued)
        ) {
            rgb_matrix_set_color(i, yellow.r, yellow.g, yellow.b);
            continue;
        }

        switch(get_highest_layer(layer_state|default_layer_state)) {
            case NUM:
                rgb_matrix_set_color(i, red.r, red.g, red.b);
                break;
            case MOD:
                if (
                    (i == UP_RGB_INDEX) ||
                    (i == LEFT_RGB_INDEX) ||
                    (i == DOWN_RGB_INDEX) ||
                    (i == RIGHT_RGB_INDEX) ||
                    (i == OS_CTRL_RGB_INDEX) ||
                    (i == OS_ALT_RGB_INDEX) ||
                    (i == OS_WIN_RGB_INDEX) ||
                    (i == OS_SHFT_RGB_INDEX)
                ) {
                    rgb_matrix_set_color(i, green_accent.r, green_accent.g, green_accent.b);
                    break;
                }
                rgb_matrix_set_color(i, green.r, green.g, green.b);
                break;
            case SYM:
                rgb_matrix_set_color(i, blue.r, blue.g, blue.b);
                break;
            default:
                break;
        }
    }
    return false;
}

void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_NYEKYOO_PURPLE);
}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_MOD:
    case LA_NUM:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_MOD:
    case LA_NUM:
    case KC_LSFT:
    case KC_RSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_WIN:
        return true;
    default:
        return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_win_state, KC_LWIN, OS_WIN,
        keycode, record
    );

    return true;
}
