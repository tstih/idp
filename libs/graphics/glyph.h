  
/*
 * glyph.h
 * shared definitions for all glyphs.
 * 
 * NOTES
 *  
 *  Tine glyph type is max 16x16 and defined by a sequen of moves. 
 *  each move is 1 byte and has format
 *
 *  7 6 5 4 3 2 1 0
 *  R R R T P D D D
 *  RRR ... reserved 
 *  T ... 0 erase pixel, 1 paint pixel
 *  P ... pen down/up
 *  DDD ... direction 
 *      000 = right
 *      001 = up + right
 *      010 = up
 *      011 = up + left
 *      100 = down
 *      101 = down + right
 *      110 = keft
 *      111 = down + left
 *  
 *  when pen is down the content of screen is changed (erased or drawn), 
 *  when it is up it merely moves the position
 * 
 * tomaz stih thu mar 25 2021
 */
#ifndef _GLYPH_H
#define _GLYPH_H

#include <stdint.h>

/* glyph type flags, only bits 7-5 are used */
#define GLYPH_TINY      0x01    /* 1=max 16x16, 2=max 256x256 */
#define GLYPH_RASTER    0x02    /* 1=raster, 0=vector */ 

/* 
 * glyph header is shared by all glyphs (cursors, fonts and sprites)
 */ 
typedef struct glyph_header_s {
    uint8_t type;               /* glyph type flags */
    uint8_t width;              /* width for fixed glyphs, max width for "vector" glyphs */
    uint8_t height;
    union {
        uint8_t width_in_bytes; /* for raster glyphs */
        uint8_t reserved;       /* for all other glyphs */
    };
} glyph_header_t;


/* 
 * glyph header provides some basic informationa about the glyph.
 */
typedef struct glyph_data_s {
    /* Glyph origin. This is (for vector fonts) also the point where we start drawing it. */
    uint8_t originx; 
    uint8_t originy;
    uint8_t width; /* if proportional font, this is glyph width. */
    uint8_t data[0]; /* actual glyph data i.e. moves or raster. */
} glyph_data_t;

#endif /* _GLYPH_H */