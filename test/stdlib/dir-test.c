/*
 * dir-test.c
 *
 * Testing dirent.h.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 10.08.2021   tstih
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include "../test.h"

extern int all_tests();

static int tests_run = 0;

void main() {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n");
    printf("Tests run: %d\n", tests_run);
}

/* Enumerate all files on B:*[*]. */
int enum_files_test() {

    /*
    DIR dir = opendir("B:*[*]");
    struct dirent *d;
    while (d=readdir(dir)) != NULL)
        printf("%s\n", d->d_name;
    */
    return 0;
}

int all_tests() {
    VERIFY(enum_files_test);
    return 0;
}