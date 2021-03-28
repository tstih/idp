/*
 * ef9367.h
 * 
 * a library of graphical primitives for the thompson ef9367 card. 
 * 
 * partner mixes text and graphics in a non-conventional way. images from 
 * both sources are mixed on display, but remain independent of each other 
 * i.e. clear screen in text mode will remove text from the display, 
 * but leave all the graphics. and vice versa. 
 *
 * tomaz stih tue mar 23 2021
 * 
 */
#ifndef _EF9367_H
#define _EF9367_H

#include <stdint.h>

/* ----- graphics card registers ------------------------------------------- */

/* the status register and its bitmask */
#define EF9367_STS 0x2f             /* status register */
#define EF9367_STS_LPEN 0x01        /* low when executing light pen seq. */
#define EF9367_STS_VBLANK 0x02      /* high during vertical blanking. */
#define EF9367_STS_READY 0x04       /* low when executing command */
#define EF9367_STS_OUTSCR 0x08      /* high when x or y is out of screen */     

/* PIO */
#define PIO_CCT 0x30                /* common control (PIO) register */


/* ----- graphic functions ------------------------------------------------- */

/* initialize */
extern void ef9367_init();

/* wait until current command is executed, returns status register */
extern uint8_t  ef9367_wait_ready() __naked;

#endif /* _EF9367_H */