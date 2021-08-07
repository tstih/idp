/*
 * libcpm3-z80-partner.c
 *
 * Platform specific functions for z80-partner platform.
 * 
 * TODO:
 *  Add time zone info. Currently it's being ignored.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 06.08.2021   tstih
 *
 */
#include <stdbool.h>
#include <time.h>
#include <partner.h>

#include <util/mm58167a.h>

/* initialize partner library, called from the standard library! */
void libinit() {}

/* Non standard function to sleep (in milliseconds) */
void msleep(int millisec) {
    millisec;
    /* For 1 millisec we need to sleep 
       0.001/(1/4000000)=4000 t-states,
       but  
        ...outer loop adds 38 t-states 
        ...push and pop commands add 42 t-states to everything 
       so we need 3962 t-states for 1ms + outer loop overhead */
    __asm
        pop     de                      ; return address
        pop     hl                      ; milliseconds
        ;; restore stack
        push    hl 
        push    de
        ;; now count hl
    msl_loop:
        ld      b,#233                  ; 7 t-states outer counter 
        ;; inner loop is 17 * 233 t-states = 3961 t-states
    msl_inner_loop:
        nop                             ; 4 t-states
        djnz    msl_inner_loop          ; 13/8 t-states
        ;; and loop.
        ;; this loop and ld b adds 38 t-states to each millisecond
        dec     hl                      ; 6 t-states
        ld      a,h                     ; 9 t-states
        or      l                       ; 4 t-states
        jr      nz,msl_loop             ; 12/7 t-states
    __endasm;
}

/* Non standard function to set system date and time */
int settimeofday(const struct timeval *tv) {
    
    /* Convert time_t to date. */
    struct tm* tmi = gmtime( &(tv->tv_sec) );

    /* NOTE: There should be no interrupts while we're 
       doing this, but we don't know where this function
       will be called from...so we just can't disable and
       enable interrupts (maybe they need to stay disabled?) */

    /* Issue go command to reset everything
       less significant than the minutes to zero. */
    RTC_GO      = 0xff;

    /* Now set values, starting from largest */
    RTC_YEAR    = bin2bcd((tmi->tm_year)%100);
    RTC_MONTH   = bin2bcd(tmi->tm_mon+1);
    RTC_MDAY    = bin2bcd(tmi->tm_mday);
    RTC_HOUR    = bin2bcd(tmi->tm_hour);
    RTC_MINUTE  = bin2bcd(tmi->tm_min);
    RTC_SECOND  = bin2bcd(tmi->tm_sec);

    /* Done. */
    return 0;
}

/* Get date and time (resolution: 1/100 s) */
int gettimeofday(struct timeval *tv) {

    /* We should read the status bit here to reset it,
       but it does not work in emulator so our hack is
       to read second twice. */ 
    struct tm tim;
    uint8_t y;

    while(true) {
        /* Get hundreds. */
        tv->tv_hsec=bcd2bin(RTC_HUNDR_S);

        /* Get date and time, but no year */
        tim.tm_sec=bcd2bin(RTC_SECOND);
        tim.tm_min=bcd2bin(RTC_MINUTE);
        tim.tm_hour=bcd2bin(RTC_HOUR);
        tim.tm_mday=bcd2bin(RTC_MDAY);
        tim.tm_mon=bcd2bin(RTC_MONTH)-1; /* Normalize */

        /* Read fake year from nvram, and normalize it. */
        y=bcd2bin(RTC_YEAR);
        if (y<70) y+=100;
        tim.tm_year=y;

        /* Loop if rollover! */
        if (bcd2bin(RTC_SECOND)==tim.tm_sec) break;
    }

    /* We did it! Convert time to date and return success. */
    tv->tv_sec = mktime(&tim);
    return 0;
}