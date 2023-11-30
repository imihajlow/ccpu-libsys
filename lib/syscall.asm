    .global __cc_ret
    .export syscall

    .const os_entry = 0xb000
    .const frameb_ret_val = 0xc800

    .section text.syscall
syscall:
    mov a, pl
    mov b, a
    mov a, ph
    ldi ph, hi(syscall_ret)
    ldi pl, lo(syscall_ret)
    st  b
    inc pl
    st  a

    ldi ph, hi(os_entry)
    ldi pl, lo(os_entry)
    jmp

    ; copy return value
    ldi  ph, hi(frameb_ret_val + 0)
    ldi  pl, lo(frameb_ret_val + 0)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 0)
    ldi pl, lo(__cc_ret + 0)
    st  a
    inc pl
    st  b
    ldi  ph, hi(frameb_ret_val + 2)
    ldi  pl, lo(frameb_ret_val + 2)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 2)
    ldi pl, lo(__cc_ret + 2)
    st  a
    inc pl
    st  b
    ldi  ph, hi(frameb_ret_val + 4)
    ldi  pl, lo(frameb_ret_val + 4)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 4)
    ldi pl, lo(__cc_ret + 4)
    st  a
    inc pl
    st  b
    ldi  ph, hi(frameb_ret_val + 6)
    ldi  pl, lo(frameb_ret_val + 6)
    ld  a
    inc pl
    ld  b
    ldi ph, hi(__cc_ret + 6)
    ldi pl, lo(__cc_ret + 6)
    st  a
    inc pl
    st  b


    ldi ph, hi(syscall_ret)
    ldi pl, lo(syscall_ret)
    ld  a
    inc pl
    ld  ph
    mov pl, a
    jmp

    .section bss.syscall
    .align 2
syscall_ret: res 2
