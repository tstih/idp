/*
 * graphics.h
 *
 * functions that manipulate the graphics_t structure.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.04.2021   tstih
 *
 */
#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "yos.h"
#include "gpx.h"

#include <stdint.h>
#include "hw/ef9367.h"
#include "hw/scn2674.h"

/* current global variable */
extern graphics_t screen;

/* initialize display, pass device specific display info */
extern graphics_t* graphics_init();



/* special line type (horizontal) */
extern void draw_hline(graphics_t* d, coord y, coord x0, coord x1, uint8_t mode, uint8_t pattern);

/* special line type (vertical) */
extern void draw_vline(graphics_t* d, coord x, coord y0, coord y1, uint8_t mode, uint8_t pattern);

/* draw individual pixel */
extern uint8_t draw_pixel(graphics_t* d, coord x0, coord y0, uint8_t mode);

/* draw line */
extern void draw_line(graphics_t *d, coord x0, coord y0, coord x1, coord y1, uint8_t mode, uint8_t pattern);

/* draw circle with radius */
extern void draw_circle(graphics_t *d, coord x0, coord y0, coord radius, uint8_t mode, uint8_t pattern);

/* draw ellipse with rectangle */
extern void draw_ellipse(graphics_t *d, coord x0, coord y0, coord x1, coord y1, uint8_t mode, uint8_t pattern);

/* draws a rectangle */
extern void draw_rectangle(graphics_t *d, coord x0, coord y0, coord x1, coord y1, uint8_t mode, uint8_t pattern);


#endif /* _GRAPHICS_H */