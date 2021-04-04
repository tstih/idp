/*
 * ef9367.h
 * 
 * a library of graphical primitives for the thompson ef9367 card. 
 * 
 * partner mixes text and graphics in a non-conventional way. images from 
 * both sources are mixed on display, but remain independent of each other 
 * i.e. clear screen in text mode will remove text from the display, 
 * but leave all the graphics. and vice versa. 
 *
 * tomaz stih tue mar 23 2021
 * 
 */
#ifndef _EF9367_H
#define _EF9367_H

#include <stdint.h>

/* ----- graphics card registers ------------------------------------------- */

/* 0x20 - W: command, R:the status register */
#define EF9367_CMD              0x20    /* command  */
#define EF9367_CMD_SEL_PEN      0x00    /* select pen (set bit 1 of CR1) */
#define EF9367_CMD_SEL_ERS      0x01    /* select eraser (clr bit 1 of CR1) */
#define EF9367_CMD_SEL_DOWN     0x02    /* pen/eraser down */
#define EF9367_CMD_SEL_UP       0x03    /* pen/eraser up */
#define EF9367_CMD_CLS          0x04    /* clear screen */
#define EF9367_CMD_00           0x05    /* x=y=0 */
#define EF9367_CMD_CLS00        0x06    /* clear screen, x=y=0 */
#define EF9367_CMD_CLEAR        0x07    /* cls, CSIZE to min., all regs to 0 */
#define EF9367_CMD_LP_INIT_W    0x08    /* light pen init, WHITE to low */
#define EF9367_CMD_LP_INIT      0x09    /* light pen init */
#define EF9367_CMD_5x8          0x0a    /* set 5x8 block drawing (CSIZE) */
#define EF9367_CMD_4x4          0x0b    /* set 4x4 block drawing (CSIZE) */
#define EF9367_CMD_SCAN         0x0c    /* screen scan */
#define EF9367_CMD_X0           0x0d    /* x=0 */
#define EF9367_CMD_Y0           0x0e    /* y=0 */
#define EF9367_CMD_DMEM         0x0f    /* direct memory access 4 next cyc. */

#define EF9367_STS              0x20    /* status reg. (no interrupt reset) */
#define EF9367_STS_LPEN         0x01    /* light pen end */
#define EF9367_STS_VBLANK       0x02    /* vertical blanking */
#define EF9367_STS_READY        0x04    /* ready */
#define EF9367_STS_POD          0x08    /* pen out of display */
#define EF9367_STS_LPI          0x10    /* light pen irq */
#define EF9367_STS_VBI          0x20    /* vb irq */
#define EF9367_STS_RDYI         0x40    /* ready irq */
#define EF9367_STS_IRQ          0x80    /* IRQ */

/* 0x21 - RW: control register, bit 7 is reserved (always read as 0) */
#define EF9367_CR1              0x21
#define EF9367_CR1_PENDOWN      0x01    /* 1 pen down, 0 pen up */
#define EF9367_CR1_PEN          0x02    /* 1 pen, 0 eraser */
#define EF9367_CR1_WO           0x04    /* 1 write only */
#define EF9367_CR1_CYCLIC       0x08    /* cyclic screen if pen out... */
#define EF9367_CR1_LPEN_ENBLE   0x10    /* enable light pen irq */
#define EF9367_CR1_VB_ENBLE     0x20    /* enable vblank */
#define EF9367_CR1_RDY_ENBLE  0  0x40    /* enable ready irq */   

/* 0x22 - RW: control register 2, bit 7-4 are not used */
#define EF9367_CR2              0x22
/* bits 1 and 2 are vector type */
#define EF9367_CR2_SOLID        0b00    /* solid vector type */
#define EF9367_CR2_DOTTED       0b01    /* dotted vector type */
#define EF9367_CR2_DASHED       0b10    /* dashed */
#define EF9367_CR2_DOT_DASH     0b11    /* dot and dash pattern*/  

/* 0x23 - RW: character size */
#define EF9367_CH_SIZE          0x23
/* low nibble is scaling on x, high nibble is scaling on y) */

/* 0x25 - RW: delta x*/
#define EF9367_DX              0x25

/* 0x27 - RW: delta y */
#define EF9367_DY              0x27

/* 0x28 - RW: x pos hi */
#define EF9367_XPOS_HI         0x28
/* 0x29 - RW: x pos low */
#define EF9367_XPOS_LO         0x29
/* 0x2a - RW: y pos hi */
#define EF9367_YPOS_HI         0x2a
/* 0x2b - RW: y pos low */
#define EF9367_YPOS_LO         0x2b

/* 0x2c - R: xlp */
#define EF9367_XLP              0x2c    /* x light pen */
/* 0x2d - R: ylp */
#define EF9367_YLP              0x2d    /* y light pen */

/* 0x2f - R:the status register and its bitmask */
#define EF9367_STS_NI           0x2f    /* status reg. (no interrupt reset) */
#define EF9367_STS_NI_LPEN      0x01    /* low when executing light pen seq. */
#define EF9367_STS_NI_VBLANK    0x02    /* high during vertical blanking. */
#define EF9367_STS_NI_READY     0x04    /* low when executing command */
#define EF9367_STS_NI_OUTSCR    0x08    /* high when x or y is out of screen */     

/* 0x30 - R/W:PIO gr. common register */
#define EF9367_GR_CMN           0x30
#define EF9367_GR_CMN_DISP_PG   0x01    /* display page of memory */
#define EF9367_GR_CMN_WR_PG     0x02    /* write page of memory */
#define EF9367_GR_CMN_WR_MODE   0x04    /* write mode (xor/nor) */
/* bits 3-4 are resolution */
#define EF9367_GR_CMN_1024x512  0x18    /* hires */        
#define EF9367_GR_CMN_GDPINT    0x20    /* RO:gdp interrupt */ 
#define EF9367_GR_CMN_AVDINT    0x40    /* RO:avd interrupt */
#define EF9367_GR_CMN_SCRL_MODE 0x80    /* scroll mode */

/* 0x31 - W:PIO port A control */
#define EF9367_A_CTRL           0x31

/* 0x32 - RW:PIO common text attributes */
#define EF9367_GR_AT            0x32
#define EF9367_GR_AT_DOTS       0x01    /* dot stretch */
#define EF9367_GR_AT_CURS_MODE  0x02    /* cursor mode */
#define EF9367_GR_AT_COLOR      0x04    /* mono / color */
#define EF9367_GR_AT_BLUE       0x08    /* force background (blue fore) */
#define EF9367_GR_AT_REVERSE    0x10    /* reverse screen (green fore) */
#define EF9367_GR_AT_DOTS_C0    0x20    /* dots / char 0 */
#define EF9367_GR_AT_DOTS_C1    0x40    /* dots / char 1 */
#define EF9367_GR_AT_CLOCK      0x80    /* 24/16 Mhz text video clock */

/* 0x33 - W:PIO port B control */
#define EF9367_B_CTRL           0x33


/* ----- graphic functions ------------------------------------------------- */
#define MAXX                    1024
#define MAXY                    512


/* ----- graphic functions ------------------------------------------------- */

/* wait until current command is executed, returns status register */
extern uint8_t  ef9367_wait_sts_ready() __naked;

/* enter hires (1024x512) mode */
extern void ef9367_set_1024x512() __naked;

/* clear screen and goto 0,0 */
extern void ef9367_cls() __naked;

/* goto x,y */
extern void ef9367_xy(uint16_t x, uint16_t y) __naked;

#endif /* _EF9367_H */