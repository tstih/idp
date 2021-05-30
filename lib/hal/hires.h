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
#ifndef __HIRES_H__
#define __HIRES_H__

#ifdef __PARTNER__
#include "dev/ef9367.h"
#include "dev/scn2674.h"
#endif /* __PARTNER__ */

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

#endif /* __HIRES_H__ */