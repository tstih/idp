#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


long
strtol(char *nptr, char **endptr, int base)
{
    long result;
    char *p = nptr;

    while (isspace(*p)) {
        p++;
    }
    if (*p == '-') {
        p++;
        result = -strtoul(p, endptr, base);
    }
    else {
        if (*p == '+') p++;
        result = strtoul(p, endptr, base);
    }
    if (endptr != 0 && *endptr == p) {
        *endptr = nptr;
    }
    return result;
}


const static char cvtIn[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,		/* '0' - '9' */
    100, 100, 100, 100, 100, 100, 100,		/* punctuation */
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,	/* 'A' - 'Z' */
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35,
    100, 100, 100, 100, 100, 100,		/* punctuation */
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,	/* 'a' - 'z' */
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};

unsigned long int
strtoul(char *string, char **endPtr, int base)
{
    register char *p;
    register unsigned long int result = 0;
    register unsigned digit;
    int anyDigits = 0;

    /*
     * Skip any leading blanks.
     */

    p = string;
    while (isspace(*p)) {
        p += 1;
    }

    /*
     * If no base was provided, pick one from the leading characters
     * of the string.
     */

    if (base == 0)
    {
        if (*p == '0') {
            p += 1;
            if (*p == 'x') {
                p += 1;
                base = 16;
            } else {

                /*
                 * Must set anyDigits here, otherwise "0" produces a
                 * "no digits" error.
                 */

                anyDigits = 1;
                base = 8;
            }
        }
        else base = 10;
    } else if (base == 16) {

        /*
         * Skip a leading "0x" from hex numbers.
         */

        if ((p[0] == '0') && (p[1] == 'x')) {
            p += 2;
        }
    }

    /*
     * Sorry this code is so messy, but speed seems important.  Do
     * different things for base 8, 10, 16, and other.
     */

    if (base == 8) {
        for ( ; ; p += 1) {
            digit = *p - '0';
            if (digit > 7) {
                break;
            }
            result = (result << 3) + digit;
            anyDigits = 1;
        }
    } else if (base == 10) {
        for ( ; ; p += 1) {
            digit = *p - '0';
            if (digit > 9) {
                break;
            }
            result = (10*result) + digit;
            anyDigits = 1;
        }
    } else if (base == 16) {
        for ( ; ; p += 1) {
            digit = *p - '0';
            if (digit > ('z' - '0')) {
                break;
            }
            digit = cvtIn[digit];
            if (digit > 15) {
                break;
            }
            result = (result << 4) + digit;
            anyDigits = 1;
        }
    } else {
        for ( ; ; p += 1) {
            digit = *p - '0';
            if (digit > ('z' - '0')) {
                break;
            }
            digit = cvtIn[digit];
            if (digit >= base) {
                break;
            }
            result = result*base + digit;
            anyDigits = 1;
        }
    }

    /*
     * See if there were any digits at all.
     */

    if (!anyDigits) {
        p = string;
    }

    if (endPtr != 0) {
        *endPtr = p;
    }

    return result;
}
