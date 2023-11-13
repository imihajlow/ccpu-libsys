    .export crc_check_relaxed
    .export crc_update
    .export crc_update_zeroes
    .export crc_table_0
    .export crc_table_1
    .export crc_table_2
    .export crc_table_3

    ; arguments for normal calls on frame B
    .const frameb_arg1 = 0xc800 + 8 * 0
    .const frameb_arg2 = 0xc800 + 8 * 1
    .const frameb_arg3 = 0xc800 + 8 * 2
    .const index       = 0xc800 + 8 * 3
    .const crc         = 0xc800 + 8 * 4
    .const ret_addr    = 0xc800 + 8 * 5

    .global __cc_ret

    ; bool crc_check_relaxed(const void *start, const void *end);
    .section text.crc_check_relaxed
crc_check_relaxed:
    mov a, pl
    mov b, a
    mov a, ph
    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    st  b
    inc pl
    st  a

    ; end -= 1
    ldi pl, lo(frameb_arg2)
    ld  b
    inc pl
    ld  a
    dec b
    sbb a, 0
    st  a
    dec pl
    st  b

    ; crc = 0xffffffff
    ldi pl, lo(crc)
    ldi a, 0xff
    st  a
    inc pl
    st  a
    inc pl
    st  a
    inc pl
    st  a

crc_check_relaxed_loop:
    ; if start == end goto loop end
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  b
    ldi pl, lo(frameb_arg2)
    ld  a
    sub b, a
    inc pl
    ld  a
    ldi pl, lo(frameb_arg1 + 1)
    ld  pl
    sub a, pl
    or  a, b
    ldi ph, hi(crc_check_relaxed_loop_end)
    ldi pl, lo(crc_check_relaxed_loop_end)
    jz

    ; index = *start ^ crc[0]
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc)
    ld  b
    xor a, b
    ldi pl, lo(index)
    st  a

    ; crc[0] = crc[1] ^ table0[index]
    ldi ph, hi(crc_table_0)
    mov pl, a
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 1)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[1] = crc[2] ^ table1[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_1)
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 2)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[2] = crc[3] ^ table2[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_2)
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 3)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[3] =          table3[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_3)
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 3)
    st  a

    ; start += 1
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  b
    inc pl
    ld  a
    inc b
    adc a, 0
    st  a
    dec pl
    st  b

    ldi ph, hi(crc_check_relaxed_loop)
    ldi pl, lo(crc_check_relaxed_loop)
    jmp
crc_check_relaxed_loop_end:
    ; check if crc == ~0x2144DF1C = 0xDEBB20E3
    ldi ph, hi(crc)
    ldi pl, lo(crc)
    ld  a
    ldi b, 0xE3
    sub b, a
    inc pl
    ld  a
    ldi pl, 0x20
    sub a, pl
    or  b, a
    ldi pl, lo(crc + 2)
    ld  a
    ldi pl, 0xBB
    sub a, pl
    or  b, a
    ldi pl, lo(crc + 3)
    ld  a
    ldi pl, 0xDE
    sub a, pl
    or  b, a
    ; if b == 0 return 1, else check next byte
    ldi a, 1
    ldi ph, hi(crc_check_return)
    ldi pl, lo(crc_check_return)
    jz

    ; index = *start ^ crc[0]
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc)
    ld  b
    xor a, b
    ldi pl, lo(index)
    st  a

    ; 0xDEBB20E3
    ; crc[0] = crc[1] ^ table0[index] - 0xE3
    ldi ph, hi(crc_table_0)
    mov pl, a
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 1)
    ld  b
    xor a, b
    ldi b, 0xe3
    sub a, b
    dec pl
    st  a

    ; crc[1] = crc[2] ^ table1[index] - 0x20
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_1)
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 2)
    ld  b
    xor a, b
    ldi b, 0x20
    sub a, b
    dec pl
    st  a

    ; crc[2] = crc[3] ^ table2[index] - 0xbb
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_2)
    ld  a
    ldi ph, hi(crc)
    ldi pl, lo(crc + 3)
    ld  b
    xor a, b
    ldi b, 0xbb
    sub b, a

    ; crc[3] =          table3[index] - 0xde
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_3)
    ld  a
    ldi pl, 0xde
    sub a, pl
    or  b, a ; b was crc[2]
    ldi ph, hi(crc)
    ldi pl, lo(crc + 1)
    ld  a
    or  b, a
    dec pl
    ld  a
    or  a, b
    ; if b == 0 return 1, else return 0
    mov a, 0
    ldi ph, hi(crc_check_return)
    ldi pl, lo(crc_check_return)
    jnz
    inc a

