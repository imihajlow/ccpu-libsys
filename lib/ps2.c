#include <libsys/ps2.h>

uint8_t ps2_read(void) {
    uint8_t status = PS2_STATUS;
    if (status & PS2_STATUS_HAS_DATA) {
        if (status & PS2_STATUS_RECV_VALID) {
            uint8_t data = PS2_DATA;
            PS2_STATUS = 0;
            return data;
        } else {
            PS2_DATA = PS2_CODE_RESEND;
            PS2_STATUS = 0;
        }
    }
    return PS2_READ_EMPTY;
}

#define STATE_START 0
#define STATE_END 1
#define STATE_WAIT_RESPONSE 2

static bool send_byte(uint8_t byte) {
    uint8_t status = PS2_STATUS;
    if (status & PS2_STATUS_HAS_DATA) {
        return false;
    }

    uint8_t state = STATE_START;
    while (state != STATE_END) {
        if (state == STATE_START) {
            PS2_DATA = byte;
            state = STATE_WAIT_RESPONSE;
        } else if (state == STATE_WAIT_RESPONSE) {
            status = PS2_STATUS;
            if (status & PS2_STATUS_HAS_DATA) {
                if (status & PS2_STATUS_RECV_VALID) {
                    uint8_t data = PS2_DATA;
                    if (data == PS2_CODE_ACK) {
                        state = STATE_END;
                    } else {
                        state = STATE_START;
                    }
                } else {
                    PS2_DATA = PS2_CODE_RESEND;
                }
                PS2_STATUS = 0;
            }
        }
    }
    return true;
}

bool ps2_reset(void) {
    return send_byte(0xff);
}

void ps2_set_led_mask(uint8_t mask) {
    send_byte(0xed);
    send_byte(mask);
}

void ps2_set_rate(uint8_t mask) {
    send_byte(0xf3);
    send_byte(mask);
}
