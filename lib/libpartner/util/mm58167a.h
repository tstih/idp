/*
 * mm58167a.c
 *
 * Partner real time clock definitions.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 06.08.2021   tstih
 *
 */
#ifndef __MM58167A_H__
#define __MM58167A_H__

/* Partner ports for the MM58167a clock */
__sfr __at 0xa0 RTC_THOUS_S;
__sfr __at 0xa1 RTC_HUNDR_S;
__sfr __at 0xa2 RTC_SECOND;  
__sfr __at 0xa3 RTC_MINUTE;
__sfr __at 0xa4 RTC_HOUR;
__sfr __at 0xa5 RTC_WDAY;
__sfr __at 0xa6 RTC_MDAY;  
__sfr __at 0xa7 RTC_MONTH;
/* fake 2 digit year, from CMOS memory */
__sfr __at 0xa9 RTC_YEAR;

/* RESET non volatile RAM */
__sfr __at 0xb3 RTC_RESET_NVRAM;
/* bit 0 of status is rollover and should be 1 for valid read
   NOTE: emulators don't know about this so we use trick with
   reading seconds twice */
__sfr __at 0xb4 RTC_STATUS;
/* RESET clock (before setting) */
__sfr __at 0xb5 RTC_GO;

#endif /* __MM58167A_H__ */