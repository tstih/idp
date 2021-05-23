/*
 * bcd.h
 *
 * bcd conversion functions (8 bit only!) (internal)
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 22.05.2021   tstih
 *
 */
#ifndef __BCD_H__
#define __BCD_H__

#include <stdint.h>

extern uint8_t _bin2bcd(uint8_t bin);
extern uint8_t _bcd2bin(uint8_t bcd);

#endif /* __BCD_H__ */