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
    printf("Press any key.\n\r");

    for (int i=0;i<200;i+=3)
        gpx_draw_pixel(NULL, 500+i,200,DWM_SET|DWM_XOR);
    for (int j=0;j<3;j++)
        gpx_draw_circle(NULL,j*200,j*100,j*50,DWM_SET);
        
    wait();
}