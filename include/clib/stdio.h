/*
 * stdio.h
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
#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#define BUFSIZ      512
#define _NFILE      8

#define EOF         0x1A	
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define uchar unsigned char

struct  _iobuf {
  char*     _ptr;
  int       _cnt;
  char*    _base;
  char      _flags[4];
  int       _file;
  bool 	    _eof;
  ssize_t   _limit;
}; 

typedef struct _iobuf FILE;
#define stdin           (&filehandles[0])
#define stdout          (&filehandles[1])
#define stderr          (&filehandles[2])

/* TODO:
extern int fprintf(FILE *stream, const char *format, ...);
extern int snprintf(char *str, size_t size, const char *format, ...);
extern char *getenv(const char *name);
extern int fflush(FILE *stream);
extern void perror(const char *s);
extern int ferror(FILE *stream);
extern int getchar(void);
extern void cpm_putchar(char c);
*/
extern FILE *fopen(const char *path, const char *mode);
extern int fseek(FILE *stream, long offset, int whence);
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern int feof(FILE *stream);
extern int fclose(FILE *stream);
extern long ftell(FILE *stream);
extern size_t fwrite(
    const void *ptr, 
    size_t size, 
    size_t nmemb, 
    FILE *stream);
extern int puts(const char *s);
extern int vsprintf(char **out, char *format, va_list ap);
extern int printf(char *fmt, ...);
extern int sprintf(char *buf, char *fmt, ...);
extern void putchar(int c);

#endif /* __STDIO_H__ */
