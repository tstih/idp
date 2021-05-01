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
    char *p = (const char *) s;
    while (p[0] != '\0') {
        cpm_putchar(p[0]);
        p++;
    }
    return 0;
}