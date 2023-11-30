#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SPI_DATA (*(volatile uint8_t*)0xFD02)
#define SPI_CTRL (*(volatile uint8_t*)0xFD03)

#define SPI_CTRL_N_CARD_DETECT 1
#define SPI_CTRL_WRITE_PROTECT 2
#define SPI_CTRL_N_CARD_CS 4
#define SPI_CTRL_EN_3V3 8

#define CARD_SUCCESS 0
#define CARD_ERROR_TIMEOUT 1
#define CARD_ERROR_RESET_FAILED 2
#define CARD_ERROR_ACMD_FAILED 3
#define CARD_ERROR_ACMD41_FAILED 4
#define CARD_ERROR_BAD_DATA_RESPONSE 5
#define CARD_ERROR_WRITE_ERROR 6
#define CARD_ERROR_CRC_ERROR 7
#define CARD_ERROR_NO_CARD 8
#define CARD_ERROR_NOT_INITIALIZED 9
#define CARD_ERROR_LOCKED 10
#define CARD_ERROR_OUT_OF_RANGE 11
#define CARD_ERROR_BLOCK_CMD_FAILED 12

#define CARD_BLOCK_SIZE 512

extern uint8_t cache_block[CARD_BLOCK_SIZE];

#define CARD_IS_PRESENT() (!(SPI_CTRL & SPI_CTRL_N_CARD_DETECT))
#define CARD_POWER_OFF() do { SPI_CTRL = SPI_CTRL_N_CARD_CS; } while (0)

uint8_t card_init(void);
uint8_t card_read_block(uint32_t block_index);
uint8_t card_write_block(uint32_t block_index);

