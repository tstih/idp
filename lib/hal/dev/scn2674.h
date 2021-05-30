/*
 * scn2674.h
 * 
 * header for text mode (only) primitives for the signetics scn2674 card.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 23.03.2021   tstih
 * 
 */
#ifndef _SCN2674_H
#define _SCN2674_H

/* initialize the scn2674, set to default mode 80x25 */
extern void scn2674_init();

/* show cursor */
extern void scn2674_cursor_on();

/* hide cursor */
extern void scn2674_cursor_off();

/* clear screen */
extern void scn2674_cls();

#endif /* _SCN2674_H */