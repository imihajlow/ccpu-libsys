#pragma once
#include <stdint.h>
#include <stddef.h>

#define ETH_SR (*(volatile uint8_t*)0xFB00)
#define ETH_RX_LEN (*(volatile uint16_t*)0xFB02)
#define ETH_BUF ((volatile uint8_t *)0xF000)
#define ETH_RX_ARM() do { *(volatile uint8_t*)0xFB00 = 0; } while (0)
#define ETH_TX_START() do { *(volatile uint8_t*)0xFB01 = 0; } while (0)

#define ETH_SR_RX_FULL (1 << 0)
#define ETH_SR_TX_BUSY (1 << 1)

// Write Ethernet preamble to the transmit buffer.
void eth_init(void);

// Write the packet to the buffer, calculate and write CRC, enable transmission.
// Does not check for completion of previous transfer!
void eth_transmit(const void *data, size_t size);
