/*
 * z80_pio.h
 * 
 * header for the z80_pio hardware functions.
 *
 * tomaz stih thu  mar 25 2021
 * 
 */
#ifndef _Z80_PIO
#define _Z80_PIO

/* 
 * PIO registers and their bits
 */


/* PIO port 1 */
#define Z80PIO_PORT1_DATA   0xd0
#define Z80PIO_PORT1_STATUS 0xd1

/* PIO port 2 */
#define Z80PIO_PORT2_DATA   0xd2
#define Z80PIO_PORT2_STATUS 0xd3

#endif // _Z80_PIO