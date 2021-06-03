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

#include <gpx.h>

extern int test();
extern void ef9367_init();
extern void ef9367_cls();

/* fast line draw */
extern int16_t  ef9367_draw_line(
    uint16_t x0, 
    uint16_t y0, 
    uint16_t x1,
    uint16_t y1,
    uint8_t mode,
    uint8_t mask);  

void wait() {
    while (!kbhit());
}

int diff;

void main() {

    ef9367_init();

    printf("%d\n\r",ef9367_draw_line(0,0,200,100,DWM_SET,DMSK_SOLID));
}