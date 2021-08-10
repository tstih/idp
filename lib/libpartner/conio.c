/*
 * conio.c
 *
 * parts of conio.h, implemented by libpartner
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.08.2021   tstih
 *
 */
#include <partner/conio.h>
#include <util/scn2674.h>

/* current text info */
static struct text_info ti;

/* initialization function */
__sfr __at 0xac _SETUP_WIDTH;
void _init_conio() {
    /* initialize text_info */
    ti.curx=0;
    ti.cury=0;
    ti.brightness=BR_NORMAL;
    ti.invvideo=false;
    ti.screenheight=24;; 
    ti.screenwidth = _SETUP_WIDTH-1;
}


void gettextinfo(struct text_info *r) {
    r->curx=ti.curx;
    r->cury=ti.cury;
    r->brightness=ti.brightness;
    r->invvideo=ti.invvideo;
    r->screenheight=ti.screenheight;
    r->screenwidth=ti.screenwidth;
}


int kbhit(void) {
    return bdos(C_RAWIO,0xff);
}


int wherex(void) {
    return ti.curx;
}


int wherey(void) {
    return ti.cury;
}


/* TODO: ignore difference between solid/normal cursor type for now */
void setcursortype(int cur_t) {
    if (cur_t==NOCURSOR)
        _scn2674_cursor_off();
    else
        _scn2674_cursor_on();
}


void highvideo(void) {
    ti.brightness=BR_HIGH;
}


void lowvideo(void) {
    ti.brightness=BR_LOW;
}


void invvideo(bool inv) {
    ti.invvideo=inv;
}


void normvideo(void) {
    ti.brightness=BR_NORMAL;
}