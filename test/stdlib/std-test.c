/*
 * std-test.c
 *
 * Standard C library tests.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 22.05.2021   tstih
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../test.h"

extern int all_tests();

static int tests_run = 0;

void main() {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n\r");
    printf("Tests run: %d\n\r", tests_run);
}

/* set system date to 1.1.1980, 10:00:00 */
int setdatetime_test() {
    struct tm tme;
    tme.tm_year=10; /* 1980 */
    tme.tm_mon=1;
    tme.tm_mday=1;
    tme.tm_hour=10;
    tme.tm_min=0;
    tme.tm_sec=0;
    setdatetime(&tme);
    return 0;
}

/* this test will fail only at the very end of 24 hour period! */
int clock_test() {
    long c1=clock();
    for(int i=0;i<10000;i++) {} /* Delay */
    long c2=clock();
    ASSERT(c1<c2);
    return 0;
}

/* will block if it doesn't work */
int time_test() {
    time_t t=time(NULL);
    for (int j=0;j<5;j++)
        for(int i=0;i<30000;i++) {} /* Delay */
    time_t t2=time(NULL);
    ASSERT(t<t2);
    return 0;
}

/* should print current partner date 1.1.1980, 10:xx:xx, but not in emulator */
int asc_test() {
    time_t t=time(NULL);
    struct tm * ptme=gmtime(&t);
    ASSERT(!strcmp(asctime(ptme),ctime(&t)));
    return 0;
}

int mktime_test() {
    struct tm tme;
    /* First create time_t 24.05.2021 21:00:00 */
    tme.tm_sec=0;
    tme.tm_min=0;
    tme.tm_hour=21;
    tme.tm_mday=24;
    tme.tm_mon=4; /* this is really may, 0 based */
    tme.tm_year=121;
    time_t t=mktime(&tme);
    struct tm *ptm;
    ptm=localtime(&t);
    ASSERT(!strcmp(asctime(ptm),"Mon May 24 21:00:00 2021"));
    return 0;
}

int all_tests() {
    VERIFY(setdatetime_test);
    VERIFY(clock_test);
    VERIFY(time_test);
    VERIFY(asc_test);
    VERIFY(mktime_test);
    return 0;
}