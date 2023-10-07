#include <libsys/ps2keyboard.h>
#include <libsys/ps2.h>
#include <stdbool.h>

static bool extended = false;
static uint8_t release = 0;

static const uint8_t scancode[256] = {
    [0x00] = 0,
    PS2_KEY_F9,
    0,
    PS2_KEY_F5,
    PS2_KEY_F3,
    PS2_KEY_F1,
    PS2_KEY_F2,
    PS2_KEY_F12,

    [0x08] = 0,
    PS2_KEY_F10,
    PS2_KEY_F8,
    PS2_KEY_F6,
    PS2_KEY_F4,
    PS2_KEY_TAB,
    PS2_KEY_BACKQUOTE,
    0,

    [0x10] = 0,
    PS2_KEY_LALT,
    PS2_KEY_LSHIFT,
    0,
    PS2_KEY_LCTRL,
    PS2_KEY_Q,
    PS2_KEY_1,
    0,

    [0x18] = 0,
    0,
    PS2_KEY_Z,
    PS2_KEY_S,
    PS2_KEY_A,
    PS2_KEY_W,
    PS2_KEY_2,
    0,

    [0x20] = 0,
    PS2_KEY_C,
    PS2_KEY_X,
    PS2_KEY_D,
    PS2_KEY_E,
    PS2_KEY_4,
    PS2_KEY_3,
    0,

    [0x28] = 0,
    PS2_KEY_SPACE,
    PS2_KEY_V,
    PS2_KEY_F,
    PS2_KEY_T,
    PS2_KEY_R,
    PS2_KEY_5,
    0,

    [0x30] = 0,
    PS2_KEY_N,
    PS2_KEY_B,
    PS2_KEY_H,
    PS2_KEY_G,
    PS2_KEY_Y,
    PS2_KEY_6,
    0,

    [0x38] = 0,
    0,
    PS2_KEY_M,
    PS2_KEY_J,
    PS2_KEY_U,
    PS2_KEY_7,
    PS2_KEY_8,
    0,

    [0x40] = 0,
    PS2_KEY_COMMA,
    PS2_KEY_K,
    PS2_KEY_I,
    PS2_KEY_O,
    PS2_KEY_0,
    PS2_KEY_9,
    0,

    [0x48] = 0,
    PS2_KEY_DOT,
    PS2_KEY_SLASH,
    PS2_KEY_L,
    PS2_KEY_SEMICOLON,
    PS2_KEY_P,
    PS2_KEY_DASH,
    0,

    [0x50] = 0,
    0,
    PS2_KEY_QUOTE,
    0,
    PS2_KEY_LBRACE,
    PS2_KEY_EQUALS,
    0,
    0,

    [0x58] = PS2_KEY_CAPSLOCK,
    PS2_KEY_RSHIFT,
    PS2_KEY_ENTER,
    PS2_KEY_RBRACE,
    0,
    0,
    0,
    0,

    [0x60] = 0,
    0,
    0,
    0,
    0,
    0,
    PS2_KEY_BACKSPACE,
    0,

    [0x68] = 0,
    PS2_KEY_NUM_1,
    0,
    PS2_KEY_NUM_4,
    PS2_KEY_NUM_7,
    0,
    0,
    0,

    [0x70] = PS2_KEY_NUM_0,
    PS2_KEY_NUM_DOT,
    PS2_KEY_NUM_2,
    PS2_KEY_NUM_5,
    PS2_KEY_NUM_6,
    PS2_KEY_NUM_8,
    PS2_KEY_ESCAPE,
    PS2_KEY_NUMLOCK,

    [0x78] = PS2_KEY_F11,
    PS2_KEY_NUM_ADD,
    PS2_KEY_NUM_3,
    PS2_KEY_NUM_SUB,
    PS2_KEY_NUM_MUL,
    PS2_KEY_NUM_9,
    PS2_KEY_SCROLLLOCK,
    0,

    [0x83] = PS2_KEY_F7,
};

static const uint8_t scancode_ext[256] = {
    [0x11] = PS2_KEY_RALT,
    [0x14] = PS2_KEY_RCTRL,
    [0x1f] = PS2_KEY_LWIN,
    [0x27] = PS2_KEY_RWIN,
    [0x2f] = PS2_KEY_MENU,
    [0x4a] = PS2_KEY_NUM_DIV,
    [0x5a] = PS2_KEY_NUM_ENTER,
    [0x69] = PS2_KEY_END,
    [0x6b] = PS2_KEY_LEFT,
    [0x6c] = PS2_KEY_HOME,
    [0x70] = PS2_KEY_INSERT,
    [0x71] = PS2_KEY_DELETE,
    [0x72] = PS2_KEY_DOWN,
    [0x74] = PS2_KEY_RIGHT,
    [0x75] = PS2_KEY_UP,
    [0x7a] = PS2_KEY_PAGEDOWN,
    [0x7d] = PS2_KEY_PAGEUP,
};

