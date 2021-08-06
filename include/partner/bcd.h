/*
 * bcd.h
 *
 * condensed binary coded decimal functions
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 05.08.2021   tstih
 *
 */
#ifndef __BDC_H__
#define __BCD_H__

#include <stdint.h>

/* convert byte to bcd */
extern uint8_t bin2bcd(uint8_t bin);

/* convert bcd to byte */
extern uint8_t bcd2bin(uint8_t bcd);

#endif /* __BCD_H__ */