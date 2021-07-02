#ifndef __AVDC_H__
#define __AVDC_H__

#include <stdint.h>

typedef union
{
    uint16_t u16;
    uint8_t u8[2];
} U16_U8;

#define DEFAULT_ATTR 0x00

#define SCN2674_STS_RDY 0x20     // ready status bit

__sfr __at 0x39 SCN2674_CMD;     // W: command 
__sfr __at 0x39 SCN2674_STS;     // R: status 
__sfr __at 0x38 SCN2674_INIT;    // W: write to current IR (init/interrupt register) 
__sfr __at 0x34 SCN2674_CHR;     // R/W: character register 
__sfr __at 0x35 SCN2674_ATTR;    // R/W: attribute register 
__sfr __at 0x3C SCN2674_CUR_LWR; // W: cursor address lower
__sfr __at 0x3D SCN2674_CUR_UPR; // W: cursor address upper

extern uint16_t row_addr[24];

void avdc_wait_access();
void avdc_wait_ready();

void avdc_cursor_off();
void avdc_cursor_on();

uint16_t avdc_get_pointer(uint8_t row, uint8_t col);
uint16_t avdc_get_pointer_cached(uint8_t row, uint8_t col);

void avdc_read_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr);

void avdc_write_at_pointer(uint16_t address, uint8_t chr, uint8_t attr);
void avdc_write_str_at_pointer(uint16_t address, uint8_t *str, uint8_t *attr);
void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr);

void avdc_set_cursor(uint8_t row, uint8_t col);

void avdc_write_at_cursor(uint8_t chr, uint8_t attr);
void avdc_write_str_at_cursor(uint8_t *str, uint8_t *attr);
void avdc_set_cursor_write_str(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr);

void avdc_done();

#endif