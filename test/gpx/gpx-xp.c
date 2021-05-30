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
extern void hal_hires_put_raster(
    uint8_t *raster,
    int16_t x, 
    int16_t y, 
    uint8_t width,
    uint8_t height);

/* the system font address */
extern void system8x16_font;
extern void dagger_font;
extern void radon_font;
extern void tut_bitmap;

void wait() {
    while (!kbhit());
}

font_t *f;

void main() {

    /* display warning */
    printf("WARNING: The screen will enter  graphics mode\n\r");
    printf("         There will be no text. Press any key\n\r");
    printf("         to continue to the next experiment.\n\r\n\r");
    printf("Now press a key to start the first experiment.\n\r");
    wait();


    /* initialize display */
    graphics_t* g = gpx_init();

    /* very slow non-optimized lines */
    for(int y=0;y<512;y+=16)
        for(int x=0;x<1024;x++)
                ef9367_put_pixel(x,y);
    wait();


    /* full screen of text, raster non/optimized font */
    gpx_cls(g);
    char *lorem="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque laoreet imperdiet ligula, ac auctor magna mollis eu vestibulum";
    f=(font_t *)&system8x16_font;
    for (int yc=0;yc<32;yc++) gpx_draw_text(g,lorem,f,0,16*yc);
    wait();


    /* the 3 font demo */
    gpx_cls(g);
    gpx_draw_text(g,"This is our system font.",f,20,30);
    f=(font_t *)&radon_font;
    gpx_draw_text(g,"This is our radon font for sci-fi games.",f,30,60);
    f=(font_t *)&dagger_font;
    gpx_draw_text(g,"This is our dagger font for text adventures.",f,30,90);

    wait();

    /* exit graphics mode */
    gpx_exit(g);
}