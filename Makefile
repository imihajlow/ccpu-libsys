CC=ccpu-cc
ASM=../ccpu/tools/asm.py
CFLAGS=-std=gnu11 -Iinclude -I../ccpu-libc/include

LIBSYS_OBJECTS=lib/vga.o lib/ps2.o lib/ps2keyboard.o lib/crc.o lib/eth.o lib/mac.o lib/bitmix.o lib/card.o lib/fat/name.o lib/syscall.o
LIBSYS_FAT_OBJECTS=lib/fat/fat_fi.o lib/fat/path_fi.o lib/fat/last_error_fi.o lib/fat/exec_fi.o\
		lib/fat/loload.o lib/fat/lorun.o lib/fat/progressbar.o

all: libsys.a libfat_impl.a

libsys.a: $(LIBSYS_OBJECTS)
	ar rc $@ $^

libfat_impl.a: $(LIBSYS_FAT_OBJECTS)
	ar rc $@ $^

%_fi.o: %.c
	$(CC) $(CFLAGS) -DFAT_IMPL -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(ASM) -o $@ $^

clean:
	rm -f lib/*.s lib/*.o *.a lib/fat/*.o

