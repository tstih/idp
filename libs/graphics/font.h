  
/*
 * font.h
 * poor mans' fonts.
 * 
 * tomaz stih thu mar 25 2021
 */
#ifndef _FONT_H
#define _FONT_H

#include <stdint.h>

#include "glyph.h"

/* font type flags  */
#define FONT_TINY       GLYPH_TINY      /* 1=max 16x16, 2=max 256x256 */
#define FONT_RASTER     GLYPH_RASTER    /* 1=raster, 0=vector */0
#define FONT_FIXED      0x10            /* 1=fixed, 0=proportional */            
#define FONT_TYPO       0x20            /* 1=typograpical, 0=plain */

/* 
 * font header is followed by an array of font pointers
 */ 
typedef struct font_header_s {

    glyph_header_t ghdr;        /* all glyphs share same header */

    uint8_t first_ascii;
    uint8_t last_ascii;
   
    /* typographical font only, otherwise 0 
       see: https://en.wikipedia.org/wiki/Typeface_anatomy   
    */
    uint8_t cap;
    uint8_t ascent;
    uint8_t descent;
    uint8_t median;
    uint8_t baseline;
    uint8_t descent;
    
    /* offsets to glpyhs, to get the address of glyph
       just add glpyh_offs[my ascii-start ascii] to the header address */
    glyph_data_s* glyph_offs[0];
} font_header_t;

#endif // _FONT_H