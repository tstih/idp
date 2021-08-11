# Auxiliary Library for Iskra Delta Partner

## Introduction

**libpartner** abstracts access to *Partner* hardware. 
It's purpose is to supplement the *libcpm3-z80*.
It implements all platform specific functions required
by the later.

Beyond supplementing the standard library, *libpartner*
implements some additional functionality.


| Header     | Content                             |
|------------|-------------------------------------| 
| bcd.h      | Support for 8-bit BCD conversions.  |
| conio.h    | Fast console (text mode) functions. |
| syinfo.h   | Querying the system info.           |

## How do I use it?

Link the `libpartner.lib` from the `bin` directory with your
project, and include `partner.h` header file in you source 
file.

## What is in the headers?

Click on the header name to see the scope of its implementation.

<details><summary>bcd.h/</summary>

~~~cpp
/* convert byte to bcd */
extern uint8_t bin2bcd(uint8_t bin);

/* convert bcd to byte */
extern uint8_t bcd2bin(uint8_t bcd);
~~~
</details>  

<details><summary>conio.h/</summary>

~~~cpp
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
extern void setcursortype(int cur_t);
~~~
</details>  

<details><summary>sysinfo.h/</summary>

~~~cpp
/* get partner model, the values are also 
   bitmasks 
   bit meaning
    0  1=2xFDD, 0=1xFDD
    1  HDD
    2  Graphics */
#define M_1F    0x00                    /* 1 FDD */
#define M_2F    0x01                    /* 2 FDD */
#define M_WF    0x02                    /* HDD + FDD */
#define M_1FG   0x04                    /* 1 FDD + G */
#define M_2FG   0x05                    /* 2 FDD + G */
#define M_WFG   0x06                    /* HDD + FDD + G */
extern int model();

/* is it the emulator? */
extern bool isemu();
~~~
</details>  

## To Do

The library is in alpha test. Console functions are not
working yet.