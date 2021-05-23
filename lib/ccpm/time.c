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
#include <string.h>

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
static const char _days[7][10] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

static const char _months[12][10] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"};

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
    static char at[26]; /* internal buffer */
    
    /* init with 25 spaces */
    memset(&(at[0]),' ',25);

    /* copy Www first... */
    strncpy(&(at[0]),_days[pt->tm_wday],3);

    /* Now Mmm */
    strncpy(&(at[5]),_months[pt->tm_mon],3);

    /* and now, the numerics */

    return &(at[0]); /* return ptr to internal buffer (unsafe, I know) */
}

/* Return current clock in 1/1000 seconds */
clock_t clock(void) {

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
    static struct tm tmret;
    uint32_t a, b, c, d, e, f;
    time_t t=*timer;
    if(t < 1) t = 0;
    /* t is in seconds, extract hms */
    tmret.tm_sec=t%60; t/=60;
    tmret.tm_min=t%60; t/=60;
    tmret.tm_hour=t%24; t/=24;
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
    tmret.tm_year=c-1900; /* C std: tm_year is years from 1900 */
    tmret.tm_mon=e-1; /* 0-11 */
    tmret.tm_mday=f;
    /* and calculated values... */
    tmret.tm_wday=_dow(tmret.tm_year,tmret.tm_mon, tmret.tm_mday);
    tmret.tm_yday=_doy(tmret.tm_year,tmret.tm_mon, tmret.tm_mday);
    tmret.tm_isdst=-1; /* 1=DST, 0=std. time, -1=not known */
    /* return internal static structure */
    return &tmret;
}

/* Return local time (for now it is equal to gmt). */


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

    /* read bcds */

    /* and return it */
    return *arg;
}

/* Non standard function to set system time. */
void settimeofday(time_t time) {

}
