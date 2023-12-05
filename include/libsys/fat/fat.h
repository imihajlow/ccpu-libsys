#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "name.h"

#define FAT_BAD_DESC 0xFF

#define FAT_ERROR_OK 0
#define FAT_ERROR_NO_CARD 1
#define FAT_ERROR_UNSUPPORTED_FAT_TYPE 2
#define FAT_ERROR_UNSUPPORTED_SECTOR_SIZE 3
#define FAT_ERROR_TOO_MANY_FATS 4
#define FAT_ERROR_TOO_MANY_OPEN_FILES 5
#define FAT_ERROR_NOT_A_DIR 6
#define FAT_ERROR_CARD_REINSERTED 7
#define FAT_ERROR_CARD 8
#define FAT_ERROR_FS_BROKEN 9
#define FAT_ERROR_NOT_FOUND 10
#define FAT_ERROR_BAD_DESCRIPTOR 11
#define FAT_EOF 12
#define FAT_ERROR_RELATIVE_PATH 13
#define FAT_ERROR_IS_DIR 14
#define FAT_ERROR_STALE_DESCRIPTOR 15
#define FAT_ERROR_FILE_EXISTS 16
#define FAT_ERROR_DIR_FULL 17
#define FAT_EXEC_ERROR_WRONG_EXTENSION 64
#define FAT_EXEC_ERROR_FILE_TOO_LARGE 65

#define O_CREAT (1 << 0)
#define O_EXCL (1 << 1)

#define FAT_FILE_ATTR_READ_ONLY 1
#define FAT_FILE_ATTR_HIDDEN 2
#define FAT_FILE_ATTR_SYSTEM 4
#define FAT_FILE_ATTR_VOLUME_ID 8
#define FAT_FILE_ATTR_DIRECTORY 16
#define FAT_FILE_ATTR_ARCHIVE 32
#define FAT_FILE_ATTR_LONG_FILENAME 0x0F

struct FatDirEntry {
    char filename[11];
    uint8_t attrs;
    uint8_t _reserved;
    uint8_t create_dsec;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t cluster_hi; // 0x0000
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t cluster_lo; // 0x0004
    uint32_t size;
} __attribute__((packed));

static_assert(sizeof(struct FatDirEntry) == 32, "sizeof FatDirEntry must be 32");


#if defined(FAT_IMPL)

extern uint8_t last_error;

void fat_print_last_error(void);

uint8_t fat_get_last_error(char _syscall_nr);

/**
 Initializes data structures. Does not fail.
 */
void fat_init(void);

/**
 Initializes the card and tries to read FAT headers.

 Returns bool, sets last_error on failure.
 */
bool fat_mount(char _syscall_nr);

/**
 Open a directory
 @param dir - dir entry or 0 for root dir.
 Returns dir descriptor or FAT_BAD_DESC on error, sets last_error on failure.
 */
uint8_t fat_open_dir(struct FatDirEntry *dir);

/**
 Returns true on success, false on fail (or when no entries are left), sets last_error on failure.
 */
bool fat_get_next_dir_entry(char _syscall_nr, uint8_t dir_desc, struct FatDirEntry *dst, uint8_t attr_skip_mask);

/**
 Returns file descriptor or FAT_BAD_DESC on error, sets last_error on failure.
 */
uint8_t fat_open_file(struct FatDirEntry *dir, const char *name, uint8_t mode);

/**
 Returns number of bytes actually read, sets last_error on failure.
*/
uint16_t fat_read(char _syscall_nr, uint8_t fd, void *dst, uint16_t len);

/**
 Returns number of bytes actually written, sets last_error on failure.
*/
uint16_t fat_write(char _syscall_nr, uint8_t fd, const void *src, uint16_t len);

/**
 Set file size to the current read/write pointer.
 Returns bool, sets last_error on failure.
 */
bool fat_truncate(char _syscall_nr, uint8_t fd);

/**
 Set file size to the current read/write pointer.
 Returns bool, sets last_error on failure.
 */
bool fat_seek_end(char _syscall_nr, uint8_t fd);

/**
 Returns bool, set last_error on failure. Fails on a stale descriptor when needs to write out the dir entry.
 */
bool fat_close(char _syscall_nr, uint8_t fd);

/**
 Returns 0xffffffff and sets last_error on error.
 */
uint32_t fat_get_size(char _syscall_nr, uint8_t fd);

