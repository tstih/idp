/*
 * ef9367ex_.c
 *
 * Extra routines in C
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 01.06.2021   tstih
 *
 */
#include <gpx.h>
#include <ef9367.h>

#define EF9367_CMD_PEN_DOWN 0x02
#define EF9367_CMD_SEL_PEN  0x00 
#define EF9367_CMD_SEL_ERS  0x01

__sfr __at 0x25 EF9367_DX;
__sfr __at 0x27 EF9367_DY;
__sfr __at 0x20 EF9367_CMD;
__sfr __at 0x28 EF9367_XPOS_HI;
__sfr __at 0x29 EF9367_XPOS_LO;
__sfr __at 0x2a EF9367_YPOS_HI;
__sfr __at 0x2b EF9367_YPOS_LO;

void dline(uint8_t cmd, int dx, int dy) {
    if (dx>255 || dy>255) {
        int dx2=dx/2, dy2=dy/2;
        dline(cmd, dx2, dy2);
        dline(cmd, dx-dx2, dy-dy2);
    } else {
        /* draw it */
        EF9367_DX=dx;
        EF9367_DY=dy;
        __asm
        call ef9367_wait_sts_ready
        __endasm;
        EF9367_CMD=cmd;
    }
}

void initpen(bool set) {
    /* select pen */
    __asm
        call ef9367_wait_sts_ready
    __endasm;
    if (!set) /* eraser? */
        EP9367_CMD=EF9367_CMD_SEL_ERS;
    else /* pen and xor modes */
        EF9367_CMD=EF9367_CMD_SEL_PEN;

    /* pen down */
    __asm
        call ef9367_wait_sts_ready
    __endasm;
    EF9367_CMD=EF9367_CMD_PEN_DOWN;
}

void xy(int x, int y) {
    /* cursor to x,y */
    __asm
    call ef9367_wait_sts_ready
    __endasm;
    uint8_t* p = &x;
    EF9367_XPOS_LO=*p++;
    EF9367_XPOS_HI=*p;
    p=&y;
    EF9367_YPOS_LO=*p++;
    EF9367_YPOS_HI=*p;
}

void  ef9367_draw_line(
    uint16_t x0, 
    uint16_t y0, 
    uint16_t x1,
    uint16_t y1) {

    /* goto x,y */
    y0=512-y0; y1=512-y1; /* reverse axis */
    
    xy(x0,y0);

    initpen(true);

    /* drawing command */
    uint8_t cmd=0x11; 
    int dx,dy;
    
    /* set sign and max length for dx and dy */
    if (y1>y0) { dy=y1-y0; cmd|=4; }
    else dy=y0-y1;
    if (x1>x0)  dx=x1-x0;  
    else { dx=x0-x1; cmd|=2; }
    
    /* now draw */
    dline(cmd,dx,dy);
}

void ef9367_put_raster(
    uint8_t *raster,
    uint8_t stride,
    uint16_t x, 
    uint16_t y, 
    uint8_t width,
    uint8_t height) {
    /* TODO: do this... */
}