crc_check_return:
    ldi ph, hi(__cc_ret)
    ldi pl, lo(__cc_ret)
    st  a
    inc pl
    mov a, 0
    st  a

    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    jmp

    ; uint32_t crc_update(uint32_t crc, const void *start, const void *end);
    .section text.crc_update
crc_update:
    mov a, pl
    mov b, a
    mov a, ph
    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    st  b
    inc pl
    st  a

crc_update_loop:
    ; if start == end goto loop end
    ldi ph, hi(frameb_arg2)
    ldi pl, lo(frameb_arg2)
    ld  b
    ldi pl, lo(frameb_arg3)
    ld  a
    sub b, a
    inc pl
    ld  a
    ldi pl, lo(frameb_arg2 + 1)
    ld  pl
    sub a, pl
    or  a, b
    ldi ph, hi(crc_update_loop_end)
    ldi pl, lo(crc_update_loop_end)
    jz

    ; index = *start ^ crc[0]
    ldi ph, hi(frameb_arg2)
    ldi pl, lo(frameb_arg2)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  b
    xor a, b
    ldi pl, lo(index)
    st  a

    ; crc[0] = crc[1] ^ table0[index]
    ldi ph, hi(crc_table_0)
    mov pl, a
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 1)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[1] = crc[2] ^ table1[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_1)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 2)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[2] = crc[3] ^ table2[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_2)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 3)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[3] =          table3[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_3)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 3)
    st  a

    ; start += 1
    ldi ph, hi(frameb_arg2)
    ldi pl, lo(frameb_arg2)
    ld  b
    inc pl
    ld  a
    inc b
    adc a, 0
    st  a
    dec pl
    st  b

    ldi ph, hi(crc_update_loop)
    ldi pl, lo(crc_update_loop)
    jmp
crc_update_loop_end:
    ; copy result
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret)
    ldi pl, lo(__cc_ret)
    st  a
    inc pl
    st  b
    ldi ph, hi(frameb_arg1 + 2)
    ldi pl, lo(frameb_arg1 + 2)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 2)
    ldi pl, lo(__cc_ret + 2)
    st  a
    inc pl
    st  b

    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    jmp

    ; uint32_t crc_update_zeroes(uint32_t crc, uint16_t n_zeroes);
    .section text.crc_update_zeroes
crc_update_zeroes:
    mov a, pl
    mov b, a
    mov a, ph
    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    st  b
    inc pl
    st  a

crc_update_zeroes_loop:
    ; if n_zeroes == 0 goto loop end
    ldi ph, hi(frameb_arg2)
    ldi pl, lo(frameb_arg2)
    ld  b
    inc pl
    ld  a
    or  a, b
    ldi ph, hi(crc_update_zeroes_loop_end)
    ldi pl, lo(crc_update_zeroes_loop_end)
    jz

    ; index = crc[0]
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  a
    ldi pl, lo(index)
    st  a

    ; crc[0] = crc[1] ^ table0[index]
    ldi ph, hi(crc_table_0)
    mov pl, a
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 1)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[1] = crc[2] ^ table1[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_1)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 2)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[2] = crc[3] ^ table2[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_2)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 3)
    ld  b
    xor a, b
    dec pl
    st  a

    ; crc[3] =          table3[index]
    ldi pl, lo(index)
    ld  pl
    ldi ph, hi(crc_table_3)
    ld  a
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1 + 3)
    st  a

    ; n_zeroes -= 1
    ldi ph, hi(frameb_arg2)
    ldi pl, lo(frameb_arg2)
    ld  b
    inc pl
    ld  a
    dec b
    sbb a, 0
    st  a
    dec pl
    st  b

    ldi ph, hi(crc_update_zeroes_loop)
    ldi pl, lo(crc_update_zeroes_loop)
    jmp
