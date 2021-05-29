#ifndef __AVDC_H__
#define __AVDC_H__

#include <stdint.h>

typedef union
{
    uint16_t u16;
    uint8_t u8[2];
} U16_U8;

#define DEFAULT_ATTR 0x00

#define SCN2674_STS_RDY 0x20  // ready status bit

__sfr __at 0x39 SCN2674_CMD;  // W: command 
__sfr __at 0x39 SCN2674_STS;  // R: status 
__sfr __at 0x38 SCN2674_INIT; // W: write to current IR (init/interrupt register) 
__sfr __at 0x34 SCN2674_CHR;  // R/W: character register 
__sfr __at 0x35 SCN2674_ATTR; // R/W: attribute register 

void avdc_wait();
uint16_t avdc_get_pointer(uint8_t row, uint8_t col);

void avdc_read_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr);

void avdc_write_at_pointer(uint16_t address, uint8_t chr, uint8_t attr);
void avdc_write_str_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr);
void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *chr, uint8_t *attr);

#endif