#pragma once
#include <stdint.h>

void syscall(uint8_t n);

#define SYSCALL_ADDR (&syscall)

#define SYSCALL_FAT_OPEN_PATH 0
#define SYSCALL_FAT_READ 1
#define SYSCALL_FAT_WRITE 2
#define SYSCALL_FAT_CLOSE 3
#define SYSCALL_FAT_TRUNCATE 4
#define SYSCALL_FAT_SEEK_END 5
#define SYSCALL_FAT_GET_SIZE 6
#define SYSCALL_FAT_GET_NEXT_DIR_ENTRY 7
#define SYSCALL_FAT_MOUNT 8
#define SYSCALL_EXEC 9
#define SYSCALL_EXEC_FD 10
#define SYSCALL_GET_LAST_ERROR 11
#define SYSCALL_REBOOT 12
#define SYSCALL_FAT_TELL 13
#define SYSCALL_FAT_SEEK 14
#define SYSCALL_PUTCHAR 15
#define SYSCALL_FAT_PRINT_LAST_ERROR 16

#define reboot() syscall(SYSCALL_REBOOT)

typedef int (*__sycall_putchar)(char _syscall_nr, int x);
#define syscall_putchar(x) ((__sycall_putchar)SYSCALL_ADDR)(SYSCALL_PUTCHAR, x)

#if defined(PUTCHAR_DECL)
int putchar(int c) {
    return syscall_putchar(c);
}
#endif