crc_update_zeroes_loop_end:
    ; copy result
    ldi ph, hi(frameb_arg1)
    ldi pl, lo(frameb_arg1)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret)
    ldi pl, lo(__cc_ret)
    st  a
    inc pl
    st  b
    ldi ph, hi(frameb_arg1 + 2)
    ldi pl, lo(frameb_arg1 + 2)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 2)
    ldi pl, lo(__cc_ret + 2)
    st  a
    inc pl
    st  b

    ldi pl, lo(ret_addr)
    ldi ph, hi(ret_addr)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    jmp

    .section rodata.crc
    .align 256
crc_table_0:
    db 0x00, 0x96, 0x2C, 0xBA, 0x19, 0x8F, 0x35, 0xA3, 0x32, 0xA4, 0x1E, 0x88, 0x2B, 0xBD, 0x07, 0x91
    db 0x64, 0xF2, 0x48, 0xDE, 0x7D, 0xEB, 0x51, 0xC7, 0x56, 0xC0, 0x7A, 0xEC, 0x4F, 0xD9, 0x63, 0xF5
    db 0xC8, 0x5E, 0xE4, 0x72, 0xD1, 0x47, 0xFD, 0x6B, 0xFA, 0x6C, 0xD6, 0x40, 0xE3, 0x75, 0xCF, 0x59
    db 0xAC, 0x3A, 0x80, 0x16, 0xB5, 0x23, 0x99, 0x0F, 0x9E, 0x08, 0xB2, 0x24, 0x87, 0x11, 0xAB, 0x3D
    db 0x90, 0x06, 0xBC, 0x2A, 0x89, 0x1F, 0xA5, 0x33, 0xA2, 0x34, 0x8E, 0x18, 0xBB, 0x2D, 0x97, 0x01
    db 0xF4, 0x62, 0xD8, 0x4E, 0xED, 0x7B, 0xC1, 0x57, 0xC6, 0x50, 0xEA, 0x7C, 0xDF, 0x49, 0xF3, 0x65
    db 0x58, 0xCE, 0x74, 0xE2, 0x41, 0xD7, 0x6D, 0xFB, 0x6A, 0xFC, 0x46, 0xD0, 0x73, 0xE5, 0x5F, 0xC9
    db 0x3C, 0xAA, 0x10, 0x86, 0x25, 0xB3, 0x09, 0x9F, 0x0E, 0x98, 0x22, 0xB4, 0x17, 0x81, 0x3B, 0xAD
    db 0x20, 0xB6, 0x0C, 0x9A, 0x39, 0xAF, 0x15, 0x83, 0x12, 0x84, 0x3E, 0xA8, 0x0B, 0x9D, 0x27, 0xB1
    db 0x44, 0xD2, 0x68, 0xFE, 0x5D, 0xCB, 0x71, 0xE7, 0x76, 0xE0, 0x5A, 0xCC, 0x6F, 0xF9, 0x43, 0xD5
    db 0xE8, 0x7E, 0xC4, 0x52, 0xF1, 0x67, 0xDD, 0x4B, 0xDA, 0x4C, 0xF6, 0x60, 0xC3, 0x55, 0xEF, 0x79
    db 0x8C, 0x1A, 0xA0, 0x36, 0x95, 0x03, 0xB9, 0x2F, 0xBE, 0x28, 0x92, 0x04, 0xA7, 0x31, 0x8B, 0x1D
    db 0xB0, 0x26, 0x9C, 0x0A, 0xA9, 0x3F, 0x85, 0x13, 0x82, 0x14, 0xAE, 0x38, 0x9B, 0x0D, 0xB7, 0x21
    db 0xD4, 0x42, 0xF8, 0x6E, 0xCD, 0x5B, 0xE1, 0x77, 0xE6, 0x70, 0xCA, 0x5C, 0xFF, 0x69, 0xD3, 0x45
    db 0x78, 0xEE, 0x54, 0xC2, 0x61, 0xF7, 0x4D, 0xDB, 0x4A, 0xDC, 0x66, 0xF0, 0x53, 0xC5, 0x7F, 0xE9
    db 0x1C, 0x8A, 0x30, 0xA6, 0x05, 0x93, 0x29, 0xBF, 0x2E, 0xB8, 0x02, 0x94, 0x37, 0xA1, 0x1B, 0x8D
