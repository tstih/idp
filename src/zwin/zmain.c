/*
 * zmain.c
 *
 * entry, exit and loop functions z windows 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.03.2021   tstih
 *
 */
#include "gpx.h"

void main() {

    /* initialize display */
    graphics_t* g;
    g=graphics_init();

    /* do some drawing */
    int y;
    for (y=0;y<SCREEN_HEIGHT-1;y+=16)
        draw_line(g,0,y,SCREEN_WIDTH-1,y, 0, 0xaa);

    /* and exit */
}