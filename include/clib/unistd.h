/*
 * unistd.h
 *
 * standard C header file
 * 
 * TODO:
 *  Original functions are based on https://github.com/dmo9000/cpmlibc
 *  but I'll probably have to rewrite.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 01.05.2021   tstih
 *
 */
#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>

/* Max CPM path len is NNX:AAAAAAAA.EEE plus zero terminator */
#define PATH_MAX 17

/* Open file. */
extern int open(const char *pathname, int flags);

/* Close file. */
extern int close(int fd);

/* Read count bytes from file. */
extern ssize_t read(int fd, void *buf, size_t count);

/* Write count bytes to file. */
extern ssize_t write(int fd, const void *buf, size_t count);

/* Move to position */
extern off_t lseek(int fd, off_t offset, int whence);

#endif /* __UNISTD_H__ */