crc_table_1:
    db 0x00, 0x30, 0x61, 0x51, 0xC4, 0xF4, 0xA5, 0x95, 0x88, 0xB8, 0xE9, 0xD9, 0x4C, 0x7C, 0x2D, 0x1D
    db 0x10, 0x20, 0x71, 0x41, 0xD4, 0xE4, 0xB5, 0x85, 0x98, 0xA8, 0xF9, 0xC9, 0x5C, 0x6C, 0x3D, 0x0D
    db 0x20, 0x10, 0x41, 0x71, 0xE4, 0xD4, 0x85, 0xB5, 0xA8, 0x98, 0xC9, 0xF9, 0x6C, 0x5C, 0x0D, 0x3D
    db 0x30, 0x00, 0x51, 0x61, 0xF4, 0xC4, 0x95, 0xA5, 0xB8, 0x88, 0xD9, 0xE9, 0x7C, 0x4C, 0x1D, 0x2D
    db 0x41, 0x71, 0x20, 0x10, 0x85, 0xB5, 0xE4, 0xD4, 0xC9, 0xF9, 0xA8, 0x98, 0x0D, 0x3D, 0x6C, 0x5C
    db 0x51, 0x61, 0x30, 0x00, 0x95, 0xA5, 0xF4, 0xC4, 0xD9, 0xE9, 0xB8, 0x88, 0x1D, 0x2D, 0x7C, 0x4C
    db 0x61, 0x51, 0x00, 0x30, 0xA5, 0x95, 0xC4, 0xF4, 0xE9, 0xD9, 0x88, 0xB8, 0x2D, 0x1D, 0x4C, 0x7C
    db 0x71, 0x41, 0x10, 0x20, 0xB5, 0x85, 0xD4, 0xE4, 0xF9, 0xC9, 0x98, 0xA8, 0x3D, 0x0D, 0x5C, 0x6C
    db 0x83, 0xB3, 0xE2, 0xD2, 0x47, 0x77, 0x26, 0x16, 0x0B, 0x3B, 0x6A, 0x5A, 0xCF, 0xFF, 0xAE, 0x9E
    db 0x93, 0xA3, 0xF2, 0xC2, 0x57, 0x67, 0x36, 0x06, 0x1B, 0x2B, 0x7A, 0x4A, 0xDF, 0xEF, 0xBE, 0x8E
    db 0xA3, 0x93, 0xC2, 0xF2, 0x67, 0x57, 0x06, 0x36, 0x2B, 0x1B, 0x4A, 0x7A, 0xEF, 0xDF, 0x8E, 0xBE
    db 0xB3, 0x83, 0xD2, 0xE2, 0x77, 0x47, 0x16, 0x26, 0x3B, 0x0B, 0x5A, 0x6A, 0xFF, 0xCF, 0x9E, 0xAE
    db 0xC2, 0xF2, 0xA3, 0x93, 0x06, 0x36, 0x67, 0x57, 0x4A, 0x7A, 0x2B, 0x1B, 0x8E, 0xBE, 0xEF, 0xDF
    db 0xD2, 0xE2, 0xB3, 0x83, 0x16, 0x26, 0x77, 0x47, 0x5A, 0x6A, 0x3B, 0x0B, 0x9E, 0xAE, 0xFF, 0xCF
    db 0xE2, 0xD2, 0x83, 0xB3, 0x26, 0x16, 0x47, 0x77, 0x6A, 0x5A, 0x0B, 0x3B, 0xAE, 0x9E, 0xCF, 0xFF
    db 0xF2, 0xC2, 0x93, 0xA3, 0x36, 0x06, 0x57, 0x67, 0x7A, 0x4A, 0x1B, 0x2B, 0xBE, 0x8E, 0xDF, 0xEF
