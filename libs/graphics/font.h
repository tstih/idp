  
/*
 * font.h
 * poor mans' fonts.
 * 
 * tomaz stih thu mar 25 2021
 */
#ifndef _FONT_H
#define _FONT_H

#include <stdint.h>

/* font type flags, only bits 7-5 are used */
#define FONT_FIXED      0x80  /* 1=fixed, 0=proportional */
#define FONT_RASTER     0x40  /* 1=raster, 0=vector */                
#define FONT_TYPO       0x20  /* 1=typograpical, 0=plain */
#define FONT_TINY       0x10  /* 1=max 16x16, 2=max 256x256 */

/* 
 * font header is followed by an array of font pointers
 */ 
typedef struct font_header_s {
    /* all fonts share this part */
    uint8_t type;           /* font type */
    uint8_t first_ascii;
    uint8_t last_ascii;
    uint8_t height;
    /* raster fonts only, otherwise 0 */
    uint8_t width_in_bytes;
    /* fixed fonts only, otherwise 0 */
    uint8_t fixed_width;
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
    uint16_t glyph_offs[0];
} font_header_t;

#endif // _FONT_H