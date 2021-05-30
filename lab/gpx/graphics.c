/*
 * graphics.c
 *
 * functions that manipulate the graphics_t structure.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.04.2021   tstih
 *
 */
#include "graphics.h"

graphics_t screen;

graphics_t* graphics_init() {
    
    /* initialize screen area and clipping */
    screen.area.x0=screen.area.y0=screen.clip.x0=screen.clip.x1=0;
    screen.area.x1=screen.clip.x1=SCREEN_WIDTH-1;
    screen.area.y1=screen.clip.y1=SCREEN_HEIGHT-1;
    
    /* first reset the avdc (text mode). clear screen and hide cursor */
    avdc_init();
    avdc_cls();
    avdc_cursor_off();

    /* now initializa gdp (hires mode), clear screen and go to 0,0 */
    gdp_init();
    gdp_cls();
    gdp_xy(0,0);
    
    return &screen;
}