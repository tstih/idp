/*
 * hal.h
 *
 * the hardware abstraction layer library for Iskra Delta Partner
 * and ZX Spectrum 48K.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 29.05.2021   tstih
 *
 */
#ifndef __HAL_H__
#define __HAL_H__

#include <yos.h>

/* Initialize graphics mode. This function resets the resolution
to max resolution, switches off text mode (Partner only), and
clears screen. */
extern void hal_hires_init();

/* Gracefully leaves graphics mode. On Partner, it restores 
text mode cursor. */
extern void hal_hires_exit();

/* Returns hires info (screen width, screen height). */
extern void hal_hires_info(int *w, int *h);

/* Clears hires screen. */
extern void hal_hires_cls();

/* Put raster on screen */
void hal_hires_put_raster(
    uint8_t *raster,
    int16_t x, 
    int16_t y, 
    uint8_t width,
    uint8_t height);

#endif /* __HAL_H__ */