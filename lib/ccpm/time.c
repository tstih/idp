/*
 * time.c
 *
 * Time functions: asctime(), clock(), ctime(), difftime(), 
 * gmtime(), mktime(), time(), settimmeofday().
 * 
 * LINKS:

 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 22.05.2021   tstih
 *
 */
#include <time.h>
#include <port.h>
#include <bcd.h>
#include <string.h>
#include <stdio.h>

/* Partner ports */
#define THOUS_S 0xa0
#define HUNDR_S 0xa1
#define SECOND  0xa2
#define MINUTE  0xa3
#define HOUR    0xa4
#define WDAY    0xa5
#define MDAY    0xa6
#define MONTH   0xa7
#define YEAR    0xa9 /* fake 2 digit year, from CMOS memory */

/* strings */
static const char *_days[7] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

static const char *_months[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

/* buffers for functions returning pointers */
static char _at[26]; 
static struct tm _tm;

/* Service function to calculates day of week */
uint8_t _dow(uint16_t y, uint8_t m, uint8_t d) {
    uint16_t h,j,k;
    y+=1900; /* year from tm format to real year! */
    m++; /* month from 0-11 to 1-12 */
    if(m <= 2) { m += 12; y -= 1; }
  
    /* j is the century */
    j = y / 100;
    /* k the year of the century */
    k = y % 100;
  
    /* Compute H using Zeller's congruence. */
    h = d + (26 * (m + 1) / 10) + k + (k / 4) + (5 * j) + (j / 4);
  
    /* And return the day of the week. */
    return ((h + 5) % 7) + 1;
}

/* Service function to calculate day of year */
uint8_t _doy(uint16_t y, uint8_t m, uint8_t d) {
    uint8_t dm[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if( (y % 4 == 0 && y % 100 != 0 ) || (y % 400 == 0) )
        dm[1]=29; /* leap year */
    uint8_t days=0;
    for (int i=0;i<m;i++) 
        days+=dm[i];
    days+=d;
    return days;
}

/* Converts given calendar time tm to a textual representation of 
the following fixed 25-character form: Www Mmm dd hh:mm:ss yyyy\n */
char* asctime(const struct tm* pt) {

    char www[4], mmm[4];

    /* copy Www first... */
    strncpy(www,_days[(pt->tm_wday)-1],3);

    /* Now Mmm */
    strncpy(mmm,_months[pt->tm_mon],3);

    /* and now format... */
    sprintf(_at,"%s %s %02d %02d:%02d:%02d %04d", www, mmm, pt->tm_mday,
        pt->tm_hour, pt->tm_min, pt->tm_sec,
        1900 +  pt->tm_year
        );

    return &(_at[0]); /* return ptr to internal buffer (unsafe, I know) */
}

/* Return current clock in 1/1000 seconds */
clock_t clock(void) {
    long clk = (_bcd2bin(_port_read(THOUS_S))>>4) +    /* 1/1000 s */
        (_bcd2bin(_port_read(HUNDR_S))*10) +           /* 1/100 s */
        (_bcd2bin(_port_read(SECOND))*1000) +         /* s */
        (_bcd2bin(_port_read(MINUTE))*(long)60000) +  /* min */
        (_bcd2bin(_port_read(HOUR))*(long)3600000); /* hrs */
    return clk;
}

/* Convert current time to textual representation using the following
format Www Mmm dd hh:mm:ss yyyy (uses asctime...)*/
char* ctime(const time_t* ptt) {
    return asctime(localtime(ptt));
}

/* Returns difference between two time points in seconds! */
long difftime(time_t time_end,time_t time_beg) {
    return time_end-time_beg;
}

/* Get Greenwich mean time (politically correct: UTC) */
struct tm *gmtime(const time_t *timer) {
    uint32_t a, b, c, d, e, f;
    time_t t=*timer;
    if(t < 1) t = 0;
    /* t is in seconds, extract hms */
    _tm.tm_sec=t%60; t/=60;
    _tm.tm_min=t%60; t/=60;
    _tm.tm_hour=t%24; t/=24;
    /* remained is the date */
    a = (uint32_t) ((4 * t + 102032) / 146097 + 15);
    b = (uint32_t) (t + 2442113 + a - (a / 4));
    c = (20 * b - 2442) / 7305;
    d = b - 365 * c - (c / 4);
    e = d * 1000 / 30601;
    f = d - e * 30 - e * 601 / 1000;
    /* jan and feb are months 13 and 14 */
    if(e <= 13) { c -= 4716; e -= 1; }
    else { c -= 4715; e -= 13; }
    /* and copy to struct */
    _tm.tm_year=c-1900; /* C std: tm_year is years from 1900 */
    _tm.tm_mon=e-1; /* 0-11 */
    _tm.tm_mday=f;
    /* and calculated values... */
    _tm.tm_wday=_dow(_tm.tm_year,_tm.tm_mon, _tm.tm_mday);
    _tm.tm_yday=_doy(_tm.tm_year,_tm.tm_mon, _tm.tm_mday);
    _tm.tm_isdst=-1; /* 1=DST, 0=std. time, -1=not known */
    /* return internal static structure */
    return &_tm;
}

/* Create time_t given tm structure */
time_t mktime(struct tm *tme) {
    long y,m,d;
    time_t t;
    y = 1900 + tme->tm_year; /* tm_year is no. of years since 1900 */
    m = 1 + tme->tm_mon; /* needs to be 1 based for this calculation */
    d = tme->tm_mday;
    /* trick...count jan and feb as months 13 and 14 of the prev. year */
    if (m<=2) { m+=12; y-=1; }
    /* first years to days */
    t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
    /* now add months as days */
    t += ((30 * m) + (3 * (m + 1) / 5) + d);
    /* first date is jan 1st 1970 */
    t -= 719561;
    /* convert total days to seconds! */
    t *= 86400;
    /* finally, add hours, minutes and seconds */
    t += (3600 * tme->tm_hour) + (60 * tme->tm_min) + tme->tm_sec;
    /* and return */
    return t;
}
	
/* Get current time. */
time_t time(time_t *arg) {

    /* init... */
    struct tm tme;

    /* populate from ports */
    tme.tm_sec=_bcd2bin(_port_read(SECOND));
    tme.tm_min=_bcd2bin(_port_read(MINUTE));
    tme.tm_hour=_bcd2bin(_port_read(HOUR));

    tme.tm_mday=_bcd2bin(_port_read(MDAY));
    tme.tm_mon=_bcd2bin(_port_read(MONTH))-1; /* Normalize */

    /* read fake year from nvram */
    uint8_t y=_bcd2bin(_port_read(YEAR));
    if (y<70) y+=100;
    tme.tm_year=y;

    /* convert */
    time_t t = mktime(&tme);

    /* copy to arg if not NULL? */
    if (arg!=NULL)
        *arg=t;

    /* and return it */
    return t;
}

/* Non standard function to set system time. */
void setdatetime(struct tm *tme) {

    /* reset counters */
    _port_write(0xb2,0xff);

    /* and set all values */
    _port_write(YEAR,_bin2bcd((tme->tm_year)%100));
    _port_write(MONTH,_bin2bcd(tme->tm_mon+1));
    _port_write(WDAY,_bin2bcd(_dow(tme->tm_year,tme->tm_mon, tme->tm_mday)));
    _port_write(MDAY,_bin2bcd(tme->tm_mday));
    _port_write(HOUR,_bin2bcd(tme->tm_hour));
    _port_write(MINUTE,_bin2bcd(tme->tm_min));
    _port_write(SECOND,_bin2bcd(tme->tm_sec));
}