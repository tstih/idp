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

extern void ef9367_put_pixel(int16_t x, int16_t y);

/* the system font address */
extern void system8x16_font;

void wait() {
    while (!kbhit());
}

void main() {

    /* display warning */
    printf("WARNING: The screen will enter  graphics mode\n\r");
    printf("         There will be no text. Press any key\n\r");
    printf("         to continue to the next experiment.\n\r\n\r");
    printf("Now press a key to start the first experiment.\n\r");

    wait();

    /* initialize display */
    graphics_t* g = gpx_init();

    /* Slow lines */
    for(int y=0;y<512;y+=16)
        for(int x=0;x<1024;x++)
                ef9367_put_pixel(x,y);

    wait();

    gpx_cls(g);
    
    char *lorem="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque laoreet imperdiet ligula, ac auctor magna mollis eu vestibulum";
    font_t *f=(font_t *)&system8x16_font;
    for (int yc=0;yc<32;yc++) gpx_draw_text(g,lorem,f,0,16*yc);

    wait();

    /* exit graphics mode */
    gpx_exit(g);

    /* now display width and height. */
    printf("Hires width=%d, height=%d\n\r\n\r", 
        g->area.x1 - g->area.x0,
        g->area.y1 - g->area.y0);
}