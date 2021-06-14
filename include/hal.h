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

#include <stdint.h>

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

/* Draw a pixel. This is very slow. Use only if no other option. */
extern void hal_hires_set_pixel(int x, int y, uint8_t mode);

/* Put raster on screen */
extern void hal_hires_put_raster(
    uint8_t *raster,
    uint8_t stride,
    int16_t x, 
    int16_t y, 
    uint8_t width,
    uint8_t height,
    uint8_t mode);

/* Draw fassst line */
extern void hal_hires_line(
    int x0, int y0, int x1, int y1, uint8_t mode, uint8_t mask);

#endif /* __HAL_H__ */