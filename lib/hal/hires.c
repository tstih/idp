/*
 * hires.c
 * 
 * abstracted hires functions.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 04.04.2021   tstih
 * 
 */
#include <yos.h>
#include <hires.h>

void hal_hires_init() {
#ifdef __PARTNER__
    /* first reset the scn2674 (text mode). clear screen and hide cursor */
    scn2674_init();
    scn2674_cls();
    scn2674_cursor_off();

    /* now initializa ef9367 (hires mode), clear screen and go to 0,0 */
    ef9367_init();
    hal_hires_cls();
#endif /* __PARTNER__ */
}

void hal_hires_exit() {
#ifdef __PARTNER__
    hal_hires_cls();
    /* Show cursor (...back to CP/M). */
    scn2674_cursor_on();
#endif /* __PARTNER__ */
}

void hal_hires_info(int *w, int *h) {
#ifdef __PARTNER__
    *w=EF9367_HIRES_WIDTH;
    *h=EF9367_HIRES_HEIGHT;
#endif /* __PARTNER__ */
}

void hal_hires_cls() {
#ifdef __PARTNER__
    ef9367_cls();
    ef9367_xy(0,0);
#endif /* __PARTNER__ */
}

void hal_hires_put_raster(
    uint8_t *raster,
    int16_t x, 
    int16_t y, 
    uint8_t width,
    uint8_t height)
{
#ifdef __PARTNER__
    /* draw raw bitmap at x,y */
    ef9367_put_raster(raster, x, y, width, height);
#endif
}