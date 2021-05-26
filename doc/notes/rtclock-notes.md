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


# Notes from 24. may 2021

## NVRAM usage

Non-volatile RAM (battery powered RAM) from `0xa8` to incl. `0xaf` is indeed used by SET UP. It covers ports: `0xab`, `0xac`, `0xae`, `0xaf`. `0xa9` is used to store curent year (last 2 digits i.e. 21 for 2021). Usage of ports `0xa8`, `0xaa`, and `0xad` is not known at the moment.

Following was decoded and reported by Miha Grcar.

 * Tabulators - **not saved**
     + Clear all tabs
     + Set all tabs to 8
 * Term. & Kbd. 
     + Terminal type
		- Partner (ab=1X) ... on emulator this is 2X
		- VT 52 (ab=2X) ...on emulator this is 1X
		- Ansi (ab=0X)
     + Terminal language
		- Usascii (ab=X0)
		- Ukascii
		- Spanish
		- French
		- Greman
		- Italian
		- Danish
		- Swedih (ab=X7) 
		- Yugoslav (ab=X8)
     + Key click bit 3
		- Yes (af=X0) XXXX 0000
		- No (af=X8)  XXXX 1000
     + Autorepeat bit 5
		- Yes (af=8X) 1000 XXXX
		- No (af=aX)  1010 XXXX
     + Keyboard type      b7 
		- Querty (af=28) 0010 1000
		- Quertz         1010 1000
 * Screen
     + Screen width    
		- 80 ac=51 (dec 81)   
		- 132 ac=85 (dec 133)
     + Screen background bit 0
		- Normal ae=06   0000 0110
		- Reverse ae=07  0000 0111
     + Auto new line bit 2
		- Yes ae=06 0000 0110
		- No ae=02  0000 0010
     + Auto wrap_around  bit 1
		- Yes ae=02  0000 0010
		- No ae=00   0000 0000

Port `0xa8` is always `0x00` on real Partner, but `0xf0` on the emulator.

## Port 0xa0 decoded

Port `0xa0` is 1/1000 of a second counter. The confusion between 1/1000 (hardware doc) and 1/10000 (Partner doc) is caused by this port always having lower nibble 
equal to 0. Hence, only values `0x00`, `0x10`, `0x20`, `0x30`... are possible. This value is updated every 1/1000 of a second.

## How to set the clock?

## Links

https://github.com/nlitsme/nlitsme.github.io/blob/e6e7c1e155bfa7426dd7c922b3dadf9bd3dd72a8/1994/03/1995-03-01-various-hardware/port.wj

https://github.com/pbetti/ZDS/blob/5943345e48957fdf7cc88465fe0b4a266160d803/software/MIXED-FULL/DATESUBS.C

https://github.com/freecores/System09/blob/699c84a13e7a6e96d3cb1560317528787abc1b26/src/sys09bug/sys09equ.asm

# Notes from 25.

## Implementation Links

http://www.jbox.dk/sanos/source/lib/time.c.html

http://www.ethernut.de/api/gmtime_8c_source.html

https://doxygen.reactos.org/d6/da2/gmtime_8c.html

