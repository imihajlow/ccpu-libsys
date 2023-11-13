#pragma once
#include <stdbool.h>
#include <stdint.h>

// Checks if the range ends with its correct CRC and 0 or 1 extra byte.
bool crc_check_relaxed(const void *start, const void *end);

// Updates CRC with the range. To finalize, the result must be negated.
uint32_t crc_update(uint32_t crc, const void *start, const void *end);

// Updates CRC with a block of zeroes. To finalize, the result must be negated.
uint32_t crc_update_zeroes(uint32_t crc, uint16_t n_zeroes);

extern uint8_t crc_table_0[256];
extern uint8_t crc_table_1[256];
extern uint8_t crc_table_2[256];
extern uint8_t crc_table_3[256];
