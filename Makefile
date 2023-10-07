CC=ccpu-cc
ASM=../ccpu/tools/asm.py
CFLAGS=--std=gnu11 -I include -I ../ccpu-libc/include

LIBSYS_OBJECTS=lib/vga.o lib/ps2.o lib/ps2keyboard.o

libsys.a: $(LIBSYS_OBJECTS)
	ar rc $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $(@:.o=.s) $<
	$(ASM) -o $@ $(@:.o=.s)

%.o: %.asm
	$(ASM) -o $@ $^

clean:
	rm -f lib/*.s lib/*.o libsys.a

