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

/* Last character (for unget) */
static int _lastch = 0;

/* Read terminal type from the setup. */
uint8_t _terminal_type() {
    return T_PARTNER;
}

#define TERM_HEIGHT 25
uint8_t _terminal_width() {
    return 80;
}

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
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            puts("\e[2J\e[1;1H");
            break;
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
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            printf("\e[%d;%dH", y, x);
            break;
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
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            puts("\e[?25l");
            break;
        default:
            errno=ENOTTY;
            break;
    }
}

void showcursor() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            puts("\e[?25h");
            break;
        default:
            errno=ENOTTY;
            break;
    }
}

void clreol() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            puts("\e[K");
            break;
        default:
            errno=ENOTTY;
            break;
    }
}

void delline() {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            puts("\e[2K");
            break;
        default:
            errno=ENOTTY;
            break;
    }
}

void textattr(uint8_t attr) {
    switch(_terminal_type()) {
        case T_VT52:
            puts("\e<"); /* Enter ansi mode */
        case T_PARTNER:
        case T_ANSI:
            printf("\e[P%d m",attr);
            break;
        default:
            errno=ENOTTY;
            break;
    }
}