static const uint8_t ps2_keymap[128] = {
        [PS2_KEY_A] =          'a',
        [PS2_KEY_B] =          'b',
        [PS2_KEY_C] =          'c',
        [PS2_KEY_D] =          'd',
        [PS2_KEY_E] =          'e',
        [PS2_KEY_F] =          'f',
        [PS2_KEY_G] =          'g',
        [PS2_KEY_H] =          'h',
        [PS2_KEY_I] =          'i',
        [PS2_KEY_J] =          'j',
        [PS2_KEY_K] =          'k',
        [PS2_KEY_L] =          'l',
        [PS2_KEY_M] =          'm',
        [PS2_KEY_N] =          'n',
        [PS2_KEY_O] =          'o',
        [PS2_KEY_P] =          'p',
        [PS2_KEY_Q] =          'q',
        [PS2_KEY_R] =          'r',
        [PS2_KEY_S] =          's',
        [PS2_KEY_T] =          't',
        [PS2_KEY_U] =          'u',
        [PS2_KEY_V] =          'v',
        [PS2_KEY_W] =          'w',
        [PS2_KEY_X] =          'x',
        [PS2_KEY_Y] =          'y',
        [PS2_KEY_Z] =          'z',
        [PS2_KEY_0] =          '0',
        [PS2_KEY_1] =          '1',
        [PS2_KEY_2] =          '2',
        [PS2_KEY_3] =          '3',
        [PS2_KEY_4] =          '4',
        [PS2_KEY_5] =          '5',
        [PS2_KEY_6] =          '6',
        [PS2_KEY_7] =          '7',
        [PS2_KEY_8] =          '8',
        [PS2_KEY_9] =          '9',
        [PS2_KEY_BACKQUOTE] =  '`',
        [PS2_KEY_DASH] =       '-',
        [PS2_KEY_EQUALS] =     '=',
        [PS2_KEY_LBRACE] =     '[',
        [PS2_KEY_RBRACE] =     ']',
        [PS2_KEY_SEMICOLON] =  ';',
        [PS2_KEY_QUOTE] =      '\'',
        [PS2_KEY_BACKSLASH] =  '\\',
        [PS2_KEY_PIPE] =       '`',
        [PS2_KEY_COMMA] =      ',',
        [PS2_KEY_DOT] =        '.',
        [PS2_KEY_SLASH] =      '/',
        [PS2_KEY_SPACE] =      ' ',
        [PS2_KEY_NUM_DIV] =    '/',
        [PS2_KEY_NUM_MUL] =    '*',
        [PS2_KEY_NUM_SUB] =    '-',
        [PS2_KEY_NUM_ADD] =    '+',
        [PS2_KEY_NUM_DOT] =    '.',
        [PS2_KEY_NUM_0] =      '0',
        [PS2_KEY_NUM_1] =      '1',
        [PS2_KEY_NUM_2] =      '2',
        [PS2_KEY_NUM_3] =      '3',
        [PS2_KEY_NUM_4] =      '4',
        [PS2_KEY_NUM_5] =      '5',
        [PS2_KEY_NUM_6] =      '6',
        [PS2_KEY_NUM_7] =      '7',
        [PS2_KEY_NUM_8] =      '8',
        [PS2_KEY_NUM_9] =      '9',
};
static const uint8_t ps2_keymap_shift[128] = {
        [PS2_KEY_A] =          'A',
        [PS2_KEY_B] =          'B',
        [PS2_KEY_C] =          'C',
        [PS2_KEY_D] =          'D',
        [PS2_KEY_E] =          'E',
        [PS2_KEY_F] =          'F',
        [PS2_KEY_G] =          'G',
        [PS2_KEY_H] =          'H',
        [PS2_KEY_I] =          'I',
        [PS2_KEY_J] =          'J',
        [PS2_KEY_K] =          'K',
        [PS2_KEY_L] =          'L',
        [PS2_KEY_M] =          'M',
        [PS2_KEY_N] =          'N',
        [PS2_KEY_O] =          'O',
        [PS2_KEY_P] =          'P',
        [PS2_KEY_Q] =          'Q',
        [PS2_KEY_R] =          'R',
        [PS2_KEY_S] =          'S',
        [PS2_KEY_T] =          'T',
        [PS2_KEY_U] =          'U',
        [PS2_KEY_V] =          'V',
        [PS2_KEY_W] =          'W',
        [PS2_KEY_X] =          'X',
        [PS2_KEY_Y] =          'Y',
        [PS2_KEY_Z] =          'Z',
        [PS2_KEY_0] =          ')',
        [PS2_KEY_1] =          '!',
        [PS2_KEY_2] =          '@',
        [PS2_KEY_3] =          '#',
        [PS2_KEY_4] =          '$',
        [PS2_KEY_5] =          '%',
        [PS2_KEY_6] =          '^',
        [PS2_KEY_7] =          '&',
        [PS2_KEY_8] =          '*',
        [PS2_KEY_9] =          '(',
        [PS2_KEY_BACKQUOTE] =  '~',
        [PS2_KEY_DASH] =       '_',
        [PS2_KEY_EQUALS] =     '+',
        [PS2_KEY_LBRACE] =     '{',
        [PS2_KEY_RBRACE] =     '}',
        [PS2_KEY_SEMICOLON] =  ':',
        [PS2_KEY_QUOTE] =      '"',
        [PS2_KEY_BACKSLASH] =  '|',
        [PS2_KEY_PIPE] =       '~',
        [PS2_KEY_COMMA] =      '<',
        [PS2_KEY_DOT] =        '>',
        [PS2_KEY_SLASH] =      '?',
        [PS2_KEY_SPACE] =      ' ',
        [PS2_KEY_NUM_DIV] =    '/',
        [PS2_KEY_NUM_MUL] =    '*',
        [PS2_KEY_NUM_SUB] =    '-',
        [PS2_KEY_NUM_ADD] =    '+',
        [PS2_KEY_NUM_DOT] =    '.',
        [PS2_KEY_NUM_0] =      '0',
        [PS2_KEY_NUM_1] =      '1',
        [PS2_KEY_NUM_2] =      '2',
        [PS2_KEY_NUM_3] =      '3',
        [PS2_KEY_NUM_4] =      '4',
        [PS2_KEY_NUM_5] =      '5',
        [PS2_KEY_NUM_6] =      '6',
        [PS2_KEY_NUM_7] =      '7',
        [PS2_KEY_NUM_8] =      '8',
        [PS2_KEY_NUM_9] =      '9',
};