/**
 Returns the dir entry associated with the file or 0 on error.
 */
struct FatDirEntry *fat_get_dir_entry(uint8_t fd);

/**
 * Look into directory entry for name inside parent and fill dst if it's a dir.
 * @return        true on success, false on fail, sets last_error on failure.
 */
bool fat_change_dir(struct FatDirEntry *parent, const char *name, struct FatDirEntry *dst);

/**
 * Load a file into Lo RAM and jump there.
 */
bool fat_exec(char _syscall_nr, const char *filename);

/**
 * Load a file into Lo RAM and jump there.
 */
bool fat_exec_fd(char _syscall_nr, uint8_t fd);


/**
 * Allocates a file descriptor. Walks the path. Populates file descriptor with the information about the file/dir.
 *
 * @param  path file/dir path
 * @param  mode open mode
 * @return file descriptor
 */
uint8_t fat_find_path(const char *path, uint8_t mode);

/**
 * Returns file descriptor or FAT_BAD_DESC on error, sets last_error on failure.
 */
uint8_t fat_open_path(char _syscall_nr, const char *path, uint8_t mode);

#else // defined(FAT_IMPL)

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

typedef uint8_t (*__fat_open_path_t)(char _syscall_nr, const char *path, uint8_t mode);
#define fat_open_path(path, mode) ((__fat_open_path_t)SYSCALL_ADDR)(SYSCALL_FAT_OPEN_PATH, path, mode)

typedef uint16_t (*__fat_read_t)(char _syscall_nr, uint8_t fd, void *dst, uint16_t len);
#define fat_read(fd, dst, len) ((__fat_read_t)SYSCALL_ADDR)(SYSCALL_FAT_READ, fd, dst, len)

typedef uint16_t (*__fat_write_t)(char _syscall_nr, uint8_t fd, const void *src, uint16_t len);
#define fat_write(fd, src, len) ((__fat_write_t)SYSCALL_ADDR)(SYSCALL_FAT_WRITE, fd, src, len)

typedef bool (*__fat_close_t)(char _syscall_nr, uint8_t fd);
#define fat_close(fd) ((__fat_close_t)SYSCALL_ADDR)(SYSCALL_FAT_CLOSE, fd)

typedef bool (*__fat_truncate_t)(char _syscall_nr, uint8_t fd);
#define fat_truncate(fd) ((__fat_truncate_t)SYSCALL_ADDR)(SYSCALL_FAT_TRUNCATE, fd)

typedef bool (*__fat_seek_end_t)(char _syscall_nr, uint8_t fd);
#define fat_seek_end(fd) ((__fat_seek_end_t)SYSCALL_ADDR)(SYSCALL_FAT_SEEK_END, fd)

typedef uint32_t (*__fat_get_size_t)(char _syscall_nr, uint8_t fd);
#define fat_get_size(fd) ((__fat_get_size_t)SYSCALL_ADDR)(SYSCALL_FAT_GET_SIZE, fd)

typedef bool (*__fat_get_next_dir_entry_t)(char _syscall_nr, uint8_t dir_desc, struct FatDirEntry *dst, uint8_t attr_skip_mask);
#define fat_get_next_dir_entry(dir_desc, dst, attr_skip_mask) ((__fat_get_next_dir_entry_t)SYSCALL_ADDR)(SYSCALL_FAT_GET_NEXT_DIR_ENTRY, dir_desc, dst, attr_skip_mask)

typedef bool (*__fat_mount_t)(char _syscall_nr);
#define fat_mount() ((__fat_mount_t)SYSCALL_ADDR)(SYSCALL_FAT_MOUNT)

typedef bool (*__fat_exec_t)(char _syscall_nr, const char *filename);
#define fat_exec(filename) ((__fat_exec_t)SYSCALL_ADDR)(SYSCALL_EXEC, filename)

typedef bool (*__fat_exec_fd_t)(char _syscall_nr, uint8_t fd);
#define fat_exec_fd(fd) ((__fat_exec_fd_t)SYSCALL_ADDR)(SYSCALL_EXEC_FD, fd)

typedef uint8_t (*__get_last_error_fd_t)(char _syscall_nr);
#define fat_get_last_error() ((__get_last_error_fd_t)SYSCALL_ADDR)(SYSCALL_GET_LAST_ERROR)

#endif // defined(FAT_IMPL)
