/*
 * glyph.h
 *
 * glyph related consts and structures.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.04.2021   tstih
 *
 */
#ifndef __GLYPH_H__
#define __GLYPH_H__

#include <gpx.h>

/* glyph generation */

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




#endif /* __GLYPH_H__ */