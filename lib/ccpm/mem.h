/*
 * mem.h
 *
 * Memory management (malloc, free, itn.)
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 25.05.2012   tstih
 *
 */
#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>
#include <stdlib.h>
#include <list.h>

#ifndef NONE
#define NONE 0
#endif

#define BLK_SIZE        (sizeof(struct block_s) - sizeof(uint8_t[1]))
#define MIN_CHUNK_SIZE  4

/* block status, use as bit operations */
#define NEW             0x00
#define ALLOCATED       0x01

typedef struct block_s {
    list_header_t   hdr;
    uint8_t         stat;
    uint16_t        size;
    uint8_t         data[1];
} block_t;

/* Defined in crt0cpm.s */
extern void heap;

/* Mem top. By convention this is start of BDOS, retreived from
the 0x0005 address (BDOS call) */
extern uint16_t* mem_top;

#endif /* __MEM_H__ */