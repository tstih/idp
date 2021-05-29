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

void draw_circle(graphics_t *d, coord_t x0, coord_t y0, coord_t radius, byte_t mode, byte_t pattern)
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
