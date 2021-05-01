/*
 * sprite.h
 * sprite definitions.
 * 
 * tomaz stih thu mar 25 2021
 */
#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdint.h>

#include "glyph.h"


typedef struct spirte_s {

    glyph_header_t ghdr;        /* standard glyph header */    
    glyph_data_t gdata;         /* glyph data */

} sprite_t;

#endif // _SPRITE_H