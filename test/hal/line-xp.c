/*
 * line-xp.c
 *
 * Testing hal functions. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 31.05.2021   tstih
 *
 */
#include <stdint.h>
#include <conio.h>
#include <stdio.h>

/* initializes gdp, enter hires (1024x512) mode */
extern void ef9367_init();

/* clear screen and goto 0,0 */
extern void ef9367_cls();

/* fast line draw */
extern int16_t  ef9367_draw_line(
    uint16_t x0, 
    uint16_t y0, 
    uint16_t x1,
    uint16_t y1);

void wait() {
    while (!kbhit());
}

int diff;

void main() {

    ef9367_init();

    ef9367_cls();
    
    for(int y=0;y<512;y++) ef9367_draw_line(0,y,2*y,y);

    wait();

    ef9367_cls();
}