uint8_t ps2_modifiers_mask = 0;

uint8_t ps2_wait_key_pressed(void) {
     uint8_t result = PS2_KEY_NONE;
     while (!result || (result & PS2_KEY_RELEASE)) {
          result = ps2_get_key_event();
     }
     return result;
}

uint8_t ps2_get_key_event(void) {
    uint8_t code = ps2_read();
    uint8_t result = PS2_KEY_NONE;
    if (code != PS2_READ_EMPTY) {
        if (code == PS2_CODE_BAT_PASSED || code == PS2_CODE_BUFFER_OVERRUN) {
            // TODO handle BAT error
            extended = false;
            release = 0;
        } else if (code == 0xf0) {
            release = 0x80;
        } else if (code == 0xe0 || code == 0xe1) {
            extended = true;
        } else {
            if (extended) {
                result = scancode_ext[code];
            } else {
                result = scancode[code];
            }
        }
    }
    if (result) {
        uint8_t mask = 0;
        if (result == PS2_KEY_RSHIFT || result == PS2_KEY_LSHIFT) {
            mask = PS2_MASK_SHIFT;
        } else if (result == PS2_KEY_LCTRL || result == PS2_KEY_RCTRL) {
            mask = PS2_MASK_CTRL;
        } else if (result == PS2_KEY_LALT || result == PS2_KEY_RALT) {
            mask = PS2_MASK_ALT;
        } else if (result == PS2_KEY_LWIN || result == PS2_KEY_RWIN) {
            mask = PS2_MASK_WIN;
        }
        if (release) {
            ps2_modifiers_mask &= ~mask;
        } else {
            ps2_modifiers_mask |= mask;
        }
        result |= release;
        release = 0;
        extended = false;
        return result;
    } else {
        return 0;
    }
}

uint16_t ps2_get_ascii(void) {
    uint8_t event = ps2_get_key_event();
    if (!event || (event & PS2_KEY_RELEASE)) {
        return 0;
    }
    if (ps2_modifiers_mask & (PS2_MASK_WIN | PS2_MASK_ALT | PS2_MASK_CTRL)) {
        return PS2_ASCII_SCANCODE_MASK | event;
    } else {
        uint8_t key;
        if (ps2_modifiers_mask & PS2_MASK_SHIFT) {
            key = ps2_keymap_shift[event];
        } else {
            key = ps2_keymap[event];
        }
        if (key) {
            return key;
        } else {
            return PS2_ASCII_SCANCODE_MASK | event;
        }
    }
}


