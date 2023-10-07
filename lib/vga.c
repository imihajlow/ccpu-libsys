#include <libsys/vga.h>
#include <string.h>

void vga_clear(uint8_t color) {
    memset(VGA_CHAR_SEG, 0, VGA_ROWS * 128);
    memset(VGA_COLOR_SEG, color, VGA_ROWS * 128);
}
