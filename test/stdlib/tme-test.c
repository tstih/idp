/*
 * tme-test.c
 *
 * Standard C library time and msleep tests.
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
#include <stdint.h>
#include <unistd.h>

#include "leanmean.h"
#include "../test.h"

extern int all_tests();

static int tests_run = 0;

void main() {
#ifndef LEAN_AND_MEAN
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n");
    printf("Tests run: %d\n", tests_run);
#else
    printf("This library has been compiled with SLIM option and has no time.h\n");
#endif
}

#ifndef LEAN_AND_MEAN

/* set system date to 1.1.1980, 10:00:00 */
int setdatetime_test() {
    struct tm tim;
    tim.tm_year=10; /* years since 1970 ... 10 = 1980 */
    tim.tm_mon=1;
    tim.tm_mday=1;
    tim.tm_hour=10;
    tim.tm_min=0;
    tim.tm_sec=0;
    /* convert to time_t */
    time_t t=mktime(&tim);
    struct timeval tv = { t, 0 };
    /* We'd need to get date and assert on compare, 
       but the emulator can'd do it. */
    ASSERT(settimeofday(&tv)==0);
    return 0;
}

/* this test will fail only at the very end of 24 hour period! */
int clock_test() {
    long c1=clock();
    msleep(1000); /* cca 1 second */
    long c2=clock();
    printf("Start clock %lu, end clock %lu.\n", c1, c2);
    ASSERT(c1+100<c2 && c1+200>c2); /* must be in the vincinity */
    return 0;
}

/* this test will [most likely] fail if rollovers are not handled correctly */
int clock_rollover_test() {
    long c1=clock();
    for(int i=0;i<100;i++) {
        long c2=clock();
        ASSERT(c2>=c1);
        c1=c2;
    }
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
    printf("Current time is %s.\n", asctime(ptme));
    ASSERT(!strcmp(asctime(ptme),ctime(&t)));
    return 0;
}

extern uint8_t _dow(struct tm * ptim);
int mktime_test() {
    struct tm tim;
    /* First create time_t 25.05.2021 21:00:00 */
    tim.tm_sec=0;
    tim.tm_min=0;
    tim.tm_hour=21;
    tim.tm_mday=25;
    tim.tm_mon=4; 
    tim.tm_year=121;
    tim.tm_wday=_dow(&tim);
    time_t t=mktime(&tim);
    struct tm *ptm;
    ptm=gmtime(&t);
    ASSERT(!strcmp(asctime(ptm),"Tue May 25 21:00:00 2021"));
    return 0;
}

int all_tests() {
    VERIFY(clock_test);
    VERIFY(clock_rollover_test);
    VERIFY(time_test);
    VERIFY(asc_test);
    VERIFY(mktime_test);
    VERIFY(setdatetime_test);
    return 0;
}

#endif /* LEAN_AND_MEAN */