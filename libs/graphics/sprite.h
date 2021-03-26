/*
 * sprite.h
 * sprite definitions.
 * 
 * tomaz stih thu mar 25 2021
 */
#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdint.h>

/* 
 * SPR_TYP_IDP_TINY sprite type is max 16x16 and defined
 * by a sequen of moves. each move is 1 byte and has format
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
 * when pen is down the content of screen is changed (erased or drawn), 
 * when it is up it merely moves the position
 * 
 */
#define SPR_TYP_IDP_TINY    1  /* max 16x16 */

typedef struct spirte_s {

    uint8_t type;       /* sprite type, only SPR_TYP_IDP_TINY is supported  */
    union {
        uint8_t moves;  /* only for SPR_TYP_IDP_TINY  */
    };
    uint8_t width;     
    uint8_t height;
    uint8_t data[0];    /* sprite data */

} sprite_t;

#endif // _SPRITE_H