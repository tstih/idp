/*
 * puts.c
 *
 * idp standard C library implementation
 * 
 * NOTES:
 *  the idp standard library is based on the cpmlibc
 *  https://github.com/dmo9000/cpmlibc
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.04.2021   tstih
 *
 */
#include <cpm_sysfunc.h>

int puts(char *s)
{
    while (*s)
        cpm_putchar(*s++);
    return 0;
}