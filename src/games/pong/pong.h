/*
 * pong.c
 *
 * Atari PONG. 
 *
 * copyright (c) 2021 tomaz stih
 *
 * 27.05.2021   tstih
 *
 */
#ifndef __PONG_H__
#define __PONG_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>


/* Basic game parameters */
#define RACKET_DISTANCE 65
#define RACKET_HEIGHT   4
#define START_OFFS      7
#define TOP             2
#define BOTTOM          24
#define BALLSPRDX       15
#define BALLSPRDY       5

/* Game key index */
#define KEY_UP      0
#define KEY_DOWN    1
#define KEY_START   2
#define KEY_QUIT    3
extern char *keys;

/* Text mode for puts_center */
#define M_NORMAL    0
#define M_DWDH      1
#define M_DWSH      2


/* Terminal info. */
extern struct text_info ti;

/* Racket and ball coordinates. */
extern short r1x, r2x, r1y, r2y;    /* racket positions */
extern short bx, by;                /* ball position */
extern short bdx,bdy;               /* ball delta x and y */


/* sleep (in milliseconds */
extern void sleep (clock_t clks);

/* random between 1 and max */
extern short random(short max);

/* draw string between x1 and x2, using size mode */
extern void puts_center(char *s, short y, short x1, short x2, short mode);

/* welcome screen */
extern void welcome();

/* victory screen, player=winner */
extern void victory(short player);

/* quit pong! */
extern void quit();

/* draw a racket */
extern void draw_racket(short x, short y1, short y2);

/* draw both rackets (initial draw) */
extern void draw_rackets();

/* quick move racket up */
extern void racket_up(short rx,short *ry);

/* quick move racket down */
extern void racket_down(short rx,short *ry);

/* draw the ball */
extern void draw_ball();

/* move the ball, return 0 or player who won (1,2) */
extern int move_ball();

#endif /* __PONG_H__ */
