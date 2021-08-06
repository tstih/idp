/*
 * conio.c
 *
 * DOS/Windows like implementation of conio.h for fast text ops.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.08.2021   tstih
 *
 */
#ifndef __CONIO_H__
#define __CONIO_H__

#include <stdbool.h>

#include <sys/bdos.h>

#define BR_LOW     0
#define BR_NORMAL  1
#define BR_HIGH    2

struct text_info {
    unsigned char brightness;   /* high, low, normal */
    bool invvideo;              /* inverse video */
    unsigned char screenheight; /* text screen's height */
    unsigned char screenwidth;  /* text screen's width */
    unsigned char curx;         /* x-coordinate */
    unsigned char cury;         /* y-coordinate */
}; 

/* check for keypress (don't block), return 0 if no key */
extern int kbhit(void);

/* move cursor to x,y */
extern void gotoxy(int x, int y);

/* clear screen and move cursor to 0,0 */
extern void clrscr(void);

/* select high intensity characters */
extern void highvideo(void);

/* select low intensity characters */
extern void lowvideo(void);

/* select inverse video on/off */
extern void invvideo(bool inv);

/* normal intensity */
extern void normvideo(void);

/* get screen dimensions */
extern void gettextinfo(struct text_info *r);

/* fast put character */
extern int putch(int c);

/* fast put string */
extern int cputs(const char *str);

/* fast console printf */
extern int cprintf(const char *format,...);

/* current x */
extern int wherex(void);

/* current y */
extern int wherey(void);

/* set cursor type */
#define NOCURSOR        0
#define NORMALCURSOR    1
#define SOLIDCURSOR     2
void setcursortype(int cur_t);

#endif /* __CONIO_H__ */