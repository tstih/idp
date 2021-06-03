/*
 * ef9367.h
 * 
 * a library of primitives for the thompson ef9367 card. 
 * 
 * notes:
 *  partner mixes text and graphics in a non-conventional way. images from 
 *  both sources are mixed on display, but remain independent of each other 
 *  i.e. clear screen in text mode will remove text from the display, 
 *  but leave all the graphics. and vice versa. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 04.04.2021   tstih
 * 
 */
#ifndef _EF9367_H
#define _EF9367_H

#include <yos.h>

/* hires mode screen size */
#define EF9367_HIRES_WIDTH      1024
#define EF9367_HIRES_HEIGHT     512

/* drawing mode flags (set and reset can be combined with xor) */
#define DWM_RESET           0   /* bit 0 */
#define DWM_SET             1   /* bit 0 */
#define DWM_XOR             2   /* bit 1 */

/* official line masks */
#define DMSK_SOLID          0xff
#define DMSK_DOTTED         0xaa
#define DMSK_DASHED         0xee
#define DMSK_DOT_DASH       0xe4

/* initializes gdp, enter hires (1024x512) mode */
extern void ef9367_init();

/* clear screen and goto 0,0 */
extern void ef9367_cls();

/* goto x,y */
extern void ef9367_xy(uint16_t x, uint16_t y);

/* put pixel at x,y using drawing mode */
extern void ef9367_put_pixel(uint16_t x, uint16_t y, uint8_t mode);

/* draw raw bitmap at x,y */
extern void ef9367_put_raster(
    uint8_t *raster,
    uint8_t stride,
    uint16_t x, 
    uint16_t y, 
    uint8_t width,
    uint8_t height,
    uint8_t mode);

/* fast line draw */
extern void  ef9367_draw_line(
    uint16_t x0, 
    uint16_t y0, 
    uint16_t x1,
    uint16_t y1,
    uint8_t mode,
    uint8_t mask);

#endif /* _EF9367_H */