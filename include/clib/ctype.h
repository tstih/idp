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

extern int isprint(int c);
extern int isspace(int c);
extern int isdigit(int c);
extern int isgraph(int c);
extern int toupper(int c);
extern int tolower(int c);

#endif /* __CTYPE_H__ */