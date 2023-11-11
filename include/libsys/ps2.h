#pragma once
#include <stdint.h>
#include <stdbool.h>

#define PS2_DATA *((volatile uint8_t*)0xFD00)
#define PS2_STATUS *((volatile uint8_t*)0xFD01)

#define PS2_STATUS_HAS_DATA 1
#define PS2_STATUS_RECV_VALID 2
#define PS2_STATUS_SEND_ACK 4

#define PS2_LED_SCROLL 1
#define PS2_LED_NUM 2
#define PS2_LED_CAPS 4

#define PS2_DELAY_250_MS (0 << 5)
#define PS2_DELAY_500_MS (1 << 5)
#define PS2_DELAY_750_MS (2 << 5)
#define PS2_DELAY_1000_MS (3 << 5)

// CPTS = characters per ten seconds
#define PS2_RATE_300_CPTS 0
#define PS2_RATE_267_CPTS 1
#define PS2_RATE_240_CPTS 2
#define PS2_RATE_218_CPTS 3
#define PS2_RATE_207_CPTS 4
#define PS2_RATE_185_CPTS 5
#define PS2_RATE_171_CPTS 6
#define PS2_RATE_160_CPTS 7
#define PS2_RATE_150_CPTS 8
#define PS2_RATE_133_CPTS 9
#define PS2_RATE_120_CPTS 10
#define PS2_RATE_109_CPTS 11
#define PS2_RATE_100_CPTS 12
#define PS2_RATE_92_CPTS 13
#define PS2_RATE_86_CPTS 14
#define PS2_RATE_80_CPTS 15
#define PS2_RATE_75_CPTS 16
#define PS2_RATE_67_CPTS 17
#define PS2_RATE_60_CPTS 18
#define PS2_RATE_55_CPTS 19
#define PS2_RATE_50_CPTS 20
#define PS2_RATE_46_CPTS 21
#define PS2_RATE_43_CPTS 22
#define PS2_RATE_40_CPTS 23
#define PS2_RATE_37_CPTS 24
#define PS2_RATE_33_CPTS 25
#define PS2_RATE_30_CPTS 26
#define PS2_RATE_27_CPTS 27
#define PS2_RATE_25_CPTS 28
#define PS2_RATE_23_CPTS 29
#define PS2_RATE_21_CPTS 30
#define PS2_RATE_20_CPTS 31

#define PS2_READ_EMPTY 0xFF
#define PS2_CODE_RESEND 0xFE
#define PS2_CODE_BAT_PASSED 0xAA
#define PS2_CODE_BAT_ERROR 0xFC
#define PS2_CODE_BAT_ERROR_2 0xFD
#define PS2_CODE_ACK 0xFA
#define PS2_CODE_BUFFER_OVERRUN 0x00

uint8_t ps2_read(void);
bool ps2_reset(void);
void ps2_set_led_mask(uint8_t mask);
void ps2_set_rate(uint8_t mask);
