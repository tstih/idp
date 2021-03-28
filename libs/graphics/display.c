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
#include "display.h"

display_t* display_init() {

    /* clear screen, hide cursor, disable tty */
    scn2674_reset();

    /* using z80 pio call to enter 1024x512 hires mode */

    return NULL;
}