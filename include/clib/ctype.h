/*
 * ctype.h
 *
 * idp standard C library header file
 * 
 * NOTES:
 *  the idp standard library is based on the cpmlibc
 *  https://github.com/dmo9000/cpmlibc
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 02.05.2021   tstih
 *
 */
#ifndef __CTYPE_H__
#define __CTYPE_H__

#include <stdbool.h>

/* True if char is a letter. */
extern bool isalpha(int c);

/* True if char is white space. */
extern bool isspace(int c);

/* True if char is punctuation. */
extern bool ispunct(int c);

/* Returns char, converted to lowercase. */
extern int tolower(int c);

#endif /* __CTYPE_H__ */