/*
 * port.h
 *
 * port i/o functions (internal!)
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 2.05.2021   tstih
 *
 */
#ifndef __PORT_H__
#define __PORT_H__

#include <stdint.h>

/* read byte from i/o port */
extern uint8_t _port_read(uint8_t addr);

/* write byte to i/o port */
extern void _port_write(uint8_t addr, uint8_t value);

#endif /* __PORT_H__ */