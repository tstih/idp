/*
 * ef9367.h
 * 
 * a library of primitives for the thompson ef9367 card. 
 * 
 * notes:
 *  partner mixes text and graphics in a non-conventional way. images from 
 *  both sources are mixed on display, but remain independent of each other 
 *  i.e. clear screen in text mode will remove text from the display, 
 *  but leave all the graphics. and vice versa. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 04.04.2021   tstih
 * 
 */
#ifndef _EF9367_H
#define _EF9367_H

#include <stdint.h>

/* hires mode screen size */
#define GDP_HIRES_WIDTH     1024
#define GDP_HIRES_HEIGHT    512

/* initializes gdp, enter hires (1024x512) mode */
extern void gdp_init();

/* clear screen and goto 0,0 */
extern void gdp_cls();

/* goto x,y */
extern void gdp_xy(int16_t x, int16_t y);

/* pixel at x,y */
extern void gdp_setpixel(int16_t x, int16_t y);

#endif /* _EF9367_H */