crc_table_2:
    db 0x00, 0x07, 0x0E, 0x09, 0x6D, 0x6A, 0x63, 0x64, 0xDB, 0xDC, 0xD5, 0xD2, 0xB6, 0xB1, 0xB8, 0xBF
    db 0xB7, 0xB0, 0xB9, 0xBE, 0xDA, 0xDD, 0xD4, 0xD3, 0x6C, 0x6B, 0x62, 0x65, 0x01, 0x06, 0x0F, 0x08
    db 0x6E, 0x69, 0x60, 0x67, 0x03, 0x04, 0x0D, 0x0A, 0xB5, 0xB2, 0xBB, 0xBC, 0xD8, 0xDF, 0xD6, 0xD1
    db 0xD9, 0xDE, 0xD7, 0xD0, 0xB4, 0xB3, 0xBA, 0xBD, 0x02, 0x05, 0x0C, 0x0B, 0x6F, 0x68, 0x61, 0x66
    db 0xDC, 0xDB, 0xD2, 0xD5, 0xB1, 0xB6, 0xBF, 0xB8, 0x07, 0x00, 0x09, 0x0E, 0x6A, 0x6D, 0x64, 0x63
    db 0x6B, 0x6C, 0x65, 0x62, 0x06, 0x01, 0x08, 0x0F, 0xB0, 0xB7, 0xBE, 0xB9, 0xDD, 0xDA, 0xD3, 0xD4
    db 0xB2, 0xB5, 0xBC, 0xBB, 0xDF, 0xD8, 0xD1, 0xD6, 0x69, 0x6E, 0x67, 0x60, 0x04, 0x03, 0x0A, 0x0D
    db 0x05, 0x02, 0x0B, 0x0C, 0x68, 0x6F, 0x66, 0x61, 0xDE, 0xD9, 0xD0, 0xD7, 0xB3, 0xB4, 0xBD, 0xBA
    db 0xB8, 0xBF, 0xB6, 0xB1, 0xD5, 0xD2, 0xDB, 0xDC, 0x63, 0x64, 0x6D, 0x6A, 0x0E, 0x09, 0x00, 0x07
    db 0x0F, 0x08, 0x01, 0x06, 0x62, 0x65, 0x6C, 0x6B, 0xD4, 0xD3, 0xDA, 0xDD, 0xB9, 0xBE, 0xB7, 0xB0
    db 0xD6, 0xD1, 0xD8, 0xDF, 0xBB, 0xBC, 0xB5, 0xB2, 0x0D, 0x0A, 0x03, 0x04, 0x60, 0x67, 0x6E, 0x69
    db 0x61, 0x66, 0x6F, 0x68, 0x0C, 0x0B, 0x02, 0x05, 0xBA, 0xBD, 0xB4, 0xB3, 0xD7, 0xD0, 0xD9, 0xDE
    db 0x64, 0x63, 0x6A, 0x6D, 0x09, 0x0E, 0x07, 0x00, 0xBF, 0xB8, 0xB1, 0xB6, 0xD2, 0xD5, 0xDC, 0xDB
    db 0xD3, 0xD4, 0xDD, 0xDA, 0xBE, 0xB9, 0xB0, 0xB7, 0x08, 0x0F, 0x06, 0x01, 0x65, 0x62, 0x6B, 0x6C
    db 0x0A, 0x0D, 0x04, 0x03, 0x67, 0x60, 0x69, 0x6E, 0xD1, 0xD6, 0xDF, 0xD8, 0xBC, 0xBB, 0xB2, 0xB5
    db 0xBD, 0xBA, 0xB3, 0xB4, 0xD0, 0xD7, 0xDE, 0xD9, 0x66, 0x61, 0x68, 0x6F, 0x0B, 0x0C, 0x05, 0x02
