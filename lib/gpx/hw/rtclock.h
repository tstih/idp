/*
 * rtclock.h
 * 
 * header for the real time clock functions of id partner
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.04.2021   tstih
 * 
 */
#ifndef _RTCLOCK_H
#define _RTCLOCK_H

#include "yos.h"

typedef struct datetime_s {
    byte_t second;
    byte_t minute;
    byte_t hour;                        /* 24 format */
    byte_t day;
    byte_t month;
    word_t year;                        /* 2000-2099 */ 
} datetime_t;

/* read current date and time */
extern datetime_t* rtc_read(datetime_t*);

/* write current date and time */
extern void rtc_write(datetime_t*);

#endif /* _RTCLOCK_H */