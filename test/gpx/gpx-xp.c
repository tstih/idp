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

extern uint8_t test();
extern void ef9367_init();
extern int ef9367_put_pixel(uint16_t x, uint16_t y, uint8_t mode);

void wait() {
    while (!kbhit());
}

void main() {

    ef9367_init();

    /* display warning */
    printf("WARNING: The screen will enter  graphics mode\n\r");
    printf("         There will be no text. Press any key\n\r");
    printf("         to continue to the next experiment.\n\r");
    printf("         You'll have to switch the computer\n\r");
    printf("         off at the end. Sorry!\n\r\n\r");
    printf("Now press a key to start the first experiment.\n\r");
    wait();


    graphics_t *g=gpx_init();

    for (int i=0;i<200;i+=3)
        gpx_draw_pixel(g, 500+i,200,DWM_SET|DWM_XOR);
    for (int j=0;j<3;j++)
        gpx_draw_circle(g,j*200,j*100,j*50,DWM_SET);
    wait();

    gpx_exit(g);
}