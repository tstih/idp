#ifndef __GDP_H__
#define __GDP_H__

#include <stdint.h>

typedef enum {
	DRAWING_MODE_CLEAR = 0,
	DRAWING_MODE_SET = 1,
	DRAWING_MODE_XOR = 2,
	DRAWING_MODE_UNDEFINED = 0xFF
} drawing_mode;

// status
#define GDP_STATUS_READY    0x04
// cmd
#define GDP_CMD_CLS         0x04 
// pio_gr_cmn
#define PIO_GR_CMN_1024x512 0x18

__sfr __at 0x20 GDP_CMD;     // W: command 
__sfr __at 0x20 GDP_STATUS;  // R: status (ready)
__sfr __at 0x28 GDP_XPOS_HI; // RW: x pos hi
__sfr __at 0x29 GDP_XPOS_LO; // RW: x pos lo 
__sfr __at 0x2a GDP_YPOS_HI; // RW: y pos hi
__sfr __at 0x2b GDP_YPOS_LO; // RW: y pos low 
__sfr __at 0x21 GDP_CR1; 	 // RW: control register, bit 7 is reserved (always 0) 
__sfr __at 0x22 GDP_CR2;     // RW: control register 2, bit 7-4 are not used
__sfr __at 0x23 GDP_CH_SIZE; // RW: character size (low nibble is scaling on x, high nibble is scaling on y)
__sfr __at 0x30 PIO_GR_CMN;  // RW: PIO gr. common register
// __sfr __at 0x36 AVDC_ACCESS;  // R: status (access)
// __sfr __at 0x38 AVDC_INIT;    // W: write to current IR (init/interrupt register) 
// __sfr __at 0x34 AVDC_CHR;     // R/W: character register 
// __sfr __at 0x35 AVDC_ATTR;    // R/W: attribute register 

// __sfr __at 0x3C AVDC_CUR_LWR; // W: cursor address lower
// __sfr __at 0x3D AVDC_CUR_UPR; // W: cursor address upper

// extern uint16_t row_addr[24];

// void avdc_init();
// void avdc_done();

// void avdc_wait_access(); // WARNME: disables interrupts (you need to call EI to re-enable them)
// void avdc_wait_ready();

// void avdc_cursor_off();
// void avdc_cursor_on();

// uint16_t avdc_get_pointer(uint8_t row, uint8_t col);
// uint16_t avdc_get_pointer_cached(uint8_t row, uint8_t col);

// void avdc_read_at_pointer(uint16_t addr, uint8_t *chr, uint8_t *attr);

// void avdc_write_at_pointer(uint16_t addr, uint8_t chr, uint8_t attr);
// void avdc_write_str_at_pointer(uint16_t addr, uint8_t *str, uint8_t *attr);
// void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr);

// void avdc_set_cursor(uint8_t row, uint8_t col);

// void avdc_write_at_cursor(uint8_t chr, uint8_t attr);
// void avdc_write_str_at_cursor(uint8_t *str, uint8_t *attr);
// void avdc_set_cursor_write_str(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr);

void gdp_wait_ready();

void gdp_init();

void gdp_cls();

void gdp_set_drawing_mode(drawing_mode mode);
void gdp_set_xy(uint16_t x, uint16_t y);
void gdp_draw_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2);

#endif