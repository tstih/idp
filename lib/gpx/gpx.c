/*
 * gpx.c
 *
 * Graphics init and exit functions.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 29.95.2021   tstih
 *
 */
#include <hal.h>
#include <gpx.h>

graphics_t screen;

graphics_t* gpx_init() {

    /* initialize hires graphics */
    hal_hires_init();

    /* get its resolution */
    int w,h;
    hal_hires_info(&w,&h);

    /* initialize structure */
    screen.area.x0=screen.area.y0=screen.clip.x0=screen.clip.y0=0;
    screen.area.x1=screen.clip.x1=w-1;
    screen.area.y1=screen.clip.y1=h-1; 

    /* and return it */
    return &screen;
}

void gpx_exit(graphics_t *g) {
    hal_hires_exit();
}