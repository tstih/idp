/*
 * stdarg.h
 *
 * standard C header file
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.05.2021   tstih
 *
 */
#ifndef __STDARG_H__
#define __STDARG_H__

#define va_list                 unsigned char *
#define va_start(ap,last)       ap=(getarg()*2)+&last-5
#define va_arg(ap,type)         (*(type*)(ap-=sizeof(type),ap+1))
#define va_copy(dst, src)       dst = src
#define va_end(ap)
#define va_ptr(ap,type)         (*(type*)(ap+1))

#endif /* __STDARG_H__ */