crc_table_3:
    db 0x00, 0x77, 0xEE, 0x99, 0x07, 0x70, 0xE9, 0x9E, 0x0E, 0x79, 0xE0, 0x97, 0x09, 0x7E, 0xE7, 0x90
    db 0x1D, 0x6A, 0xF3, 0x84, 0x1A, 0x6D, 0xF4, 0x83, 0x13, 0x64, 0xFD, 0x8A, 0x14, 0x63, 0xFA, 0x8D
    db 0x3B, 0x4C, 0xD5, 0xA2, 0x3C, 0x4B, 0xD2, 0xA5, 0x35, 0x42, 0xDB, 0xAC, 0x32, 0x45, 0xDC, 0xAB
    db 0x26, 0x51, 0xC8, 0xBF, 0x21, 0x56, 0xCF, 0xB8, 0x28, 0x5F, 0xC6, 0xB1, 0x2F, 0x58, 0xC1, 0xB6
    db 0x76, 0x01, 0x98, 0xEF, 0x71, 0x06, 0x9F, 0xE8, 0x78, 0x0F, 0x96, 0xE1, 0x7F, 0x08, 0x91, 0xE6
    db 0x6B, 0x1C, 0x85, 0xF2, 0x6C, 0x1B, 0x82, 0xF5, 0x65, 0x12, 0x8B, 0xFC, 0x62, 0x15, 0x8C, 0xFB
    db 0x4D, 0x3A, 0xA3, 0xD4, 0x4A, 0x3D, 0xA4, 0xD3, 0x43, 0x34, 0xAD, 0xDA, 0x44, 0x33, 0xAA, 0xDD
    db 0x50, 0x27, 0xBE, 0xC9, 0x57, 0x20, 0xB9, 0xCE, 0x5E, 0x29, 0xB0, 0xC7, 0x59, 0x2E, 0xB7, 0xC0
    db 0xED, 0x9A, 0x03, 0x74, 0xEA, 0x9D, 0x04, 0x73, 0xE3, 0x94, 0x0D, 0x7A, 0xE4, 0x93, 0x0A, 0x7D
    db 0xF0, 0x87, 0x1E, 0x69, 0xF7, 0x80, 0x19, 0x6E, 0xFE, 0x89, 0x10, 0x67, 0xF9, 0x8E, 0x17, 0x60
    db 0xD6, 0xA1, 0x38, 0x4F, 0xD1, 0xA6, 0x3F, 0x48, 0xD8, 0xAF, 0x36, 0x41, 0xDF, 0xA8, 0x31, 0x46
    db 0xCB, 0xBC, 0x25, 0x52, 0xCC, 0xBB, 0x22, 0x55, 0xC5, 0xB2, 0x2B, 0x5C, 0xC2, 0xB5, 0x2C, 0x5B
    db 0x9B, 0xEC, 0x75, 0x02, 0x9C, 0xEB, 0x72, 0x05, 0x95, 0xE2, 0x7B, 0x0C, 0x92, 0xE5, 0x7C, 0x0B
    db 0x86, 0xF1, 0x68, 0x1F, 0x81, 0xF6, 0x6F, 0x18, 0x88, 0xFF, 0x66, 0x11, 0x8F, 0xF8, 0x61, 0x16
    db 0xA0, 0xD7, 0x4E, 0x39, 0xA7, 0xD0, 0x49, 0x3E, 0xAE, 0xD9, 0x40, 0x37, 0xA9, 0xDE, 0x47, 0x30
    db 0xBD, 0xCA, 0x53, 0x24, 0xBA, 0xCD, 0x54, 0x23, 0xB3, 0xC4, 0x5D, 0x2A, 0xB4, 0xC3, 0x5A, 0x2D
