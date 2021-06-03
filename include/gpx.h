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
#define DWM_RESET           0
#define DWM_SET             1
#define DWM_XOR             2

/* official list masks (will work fast!) */
#define DMSK_SOLID          0xff
#define DMSK_DOTTED         0xaa
#define DMSK_DASHED         0xee
#define DMSK_DOT_DASH       0xe4

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
 * initialization and screen functions
 */

/* initialize the graphics system */
extern graphics_t* gpx_init();

/* exit graphics mode */
extern void gpx_exit(graphics_t* g);

/* clear screen */
extern void gpx_cls(graphics_t *g);


/*
 * drawing functions
 */

/* Draw single pixel 
   TODO: implement mode - SET, RESET, or XOR ... currently it is ignored
*/
extern void gpx_draw_pixel(graphics_t *g, coord x, coord y, uint8_t mode);

/* draw text on screen */
extern void gpx_draw_text(
    graphics_t* g, 
    char *s, 
    font_t *font, 
    coord x, 
    coord y,
    uint8_t mode); 

/* draw fast line */
extern void gpx_draw_line(
    graphics_t *h, 
    coord x0, 
    coord y0, 
    coord x1, 
    coord y1, 
    uint8_t mode,
    uint8_t mask);

/* draw slow circle */
extern void gpx_draw_circle(
    graphics_t *d, 
    coord x0, 
    coord y0, 
    coord radius, 
    uint8_t mode);

/*
 * rectangle management functions
 */

/* Does rectangle contains point? */
extern bool rect_contains(rect_t *r, coord x, coord y);

/* Do rectangles overlap? */
extern bool rect_overlap(rect_t *a, rect_t *b);

/* Inflate coordinates by dx and dy */
extern rect_t* rect_inflate(rect_t* rect, coord dx, coord dy);

/* Get intersect rectangle. */
extern rect_t* rect_intersect(rect_t *a, rect_t *b, rect_t *intersect);

/* Convert relative to absolute coordinates for parent and child rectangle. */
extern rect_t* rect_rel2abs(rect_t* abs, rect_t* rel, rect_t* out);

/* Subtract rectangles and return what's left. */
extern void rect_subtract(rect_t *outer, rect_t *inner, rect_t *result,	uint8_t *num);

/* Offset rectangle. */
extern void rect_delta_offset(rect_t *rect, 
    coord oldx, coord newx, coord oldy, coord newy, coord size_only);

#endif /* __GPX_H__ */