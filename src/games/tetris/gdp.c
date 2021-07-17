#include "gdp.h"

typedef union {
   uint16_t u16;
   uint8_t u8[2];
} addr_t;

drawing_mode mode_cache = DRAWING_MODE_UNDEFINED;

void gdp_wait_ready() {
    uint8_t status = 0;
    while ((status & GDP_STATUS_READY) == 0) {
        status = GDP_STATUS;
    }
}

void gdp_init() {
    GDP_CR1 = 0;
    GDP_CR2 = 0;
    GDP_CH_SIZE = 0;
    PIO_GR_CMN = PIO_GR_CMN_1024x512;
    gdp_set_drawing_mode(DRAWING_MODE_SET);
    gdp_cls();
}

void gdp_cls() {
    gdp_wait_ready();
    GDP_CMD = GDP_CMD_CLS;
}

void gdp_set_drawing_mode(drawing_mode mode) {
    if (mode_cache == mode) { return; }
    gdp_wait_ready();
    mode_cache = mode;
    // ...
}

void gdp_set_xy(uint16_t x, uint16_t y) {
    gdp_wait_ready();
    addr_t tmp;
    tmp.u16 = x;
    GDP_XPOS_HI = tmp.u8[0];
    GDP_XPOS_LO = tmp.u8[1];
    tmp.u16 = y;
    GDP_YPOS_HI = tmp.u8[0];
    GDP_YPOS_LO = tmp.u8[1];
}

void gdp_draw_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2)
{
    gdp_set_xy(x, y);
    
}