/*
 * conio-xp.c
 *
 * conio effects.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 24.05.2021   tstih
 *
 */
#include <conio.h>
#include <stdio.h>

void main() {
    
    clrscr();
    
    /* first get terminal info */
    struct text_info ti;
    gettextinfo(&ti);
    if (ti.terminal==T_PARTNER)
        puts("Terminal: PARTNER ");
    else if (ti.terminal==T_VT52)
        puts("Terminal: VT52    ");
    else if (ti.terminal==T_ANSI)
        puts("Terminal: ANSI    ");
    printf("Dimensions: %dx%d\n\r", ti.screenwidth, ti.screenheight);

    if (ti.terminal==T_PARTNER) {
        printf("Can't do effects on the Partner terminal.\n\r");
        return;
    }

    gotoxy(3,3);
    textattr(AT_UNDERLINE);
    cputs("ASCII TABLE\n\r");
    textattr(AT_NONE);

    int x=3, y=3, ascii=32;
    while (ascii<128) {
        if (ascii%32==0) { y++; x=3; }
        gotoxy(x,y);
        putch(ascii);
        x+=2;
        ascii++;
    }

    /* core attributes */
    gotoxy(3,8); textattr(AT_NONE); cputs("Normal"); textattr(AT_NONE);
    cputs(", "); textattr(AT_BOLD); cputs("Bold"); textattr(AT_NONE);
    cputs(", "); textattr(AT_HIGHLIGHT); cputs("Highlight"); textattr(AT_NONE);
    cputs(", ");  textattr(AT_BOLD_HIGHLIGHT); cputs("Bold Highlight"); textattr(AT_NONE); /* 1+2 */
    gotoxy(3,9);  textattr(AT_UNDERLINE); cputs("Underline"); textattr(AT_NONE);
    cputs(", ");  textattr(AT_BLINK); cputs("Blink"); textattr(AT_NONE); /*1+4 */
    cputs(", ");  textattr(AT_INVERSE); cputs("Inverse black text"); textattr(AT_NONE);

    /* multi attr. */
    gotoxy(3,10); textattr(AT_BOLD); textattr(AT_INVERSE); 
    cputs("Bold, Inverse"); 
    textattr(AT_HIGHLIGHT); cputs("+Highlight");
    textattr(AT_UNDERLINE); cputs("+Underline");
    textattr(AT_NONE);

    gotoxy(0,12);
    /* Draw two lines of text (top and bottom) 
    use sequences 
     - DEC Double-Height Letters, Top Half
     - DEC Double-Height Letters, Bottom Half */
    printf("\x1b#3 Please, send photo.\n\r\x1b#4 Please, send photo.\n\r");

    /* test SGR attributes */
    gotoxy(0,15);
    /* use DEC Double-Width Line seq. */
    cputs("\x1b#6 Amd have a nice day.");

   gotoxy(3,17);
}