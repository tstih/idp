/*
 * draw.c
 *
 * drawing routines.
 * 
 * TODO:
 *  optimize functions for id partner
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.04.2021   tstih
 *
 */
#include "draw.h"



void ef9367_put_raster(
    uint8_t *raster,
    uint8_t stride,
    uint16_t x, 
    uint16_t y, 
    uint8_t width,
    uint8_t height) {

    initpen();

    y=512-y; /* reverse axis */

    while (height>0) {
        
        xy(x,y);
        
        /* next row, remember:reversed axis */
        y--;

        /* draw stride of bytes */
        uint8_t bits=8;
        for(uint8_t b=0;b<stride;b++) {
            if (b==stride-1) /* last byte */
                bits=width & 7; /* remainder bits */
            /* paint bits */
            uint8_t pattern=*raster;
            for(uint8_t bit=0;bit<bits;bit++) {
                if ((pattern&0x80)!=0) {
                    __asm
                        call ef9367_wait_sts_ready
                    __endasm;
                    EF9367_CMD=EF9367_CMD_SEL_PEN;
                } else {
                    __asm
                        call ef9367_wait_sts_ready
                    __endasm;
                    EF9367_CMD=EF9367_CMD_SEL_ERS;
                }
                pattern<<=1;

                __asm
                call ef9367_wait_sts_ready
                __endasm;            
                EF9367_CMD=0b10100000;
            }
            raster++;
        }

        /* loop */
        height--;
    }
}



byte_t draw_pixel(graphics_t *d, coord_t x0, coord_t y0, byte_t mode)
{
    gdp_setpixel(x0,y0);
    return 0;
}

void draw_line(graphics_t *d, coord_t x0, coord_t y0, coord_t x1, coord_t y1, byte_t mode, byte_t pattern)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        draw_pixel(d, x0, y0, mode);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void gpx_draw_circle(graphics_t *d, coord_t x0, coord_t y0, coord_t radius, byte_t mode)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    draw_pixel(d, x0, y0 + radius, mode);
    draw_pixel(d, x0, y0 - radius, mode);
    draw_pixel(d, x0 + radius, y0, mode);
    draw_pixel(d, x0 - radius, y0, mode);
    while (x < y)
    {
        /*  ddF_x == 2 * x + 1;
        ddF_y == -2 * y;
        f == x*x + y*y - radius*radius + 2*x - y + 1;
    */
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        draw_pixel(d, x0 + x, y0 + y, mode);
        draw_pixel(d, x0 - x, y0 + y, mode);
        draw_pixel(d, x0 + x, y0 - y, mode);
        draw_pixel(d, x0 - x, y0 - y, mode);
        draw_pixel(d, x0 + y, y0 + x, mode);
        draw_pixel(d, x0 - y, y0 + x, mode);
        draw_pixel(d, x0 + y, y0 - x, mode);
        draw_pixel(d, x0 - y, y0 - x, mode);
    }
}
