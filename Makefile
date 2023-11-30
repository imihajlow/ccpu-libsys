CC=ccpu-cc
ASM=../ccpu/tools/asm.py
CFLAGS=--std=gnu11 -I include -I ../ccpu-libc/include -D FAT_IMPL

LIBSYS_OBJECTS=lib/vga.o lib/ps2.o lib/ps2keyboard.o lib/crc.o lib/eth.o lib/mac.o lib/bitmix.o lib/card.o lib/fat/name.o lib/syscall.o
LIBSYS_FAT_OBJECTS=lib/fat/fat.o lib/fat/path.o lib/fat/last_error.o lib/fat/exec.o lib/fat/loload.o lib/fat/lorun.o lib/fat/progressbar.o

all: libsys.a libfat_impl.a

libsys.a: $(LIBSYS_OBJECTS)
	ar rc $@ $^

libfat_impl.a: $(LIBSYS_FAT_OBJECTS)
	ar rc $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $(@:.o=.s) $<
	$(ASM) -o $@ $(@:.o=.s)

%.o: %.asm
	$(ASM) -o $@ $^

clean:
	rm -f lib/*.s lib/*.o libsys.a

