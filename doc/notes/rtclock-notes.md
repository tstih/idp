# Notes from 22. may 2021

## The real time clock hardware

Partner uses the MM58167A real time clock chip. It has 32 registers which can be
accesset at i/o addresses (ports) from 0xa0 to 0xbf:

| port | description |
|------|-------------------------------------|
| 0xa0 | 1/1000 sec (some sources: 1/10000) |
| 0xa1 | 1/100 sec |
| 0xa2 | sec |
| 0xa3 | min |
| 0xa4 | hour |
| 0xa5 | weekday |
| 0xa6 | month day |
| 0xa7 | month |
| 0xa9 | year* |
|||

All digits are in BCD. According to some sources the value at address 0xa0
is 1/1000 of a second and can only have values from 00 - 09. Other sources
say that this is 1/10000 of a second and can have values up to 99.

**TODO:** Test it.

### Year handling

MM58167A does not track the year. The value at 0xa9 is "fake" year tracked
by the Partner software. In fact, ports from 0xa8 to 0xaf (incl) are battery
powered RAM locations (also provided by the MM58167A). The MM58167A can use
these locations for raising an alert (via interrupt), but Partner uses them
as storage for year.

**TODO:** We also suspect SET UP program uses these to store system 
settings, but this still needs to be confirmed via tests.

## Links (to hardware and standard C library implementations)

 *  https://www.oryx-embedded.com/doc/date__time_8c_source.html
 
 *  https://www.geeksforgeeks.org/time-h-header-file-in-c-with-examples/
    
 *  http://sourceware.org/git/?p=glibc.git;a=tree;f=time;h=c950c5d4dd90541e8f3c7e1649fcde4aead989bb;hb=master
   
 *  https://www.unige.ch/medecine/nouspikel/ti99/mbp.htm
   
 *  https://www.unige.ch/medecine/nouspikel/ti99/mbp.htm#Acessing%20clock