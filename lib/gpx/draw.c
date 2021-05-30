/*
 * draw.h
 *
 * graphics drawing primitives
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 30.05.2021   tstih
 *
 */
#include <gpx.h>
#include <font.h>
#include <hal.h>

void gpx_draw_text(graphics_t* g, char *s, font_t *font, coord x, coord y)
{
    uint8_t* gstart=((uint8_t *)font) + sizeof(font_t);
    while (*s) {
        uint16_t index = ((*s)-font->first_ascii) * font->height * font->stride;
        hal_hires_put_raster(gstart+index, x, y, font->width, font->height);
        x+=(font->width);
        s++;
    }
}