/*
 * tetris.c
 *
 * Tetris. 
 *
 * copyright (c) 2021 tomaz stih
 *
 * 23.05.2021   mgrcar
 *
 */
#include <tetris.h>

extern char cpm_getchar_nonblock();

void main() {
    clrscr();
    cputs("Press any key to abort...\n\r");
    while (!kbhit());
    cputs("\n\r");
}