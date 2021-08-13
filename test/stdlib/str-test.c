/*
 * str-test.c
 *
 * Standard C library ctype and string tests.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.08.2021   tstih
 *
 */
#include <ctype.h>
#include <string.h>

#include "../test.h"

extern int all_tests();

static int tests_run = 0;

void main() {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n");
    printf("Tests run: %d\n", tests_run);
}

/* check the is*() functions of ctype */
int ctype_is_test() {
    /* isdigit */
    ASSERT(isdigit('0'));
    ASSERT(isdigit('9'));
    ASSERT(!isdigit('J'));
    ASSERT(!isdigit('!'));
    return 0;
}

/* uppercase, lowercase */
int ctype_uplow_test() {
    /* to upper */
    ASSERT(toupper('0')=='0');
    ASSERT(toupper('g')=='G');
    ASSERT(toupper('N')=='N');
    ASSERT(toupper('#')=='#');
    /* to lower */
    ASSERT(tolower('0')=='0');
    ASSERT(tolower('d')=='d');
    ASSERT(tolower('N')=='n');
    ASSERT(tolower('#')=='#');
    return 0;
}

int all_tests() {
    VERIFY(ctype_is_test);
    VERIFY(ctype_uplow_test);
    return 0;
}