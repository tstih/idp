/*
 * display.c
 * 
 * functions for working with display.
 *
 * tomaz stih tue mar 23 2021
 * 
 */
#include <stddef.h>

#include "hw/ef9367.h"
#include "hw/scn2674.h"
#include "hw/z80_pio.h"
#include "display.h"

display_t display;

display_t* display_init() {

    /* clear screen, hide cursor, disable tty */
    scn2674_reset();

    /* init graphic mode */
    ef9367_wait_sts_ready();
    ef9367_set_1024x512();

    /* clear screen */
    ef9367_cls();

    /* go to top, left corner. in theory initializing graphical
       mode should move cursor to 0,0. but idp's origin is left, 
       bottom corner, and function is needed to reverse the 
       coordinates! it calls x=0,y=512 on the hardware. */
    ef9367_xy(0,0); 

    display.width=MAXX;
    display.height=MAXY;

    return &display;
}