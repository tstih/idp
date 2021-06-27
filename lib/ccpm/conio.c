/*
 * conio.c
 *
 * non-standard conio.c, translates calls to terminal
 * functions
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 23.05.2021   tstih
 *
 */
#include <cpm_sysfunc.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <port.h>

/* Last character (for unget) */
static int _lastch;

void _conio_init() {
    _lastch = 0;
}

/* Read terminal type from the setup. */
#define PORT_TTYPE 0xab
uint8_t _terminal_type() {
    uint8_t tt=_port_read(PORT_TTYPE)>>4;
    if (tt==0) return T_ANSI;
    else if(tt==1) return T_VT52;
    else return T_PARTNER;
}

#define PORT_TWIDTH 0xac
uint8_t _terminal_width() {
    return _port_read(PORT_TWIDTH) - 1;
}

#define TERM_HEIGHT 24
/* Basic information about the screen. */
void gettextinfo(struct text_info *ti) {
    ti->screenwidth=_terminal_width() ;
    ti->screenheight=TERM_HEIGHT; /* fixed */
    ti->terminal = _terminal_type();
}

int kbhit() {
    return cpm_getchar_nonblock();
}

/* Clears the screen. Moves cursor to 0,0. */

void clrscr() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            puts("\x1b[2J\x1b[1;1H");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* A single character from the predefined 
standard input handle is read and returned. */
int getch(void) {
    int ch=_lastch;
    if (ch) {
        _lastch=0;
        return ch;
    } else
        return cpm_getchar();
}

/* Puts char back so that getch returns it again. */
int ungetch(int c) {
    int prev=_lastch;
    _lastch=c;
    return prev;
}

/* Move cursor */
void gotoxy(int x, int y) {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b[%d;%dH", y + 1, x + 1);
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* Put char. */
int putch(int c) {
    cpm_putchar(c);
    return 0;
}

/* Put string */
int cputs(char *s) {
    while (*s) putch(*s++);
    return 0;
}

void hidecursor() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            puts("\x1b[?25l");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

void showcursor() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            puts("\x1b[?25h");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

void clreol() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            puts("\x1b[K");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

void delline() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            puts("\x1b[2K");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

void textattr(uint8_t attr) {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b[%dm",attr);
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* DEC Duble-Width, Double-Height Line top */
void decdhl_top() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b#3");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* DEC Duble-Width, Double-Height Line bottom */
void decdhl_bottom() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b#4");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* DEC Duble-Width, Single-Height Line */
void decdwl() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b#6");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}

/* DEC Single-Width, Single-Height Line top */
void decswl() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\x1b<"); /* Enter ansi mode */
        case T_ANSI:
            printf("\x1b#5");
            break;
        case T_PARTNER:
        default:
            errno=ENOTTY;
            break;
    }
}