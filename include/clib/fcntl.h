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

#define O_RDONLY    0
#define O_WRONLY    1
#define O_RDWR      2
#define O_TRUNC     4

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#endif /* __FCNTL_H__ */