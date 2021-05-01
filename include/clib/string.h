/*
 * string.h
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
#ifndef __STRING_H__
#define __STRING_H___

#include <stddef.h>

#ifndef NULL
#define NULL ( (void *) 0)
#endif /* NULL */

extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memchr(const void *s, int c, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);

extern size_t strlen(const char *s);
extern char *strcpy(char *dest, const char *src);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern int strcmp(const char *s1, const char *s2);
extern char *strcpy(char *dest, const char *src);
extern char *strncpy(char *dest, const char *src, size_t n);
extern char *strchr(const char *s, int c);
extern char *strerror(int errnum);
extern char *strstr(const char *haystack, const char *needle);
extern char *strsep(char **stringp, const char *delim);
extern char *strrchr(const char *s, int c);
extern char *strcat(char *dest, const char *src);

#endif /* __STRING_H__ */