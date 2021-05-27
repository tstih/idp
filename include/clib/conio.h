/*
 * conio.h
 *
 * non standard conio implementation, uses basic
 * terminal sequences to simulate conio functions.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 23.05.2021   tstih
 *
 */
#ifndef __CONIO_H__
#define __CONIO_H__

/* Terminal type */
#define T_PARTNER           0x00
#define T_VT52              0x01
#define T_ANSI              0x02

/* Text attributes - these are loosely compatible with
VT52 SGR attributes */
#define AT_NONE             0x00
#define AT_BOLD             0x01
#define AT_HIGHLIGHT        0x02
#define AT_BOLD_HIGHLIGHT   0x03
#define AT_UNDERLINE        0x04
#define AT_BLINK            0x05
#define AT_INVERSE          0x07

/* Basic info about the terminal */
struct text_info {
    unsigned char screenwidth;
    unsigned char screenheight;
    unsigned char terminal;
};

/* Basic information about the screen. */
extern void gettextinfo(struct text_info *ti);

/* Clears the screen. Moves cursor to 0,0. */
extern void clrscr(void);

/* A single character from the predefined 
standard input handle is read and returned. */
extern int getch(void);

/* Puts char back so that getch returns it again. */
extern int ungetch(int);

/* Move cursor */
extern void gotoxy(int x, int y);

/* Put char. */
extern int putch(int c);

/* Put string */
extern int cputs(char *s);

/* Hide cursor. */
extern void hidecursor();

/* Show cursor. */
extern void showcursor();

/* Non blocking keyboard read. */
extern int kbhit();

/* Delete until end of line. */
extern void clreol();

/* Delete current line/ */
extern void delline();

/* Set text attributes. */
extern void textattr(unsigned char attr);

/* DEC Duble-Width, Double-Height Line top */
extern void decdhl_top();

/* DEC Duble-Width, Double-Height Line bottom */
extern void decdhl_bottom();

/* DEC Duble-Width, Single-Height Line */
extern void decdwl();

/* DEC Single-Width, Single-Height Line top */
extern void decswl();

#endif /* __CONIO_H__ */