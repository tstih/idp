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
    /* first reset the avdc (text mode). clear screen and hide cursor */
    avdc_init();
    avdc_cls();
    avdc_cursor_off();

    /* now initializa gdp (hires mode), clear screen and go to 0,0 */
    gdp_init();
    hal_hires_cls();
#endif /* __PARTNER__ */
}

void hal_hires_exit() {
#ifdef __PARTNER__
    hal_hires_cls();
    /* Show cursor (...back to CP/M). */
    avdc_cursor_on();
#endif /* __PARTNER__ */
}

void hal_hires_info(int *w, int *h) {
#ifdef __PARTNER__
    *w=GDP_HIRES_WIDTH;
    *h=GDP_HIRES_HEIGHT;
#endif /* __PARTNER__ */
}

void hal_hires_cls() {
#ifdef __PARTNER__
    gdp_cls();
    gdp_xy(0,0);
#endif /* __PARTNER__ */
}