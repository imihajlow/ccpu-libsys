#pragma once
#include <stdint.h>
#include <stdbool.h>

#define PS2_KEY_RELEASE 0x80

#define PS2_KEY_NONE 0
#define PS2_KEY_A 'A'
#define PS2_KEY_B 'B'
#define PS2_KEY_C 'C'
#define PS2_KEY_D 'D'
#define PS2_KEY_E 'E'
#define PS2_KEY_F 'F'
#define PS2_KEY_G 'G'
#define PS2_KEY_H 'H'
#define PS2_KEY_I 'I'
#define PS2_KEY_J 'J'
#define PS2_KEY_K 'K'
#define PS2_KEY_L 'L'
#define PS2_KEY_M 'M'
#define PS2_KEY_N 'N'
#define PS2_KEY_O 'O'
#define PS2_KEY_P 'P'
#define PS2_KEY_Q 'Q'
#define PS2_KEY_R 'R'
#define PS2_KEY_S 'S'
#define PS2_KEY_T 'T'
#define PS2_KEY_U 'U'
#define PS2_KEY_V 'V'
#define PS2_KEY_W 'W'
#define PS2_KEY_X 'X'
#define PS2_KEY_Y 'Y'
#define PS2_KEY_Z 'Z'
#define PS2_KEY_0 '0'
#define PS2_KEY_1 '1'
#define PS2_KEY_2 '2'
#define PS2_KEY_3 '3'
#define PS2_KEY_4 '4'
#define PS2_KEY_5 '5'
#define PS2_KEY_6 '6'
#define PS2_KEY_7 '7'
#define PS2_KEY_8 '8'
#define PS2_KEY_9 '9'
#define PS2_KEY_BACKQUOTE '`'
#define PS2_KEY_DASH '-'
#define PS2_KEY_EQUALS '='
#define PS2_KEY_LBRACE '['
#define PS2_KEY_RBRACE ']'
#define PS2_KEY_SEMICOLON ';'
#define PS2_KEY_QUOTE '"'
#define PS2_KEY_BACKSLASH '\\'
#define PS2_KEY_PIPE '|'
#define PS2_KEY_COMMA ','
#define PS2_KEY_DOT '.'
#define PS2_KEY_SLASH '/'
#define PS2_KEY_SPACE ' '
#define PS2_KEY_ESCAPE 121
#define PS2_KEY_F1 1
#define PS2_KEY_F2 2
#define PS2_KEY_F3 3
#define PS2_KEY_F4 4
#define PS2_KEY_F5 5
#define PS2_KEY_F6 6
#define PS2_KEY_F7 7
#define PS2_KEY_F8 8
#define PS2_KEY_F9 9
#define PS2_KEY_F10 10
#define PS2_KEY_F11 11
#define PS2_KEY_F12 12
#define PS2_KEY_BACKSPACE 13
#define PS2_KEY_TAB 14
#define PS2_KEY_CAPSLOCK 15
#define PS2_KEY_ENTER 16
#define PS2_KEY_LSHIFT 17
#define PS2_KEY_RSHIFT 18
#define PS2_KEY_LCTRL 19
#define PS2_KEY_LALT 20
#define PS2_KEY_LWIN 21
#define PS2_KEY_RALT 22
#define PS2_KEY_RWIN 23
#define PS2_KEY_MENU 24
#define PS2_KEY_RCTRL 25
#define PS2_KEY_INSERT 26
#define PS2_KEY_DELETE 27
#define PS2_KEY_HOME 28
#define PS2_KEY_END 29
#define PS2_KEY_PAGEUP 30
#define PS2_KEY_PAGEDOWN 31
#define PS2_KEY_LEFT 97
#define PS2_KEY_RIGHT 98
#define PS2_KEY_UP 99
#define PS2_KEY_DOWN 100
#define PS2_KEY_NUMLOCK 101
#define PS2_KEY_NUM_DIV 102
#define PS2_KEY_NUM_MUL 103
#define PS2_KEY_NUM_SUB 104
#define PS2_KEY_NUM_ADD 105
#define PS2_KEY_NUM_ENTER 106
#define PS2_KEY_NUM_DOT 107
#define PS2_KEY_NUM_0 108
#define PS2_KEY_NUM_1 109
#define PS2_KEY_NUM_2 110
#define PS2_KEY_NUM_3 111
#define PS2_KEY_NUM_4 112
#define PS2_KEY_NUM_5 113
#define PS2_KEY_NUM_6 114
#define PS2_KEY_NUM_7 115
#define PS2_KEY_NUM_8 116
#define PS2_KEY_NUM_9 117
#define PS2_KEY_SCROLLLOCK 118
#define PS2_KEY_PAUSE 119
#define PS2_KEY_PRINT 120

#define PS2_MASK_CTRL 0x01
#define PS2_MASK_ALT 0x02
#define PS2_MASK_WIN 0x04
#define PS2_MASK_SHIFT 0x08

extern uint8_t ps2_modifiers_mask;

uint8_t ps2_get_key_event(void);
uint8_t ps2_wait_key_pressed(void);

#define PS2_ASCII_SCANCODE_MASK 0x8000u
#define PS2_IS_ASCII(x) (!(x & 0x8000u))
#define PS2_IS_SCANCODE(x) (x & 0x8000u)
uint16_t ps2_get_ascii(void);
