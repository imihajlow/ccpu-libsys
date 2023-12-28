#include <string.h>
#include <libsys/fat/fat.h>

#include "progressbar.h"
#include "loload.h"
#include "lorun.h"

extern void *__seg_ramtext_begin;

bool fat_exec(char _syscall_nr, const char *filename, uint8_t argc, ...) {
    uint8_t fd = fat_open_path(0, filename, 0);
    if (fd == FAT_BAD_DESC) {
        return false;
    }
    va_list va;
    va_start(va, argc);
    bool r = fat_exec_fd(0, fd, argc, va);
    va_end(va);
    return r;
}

bool fat_exec_fd(char _syscall_nr, uint8_t fd, uint8_t argc, va_list va) {
    struct FatDirEntry *de = fat_get_dir_entry(fd);
    if (!de) {
        fat_close(0, fd);
        return false;
    }
    char *ext = de->filename + 8;
    if (memcmp(ext, "APP", 3) != 0) {
        fat_close(0, fd);
        last_error = FAT_EXEC_ERROR_WRONG_EXTENSION;
        return false;
    }
    uint32_t size32 = de->size;
    uint16_t max_size = (uint16_t)&__seg_ramtext_begin;
    if (size32 > max_size) {
        fat_close(0, fd);
        last_error = FAT_EXEC_ERROR_FILE_TOO_LARGE;
        return false;
    }
    uint16_t size = size32;
    progressbar_init(size >> 8);
    uint8_t dst_hi = 0;
    // load lo RAM part
    while (size != 0 && dst_hi != 0x80) {
        uint16_t r = fat_read(0, fd, loload_buf, sizeof(loload_buf));
        if (r != sizeof(loload_buf)) {
            if (last_error != FAT_EOF) {
                fat_close(0, fd);
                return false;
            }
        }
        copy_to_loram(dst_hi);
        size -= r;
        dst_hi += 1;
        progressbar_progress(1);
    }
    // load hi RAM part
    if (size != 0) {
        uint16_t r = fat_read(0, fd, (uint8_t*)0x8000, size);
        if (r != size) {
            fat_close(0, fd);
            return false;
        }
        progressbar_progress(r >> 8);
    }
    fat_close(0, fd);
    lorun(argc, va);
    return true;
}
