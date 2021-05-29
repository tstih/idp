/*
 * gpx-xp.c
 *
 * Tests of graphical functions. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.03.2021   tstih
 *
 */
#include <conio.h>
#include <stdio.h>
#include <gpx.h>

extern void* zx6x8_font;

void main() {

    /* display warning */
    printf("WARNING: The screen will enter graphics mode\n\r");
    printf("         and go blank. When blank, press any\n\r");
    printf("         key to come back.\n\r\n\r");
    printf("Now press a key to enter graphics mode!\n\r");

    while (!kbhit());

    /* initialize display */
    graphics_t* g = gpx_init();

    /* exit graphics mode */
    gpx_exit(g);

    while (!kbhit());

    /* now display width and height. */
    printf("Hires width=%d, height=%d\n\r\n\r", 
        g->area.x1 - g->area.x0,
        g->area.y1 - g->area.y0);

}