/*
 * stdlib.h
 *
 * standard C header file
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * TODO:
 *  malloc, free, realloc, calloc
 *  META_SIZE could be obsolerte ... #define META_SIZE sizeof(struct block_meta)
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

/* Standard requires it here. */
#ifndef NULL
#define NULL 0
#endif /* NULL */

/* Exit application. */
extern void exit(int status);

/* String to long using base. */
extern long strtol(char *nptr, char **endptr, int base);

/* Strin to unsigned long using base, */
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

/* Absolute value. */
extern int abs (int i);

/* Return random number */
extern int rand(void);

/* Set random seed. */
extern void srand(unsigned int seed);

#define META_SIZE sizeof(struct block_meta)

#endif /* __STDLIB_H__ */