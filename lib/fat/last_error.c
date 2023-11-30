#include <libsys/fat/fat.h>
#include <stdio.h>

uint8_t last_error = 0;

uint8_t fat_get_last_error(char _syscall_nr) {
    return last_error;
}


void fat_print_last_error(void) {
    uint8_t e = last_error;
    switch (e) {
    case FAT_ERROR_OK: puts("ok"); break;
    case FAT_ERROR_NO_CARD: puts("no card"); break;
    case FAT_ERROR_UNSUPPORTED_FAT_TYPE: puts("unsupported FAT type"); break;
    case FAT_ERROR_UNSUPPORTED_SECTOR_SIZE: puts("unsupported sector size"); break;
    case FAT_ERROR_TOO_MANY_FATS: puts("too many FATs"); break;
    case FAT_ERROR_TOO_MANY_OPEN_FILES: puts("too many open files"); break;
    case FAT_ERROR_NOT_A_DIR: puts("not a directory"); break;
    case FAT_ERROR_CARD_REINSERTED: puts("card was reinserted"); break;
    case FAT_ERROR_CARD: puts("card error"); break;
    case FAT_ERROR_FS_BROKEN: puts("filesystem is broken"); break;
    case FAT_ERROR_NOT_FOUND: puts("not found"); break;
    case FAT_ERROR_BAD_DESCRIPTOR: puts("bad descriptor"); break;
    case FAT_EOF: puts("EOF"); break;
    case FAT_ERROR_RELATIVE_PATH: puts("relative path"); break;
    case FAT_ERROR_IS_DIR: puts("is a directory"); break;
    case FAT_ERROR_STALE_DESCRIPTOR: puts("stale descriptor"); break;
    case FAT_ERROR_FILE_EXISTS: puts("file exists"); break;
    case FAT_ERROR_DIR_FULL: puts("directory is full"); break;
    case FAT_EXEC_ERROR_WRONG_EXTENSION: puts("wrong extension"); break;
    case FAT_EXEC_ERROR_FILE_TOO_LARGE: puts("file is too large"); break;
    default: puts("unknown error"); break;
    }
}
