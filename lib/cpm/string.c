/*
 * string.c
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
#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>


char *strerror(int en)
{
    switch(en) {
    case 0:
        return (const char *) "Success";
        break;
    case ENOENT:
        return (const char *) "No such file or directory";
        break;
    case EIO:
        return (const char *) "I/O error";
        break;
    case E2BIG:
        return (const char *) "Argument list too long";
        break;
    case EBADF:
        return (const char *) "Bad file descriptor";
        break;
    case EINVAL:
        return (const char *) "Illegal address";
        break;
    case ENFILE:
        return (const char *) "Too many open files";
        break;
    case ENOTTY:
        return (const char *) "Not a typewriter";
        break;
    case EPIPE:
        return (const char *) "Transport endpoint not connected";
        break;
    case EAGAIN:
        return (const char *) "Try again";
        break;
    }
    /* if we're here, we're toast */
    return "Unknown error!";
}


char * strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}


char *strrchr(char *s, int c)
{
    char *ptr = (const char *) s;
    char *retptr = NULL;

    while (ptr[0] != '\0') {
        if (ptr[0] == c) {
            retptr = (char *) ptr;
        }
        ptr ++;
    }
    return retptr;
}

#endif /* __STRING_H__ */