/*
 * fcntl.h
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
#ifndef __FNCTL_H__
#define __FNCTL_H__

#include <limits.h>

#define O_ACCMODE    0003
#define O_RDONLY       00
#define O_WRONLY       01
#define O_RDWR         02
#define O_TRUNC     01000

#define O_CREAT		00000100            /* not fcntl */
#define O_EXCL		00000200            /* not fcntl */

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#endif /* __FCNTL_H__ */