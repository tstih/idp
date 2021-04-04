/*
 * z80_sio.h
 * 
 * header for the z80_sio (serial) functions.
 *
 * tomaz stih sat arp 03 2021
 * 
 */
#ifndef _Z80_SIO
#define _Z80_SIO

#include "sdcc.h"

/* 
 * SIO registers and their bits
 */

/* on idp this is the keyboard port */
#define Z80SIO_PORT1_DATA   0xd8
#define Z80SIO_PORT1_STATUS 0xd9

/* on idp this is the 1st serial port (mouse, data comm.) */
#define Z80SIO_PORT2_DATA   0xda
#define Z80SIO_PORT2_STATUS 0xdb

#define Z80SIO_PORT3_DATA   0xe0
#define Z80SIO_PORT3_STATUS 0xe1

#define Z80SIO_PORT4_DATA   0xe2
#define Z80SIO_PORT4_STATUS 0xe3

#endif // _Z80_SIO