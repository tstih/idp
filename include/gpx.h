/*
 * gpx.h
 *
 * include this and -l gpx if you want to use raw graphics functions
 * without the zwin windows. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 23.03.2021   tstih
 *
 */
#ifndef __GPX_H__
#define __GPX_H__

#include <yos.h>


/*
 * extra gpx types
 */
typedef int16_t coord;


/* 
 * basic graphics structures (rect_t, graphics_t, etc.)
 */

/* the rectangle */
typedef struct rect_s {
	coord x0;
	coord y0;
	coord x1;
	coord y1;
} rect_t;

/* the display */
typedef struct graphics_s {
    rect_t area;            /* graphics area in abs. coordinates */
    rect_t clip;            /* grapics clipping area in abs. coordinates */
} graphics_t;


/*
 * glyphs 
 */

/* glyph class, bits 2-0 of the generation flag */
#define GCLS_BITMMAP        0
#define GCLS_MOUSE_CURSOR   1
#define GCLS_FONT           2
#define GCLS_ANIMATION      3

/* glyph drawing mode bits 4-3 of the generation flag */
#define GDWM_TINY           0
#define GDWM_RASTER         1
#define GDWM_LINES          2

/* tiny glyph directions bits 2-0 */
#define TDR_RIGHT           0
#define TDR_RIGHT_UP        1
#define TDR_UP              2
#define TDR_LEFT_UP         3
#define TDR_DOWN            4
#define TDR_RIGHT_DOWN      5
#define TDR_LEFT            6
#define TDR_LEFT_DOWN       7

/* tiny move values bits 4-3 */
#define TPV_MASK            0x18        /* bits 4 and 3 */
#define TPV_SET             0x00        /* set pixel */
#define TPV_RESET           0x08        /* reset pixel */
#define TPV_MOVE            0x10        /* just move */

/* drawing mode */
#define DWM_SET             0x00
#define DWM_RESET           0x00



typedef struct gpy_generation_s {
    unsigned int gcls:3;                /* glyph class */
    unsigned int gdwm:2;                /* drawing mode */
    unsigned int reserved:3;            /* up to 1 byte */
} gpy_generation_t;

/* each glyph file starts with glyph envelope */
typedef struct gpy_envelope_s {
    gpy_generation_t generation;
    uint16_t width;
    uint16_t height;
    uint8_t reserved;                   /* number of glyphs following */
} gpy_envelope_t;

/* font_s struct is a gpy_envelope_s */
typedef struct font_s {
    gpy_generation_t generation;
    uint16_t width;
    uint16_t height;
    union {
        uint8_t stride;                     /* bytes per glyph row for raster fonts */   
        uint8_t reserved;
    };
    uint8_t first_ascii;
    uint8_t last_ascii;
} font_t;


/*
 * drawing functions
 */

/* initialize the graphics system */
extern graphics_t* gpx_init();

/* exit graphics mode */
extern void gpx_exit(graphics_t* g);

/* clear screen */
extern void gpx_cls(graphics_t *g);

/* draw text on screen */
extern void gpx_draw_text(graphics_t* g, char *s, font_t *font, coord x, coord y); 

#endif /* __GPX_H__ */