/*
 * stdlib.h
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
#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>

extern void exit(int status);

extern long strtol(char *nptr, char **endptr, int base);
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

extern int abs (int i);

extern int rand(void);
extern void srand(unsigned int seed);

#define META_SIZE sizeof(struct block_meta)
/* TODO: malloc, free, realloc, calloc */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#endif /* __STDLIB_H__ */