/*
 * scn2674.h
 * 
 * header  for text mode (only) primitives for the signetics scn2674 card.
 *
 * tomaz stih tue mar 23 2021
 * 
 */
#ifndef _SCN2674_H
#define _SCN2674_H

/* 
 * command register and bits 
 */

/* 0x34 - R/W:character register */
#define SCN2647_CHR             0x34

/* 0x35 - R/W:attribute register */
#define SCN2647_AT              0x35
#define SCN2647_AT_BLINK        0x01    /* blink */
#define SCN2647_AT_UNDERLINE    0x02    /* underline */
#define SCN2647_AT_SPC_CHR      0x04    /* special character */
#define SCN2647_AT_PROTECT      0x08    /* protect */
#define SCN2647_AT_HIGHLIGHT    0x10    /* R:highlight, W:red foreground */
#define SCN2647_AT_REVERSE      0x20    /* R:reverse, W:green background */
#define SCN2647_AT_GP2          0x40    /* R:GP 2, W:blue background */
#define SCN2647_AT_GP1          0x80    /* R:GP 1, W:red background */

/* 0x36 - W:gr.scroll (byte value=scan lines), R:common input */
#define SCN2647_GR_SCROLL       0x36

#define SCN2647_GR_CMNI         0x36    /* same register, different name */
#define SCN2647_GR_CMNI_AVDCA   0x10    /* avdc access flag */
#define SCN2647_GR_CMNI_PIX     0x80    /* graph. pix input */

/* 0x38 - W:init (all 15 init reg. sequentially accessed), R:interrupt
          (bits 7-6 default 0) */
#define SCN2647_INIT            0x38
#define SCN2647_IR              0x38    /* same register, different name */
#define SCN2647_IR_SS2          0x01    /* split screen 2 interrupt */
#define SCN2647_IR_RDY          0x02    /* ready interrupt */
#define SCN2647_IR_SS1          0x04    /* split screen 1 interrupt */
#define SCN2647_IR_LZ           0x08    /* line zero interrupt */
#define SCN2647_IR_VB           0x10    /* vblank int */

/* 0x39 - W:command (byte is command), R:status (bits 7-5 default 0) */
#define SCN2647_CMD             0x39    
#define SCN2674_CMD_RESET       0x00
#define SCN2674_CMD_CURS_OFF    0x30
#define SCN2674_CMD_CURS_ON     0x31

#define SCN2647_STS             0x39    /* same register, different name */    
#define SCN2647_STS_SS2         0x01    /* split screen 2 interrupt */
#define SCN2647_STS_RDYI        0x02    /* ready interrupt */
#define SCN2647_STS_SS1         0x04    /* split screen 1 interrupt */
#define SCN2647_STS_LZ          0x08    /* line zero interrupt */
#define SCN2647_STS_VB          0x10    /* vblank int */
#define SCN2647_STS_RDY         0x20    /* ready flag */

/* 0x3a - R/W: screen start 1 lower register */
#define SCN2647_SS1_LO          0x3a

/* 0x3b - R/W: screen start 1 upper register */
#define SCN2647_SS1_HI          0x3b

/* 0x3c - R/W: cursor address lower register */
#define SCN2647_CUR_LO          0x3c

/* 0x3d - R/W: cursor address upper register */
#define SCN2647_CUR_HI          0x3d

/* 0x3e - R/W: screen start 2 lower register */
#define SCN2647_SS2_LO          0x3e

/* 0x3f - R/W: screen start 2 upper register */
#define SCN2647_SS2_HI          0x3f

/*
 * functions
 */

/* master reset */
extern void  scn2674_reset() __naked;

/* display off and float DADD bus */
extern void scn2674_display_off() __naked;

/* cursor off */
extern void scn2674_cursor_off() __naked;

#endif /* _SCN2674_H */