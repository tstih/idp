/*
 * gpx3-xp.c
 *
 * Strides test.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 12.08.2021   tstih
 *
 */
#include <stdint.h>
#include <stdio.h>

#include <gpx.h>

/* stride */
extern int _stridexy(
    coord x, 
    coord y, 
    void *data, 
    uint8_t start, 
    uint8_t end);

uint8_t stride[]={
    0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa};

void main() {

    /* enter graphics */
    gpx_t *g=gpx_init();

    int offs=192;
    int size=16;

    for(int y=offs;y<offs+size;y++)
        _stridexy(offs+(y%2),y,stride,0,size);

    /* leave graphics */
    gpx_exit(g);
}