/*
 * gpx-test.c
 *
 * Tests of graphical functions. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.03.2021   tstih
 *
 */
#include "gpx.h"

extern void* zx6x8_font;

void main() {

    /* initialize display */
    graphics_t* g;
    g=graphics_init();

    /* fill the screen with lines */
    int y;
    for (y=0;y<SCREEN_HEIGHT-1;y+=64)
        draw_line(g,0,y,SCREEN_WIDTH-1,y, 0, 0xaa);

}