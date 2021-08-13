/*
 * gpx2-xp.c
 *
 * Graphical functions test.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 12.08.2021   tstih
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <gpx.h>


void main() {

    /* enter graphics */
    gpx_t *g=gpx_init();

    /* clear screen */
    gpx_cls(g);

    /* query graphics capabilities
       NOTE: this is only possible because initial page
             and initial resolution are both 0. */
    gpx_cap_t *cap=gpx_cap(g);
    coord centerx = cap->pages[0].resolutions[0].width/2;
    coord centery = cap->pages[0].resolutions[0].height/2;
    coord offs=centery/8;

    printf("Center is at %d,%d; offset is %d\n",
        centerx, centery, offs);

    /* draw line */
    for (coord x=centerx-offs; x<centerx+offs;x++)
        gpx_draw_pixel(g,x,centery);

    /* draw circle */
    gpx_draw_circle(g,centerx,centery,offs);

    /* leave graphics */
    gpx_exit(NULL);
}