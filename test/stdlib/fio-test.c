/*
 * fio-test.c
 *
 * Standard C library file I/O tests.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.08.2021   tstih
 *
 */
#include <unistd.h>
#include <stdlib.h>

#include "../test.h"

extern int all_tests();

static int tests_run = 0;

void main() {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n\r");
    printf("Tests run: %d\n\r", tests_run);
}

/* Should parse filenames correctly 
   filname is */
int fname_parse_test() {
    
    /* result placeholders */
    int user;
    char fname[MAX_FNAME+1];
    char drive;
    char ext[MAX_EXT+1];
    
    /* must fail - invalid chars*/
    const char *fn1=":TEST.C[0]";
    ASSERT(splitpath(fn1, &drive, &user, fname, ext));
 
    /* must fail - too long */
    const char *fn2="jojorabbit.txt";
    ASSERT(splitpath(fn2, &drive, &user, fname, ext));

    /* must succeed */
    const char *fn3="P:TEST.C[12]";
    ASSERT(!splitpath(fn3, &drive, &user, fname, ext));

    /* must succeed */
    const char *fn4="MAMAMIA.TXT";
    ASSERT(!splitpath(fn4, &drive, &user, fname, ext));

    return 0;
}

int all_tests() {
    VERIFY(fname_parse_test);
    return 0;
}