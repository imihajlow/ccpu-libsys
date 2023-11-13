#include <libsys/eth.h>
#include <string.h>
#include <strings.h>
#include <libsys/crc.h>
#include <libsys/vga.h>
#include <stdio.h>

#define TX_BUF_LEN 1024
#define PREAMBLE_LEN 12
#define FCS_LEN 4

static size_t last_frame_len;

extern uint8_t bitmix[256];

void eth_init(void) {
    memset(ETH_BUF, bitmix[0x55], PREAMBLE_LEN - 1);
    ETH_BUF[PREAMBLE_LEN - 1] = bitmix[0xd5];
    bzero(ETH_BUF + PREAMBLE_LEN, TX_BUF_LEN - PREAMBLE_LEN - FCS_LEN);
    last_frame_len = 0;
}

void eth_transmit(const void *data, size_t size) {
    uint8_t *cdata = data;
    for (uint16_t i = 0; i != size; ++i) {
        ETH_BUF[PREAMBLE_LEN + i] = bitmix[cdata[i]];
    }
    // memcpy(ETH_BUF + PREAMBLE_LEN, data, size);
    if (size < last_frame_len) {
        bzero(ETH_BUF + PREAMBLE_LEN + size, last_frame_len - size);
    }
    last_frame_len = size;

    uint32_t c = crc_update(0xffffffff, data, data + size);
    sprintf(VGA_CHAR_SEG + VGA_OFFSET(0, 0), "data crc = %08lX", c);
    c = crc_update_zeroes(c, TX_BUF_LEN - FCS_LEN - PREAMBLE_LEN - 1 - size);
    sprintf(VGA_CHAR_SEG + VGA_OFFSET(0, 1), "zero crc = %08lX", c);
    // uint8_t adj = 0xd9 ^ (uint8_t)c;
    uint8_t adj =c;
    sprintf(VGA_CHAR_SEG + VGA_OFFSET(0, 2), "adj = %02hX", adj);
    ETH_BUF[TX_BUF_LEN - 5] = bitmix[adj];
    c = crc_update(c, &adj, &adj + 1);
    sprintf(VGA_CHAR_SEG + VGA_OFFSET(0, 3), "total crc = %08lX", c);
    // *(uint32_t*)(ETH_BUF + TX_BUF_LEN - FCS_LEN) = c;
    ETH_BUF[TX_BUF_LEN - 4] = ~bitmix[(uint8_t)c];
    ETH_BUF[TX_BUF_LEN - 3] = ~bitmix[(uint8_t)(c >> 8)];
    ETH_BUF[TX_BUF_LEN - 2] = ~bitmix[(uint8_t)(c >> 16)];
    ETH_BUF[TX_BUF_LEN - 1] = 0xff;
    ETH_TX_START();
}

