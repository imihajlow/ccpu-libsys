    .export lorun

    .section ramtext.lorun
lorun:
    ; load argc
    ldi ph, hi(0xc800)
    ldi pl, lo(0xc800)
    ld  b

    ; set SP1 to point to the vararg page, SP0 to the previous page
    ldi ph, hi(0xc809)
    ldi pl, lo(0xc809)
    ld  a
    ldi ph, hi(0xfc01)
    ldi pl, lo(0xfc01)
    st  a
    dec a
    dec pl
    st  a

    ; store argc on frame
    ldi ph, hi(0xc800)
    st  b

    ldi pl, lo(0xff02)
    ldi ph, hi(0xff02)
    ldi a, 0xdf ; enable segments A-B, D-E, both boards, lo RAM
    st a

    mov a, 0
    mov pl, a
    mov ph, a
    jmp

    ldi pl, lo(0xff02)
    ldi ph, hi(0xff02)
    ldi a, 0xde ; enable segments A-B, D-E, both boards, disable lo RAM
    st a

    ; restart
    mov a, 0
    mov ph, a
    mov pl, a
    jmp


    .section bss.lorun
argc:
    res 1
