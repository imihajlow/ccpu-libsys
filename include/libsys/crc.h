#pragma once
#include <stdbool.h>
#include <stdint.h>

// Checks if last 4 bytes of the range are its correct CRC.
bool crc_check(const void *start, const void *end);

// Updates CRC with the range. To finalize, the result must be negated.
uint32_t crc_update(uint32_t crc, const void *start, const void *end);

// Updates CRC with a block of zeroes. To finalize, the result must be negated.
uint32_t crc_update_zeroes(uint32_t crc, uint16_t n_zeroes);
