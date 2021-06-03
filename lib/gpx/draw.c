/*
 * draw.h
 *
 * Graphics drawing functions. These functions call HAL
 * (hardware abstract layer) primitives to get the job done.
 * In addition they do clipping, coordinates validation, etc.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 30.05.2021   tstih
 *
 */
#include <gpx.h>
#include <hal.h>

void gpx_draw_pixel(graphics_t *g, coord x, coord y, uint8_t mode) {
    hal_hires_set_pixel(x,y,mode);
}

void gpx_draw_text(graphics_t* g, char *s, font_t *font, coord x, coord y, uint8_t mode)
{
    uint8_t* gstart=((uint8_t *)font) + sizeof(font_t);
    while (*s) {
        uint16_t index = ((*s)-font->first_ascii) * font->height * font->stride;
        hal_hires_put_raster(
            gstart+index, 
            font->stride, 
            x, 
            y, 
            font->width, 
            font->height, 
            mode);
        x+=(font->width);
        s++;
    }
}

void gpx_draw_line(
    graphics_t *h, 
    coord x0, 
    coord y0, 
    coord x1, 
    coord y1, 
    uint8_t mode,
    uint8_t mask) {
    hal_hires_line(x0,y0,x1,y1,mode,mask);
}

void gpx_draw_circle(graphics_t *d, coord x0, coord y0, coord radius, uint8_t mode)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    hal_hires_set_pixel(x0, y0 + radius, mode);
    hal_hires_set_pixel(x0, y0 - radius, mode);
    hal_hires_set_pixel(x0 + radius, y0, mode);
    hal_hires_set_pixel(x0 - radius, y0, mode);
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
        hal_hires_set_pixel(x0 + x, y0 + y, mode);
        hal_hires_set_pixel(x0 - x, y0 + y, mode);
        hal_hires_set_pixel(x0 + x, y0 - y, mode);
        hal_hires_set_pixel(x0 - x, y0 - y, mode);
        hal_hires_set_pixel(x0 + y, y0 + x, mode);
        hal_hires_set_pixel(x0 - y, y0 + x, mode);
        hal_hires_set_pixel(x0 + y, y0 - x, mode);
        hal_hires_set_pixel(x0 - y, y0 - x, mode);
    }
}