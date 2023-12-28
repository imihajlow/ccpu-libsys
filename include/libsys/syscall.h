#pragma once
#include <stdint.h>
#include <stddef.h>

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
#define SYSCALL_GET_LAST_ERROR 11
#define SYSCALL_REBOOT 12
#define SYSCALL_FAT_TELL 13
#define SYSCALL_FAT_SEEK 14
#define SYSCALL_PUTCHAR 15
#define SYSCALL_FAT_PRINT_LAST_ERROR 16
#define SYSCALL_GET_SHARED_MEM_PTR 17
#define SYSCALL_GET_SHARED_MEM_SIZE 18

#define reboot() syscall(SYSCALL_REBOOT)

typedef int (*__sycall_putchar)(char _syscall_nr, int x);
#define syscall_putchar(x) ((__sycall_putchar)SYSCALL_ADDR)(SYSCALL_PUTCHAR, x)

#if defined(PUTCHAR_DECL)
int putchar(int c) {
    return syscall_putchar(c);
}
#endif


typedef void *(*__syscall_get_shared_mem_ptr)(char _syscall_nr);
#define get_shared_mem_ptr() ((__syscall_get_shared_mem_ptr)SYSCALL_ADDR)(SYSCALL_GET_SHARED_MEM_PTR)

typedef size_t (*__syscall_get_shared_mem_size)(char _syscall_nr);
#define get_shared_mem_size() ((__syscall_get_shared_mem_size)SYSCALL_ADDR)(SYSCALL_GET_SHARED_MEM_SIZE)
