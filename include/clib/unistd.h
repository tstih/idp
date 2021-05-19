/*
 * unistd.h
 *
 * standard C header file
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
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

#define PATH_MAX 13

typedef uint16_t useconds_t;

/* TODO:
extern unsigned int alarm(unsigned int seconds);
extern int usleep(useconds_t usec);
extern void *sbrk(intptr_t increment);
extern int isatty(int fd);
extern int unlink(const char *path);
*/

extern int open(const char *pathname, int flags);
extern int close(int fd);
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, const void *buf, size_t count);
extern off_t lseek(int fd, off_t offset, int whence);


#endif /* __UNISTD_H__ */