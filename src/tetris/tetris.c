/*
 * tetris.h
 *
 * Partner Terminal Micro Tetris. 
 * 
 * Based on Micro Tetris.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 *
 * copyright (c) 1989 IOCCC Best Game
 * copyright (c) 1989  John Tromp <john.tromp@gmail.com>
 * copyright (c) 2009, 2010, 2017  Joachim Nilsson <troglobit@gmail.com>
 * copyright (c) 2021 tomaz stih
 *
 * 23.05.2021   tstih
 *
 */
#include <tetris.h>

static int havemodes = 0;

#define TL -B_COLS - 1 /* top left */
#define TC -B_COLS     /* top center */
#define TR -B_COLS + 1 /* top right */
#define ML -1          /* middle left */
#define MR 1           /* middle right */
#define BL B_COLS - 1  /* bottom left */
#define BC B_COLS      /* bottom center */
#define BR B_COLS + 1  /* bottom right */

/* These can be overridden by the user. */
#define DEFAULT_KEYS "jkl pq"
#define KEY_LEFT 0
#define KEY_RIGHT 2
#define KEY_ROTATE 1
#define KEY_DROP 3
#define KEY_PAUSE 4
#define KEY_QUIT 5

char *keys = DEFAULT_KEYS;
int level = 1;
int points = 0;
int lines_cleared = 0;
int board[B_SIZE], shadow[B_SIZE];

int *peek_shape; /* peek preview of next shape */
int *shape;

int shapes[] = {
    7, TL, TC, MR,
    8, TR, TC, ML,
    9, ML, MR, BC,
    3, TL, TC, ML,
    12, ML, BL, MR,
    15, ML, BR, MR,
    18, ML, MR, 2, /* sticks out */
    0, TC, ML, BL,
    1, TC, MR, BR,
    10, TC, MR, BC,
    11, TC, ML, MR,
    2, TC, ML, BC,
    13, TC, BC, BR,
    14, TR, ML, MR,
    4, TL, TC, BC,
    16, TR, TC, BC,
    17, TL, MR, ML,
    5, TC, BC, BL,
    6, TC, BC, 2 * B_COLS, /* sticks out */
};

int update(void)
{
    int x, y;

    /* Display board. */
    for (y = 1; y < B_ROWS - 1; y++)
    {
        for (x = 0; x < B_COLS; x++)
        {
            if (board[y * B_COLS + x] - shadow[y * B_COLS + x])
            {
                shadow[y * B_COLS + x] = board[y * B_COLS + x];
                gotoxy(x * 2 + 28, y);
                printf("\e[%dm  ", board[y * B_COLS + x]);
            }
        }
    }

    /* Update points and level */
    while (lines_cleared >= 10)
    {
        lines_cleared -= 10;
        level++;
    }
    gotoxy(26 + 28, 10);
    printf("Keys:");

    return getch();
}

int fits_in(int *shape, int pos)
{
    if (board[pos] || board[pos + shape[1]] || board[pos + shape[2]] || board[pos + shape[3]])
        return 0;

    return 1;
}

void place(int *shape, int pos, int b)
{
    board[pos] = b;
    board[pos + shape[1]] = b;
    board[pos + shape[2]] = b;
    board[pos + shape[3]] = b;
}

int *next_shape(void)
{
    int *next = peek_shape;

    peek_shape = &shapes[rand() % 7 * 4];
    if (!next)
        return next_shape();

    return next;
}

void show_online_help(void)
{
    const int start = 11;

    gotoxy(26 + 28, start);
    puts("j     - left");
    gotoxy(26 + 28, start + 1);
    puts("k     - rotate");
    gotoxy(26 + 28, start + 2);
    puts("l     - right");
    gotoxy(26 + 28, start + 3);
    puts("space - drop");
    gotoxy(26 + 28, start + 4);
    puts("p     - pause");
    gotoxy(26 + 28, start + 5);
    puts("q     - quit");
}

int main()
{
    int c = 0, i, j, *ptr;
    int pos = 17;
    int *backup;

    /* Initialize board */
    ptr = board;
    for (i = B_SIZE; i; i--)
        *ptr++ = i < 25 || i % B_COLS < 2 ? 7 : 0;

    srand((unsigned int)time(NULL));

    /* initialize screen */

    clrscr();
    show_online_help();

    shape = next_shape();
    while (1)
    {
        if (c < 0)
        {
            if (fits_in(shape, pos + B_COLS))
            {
                pos += B_COLS;
            }
            else
            {
                place(shape, pos, 7);
                ++points;
                for (j = 0; j < 252; j = B_COLS * (j / B_COLS + 1))
                {
                    for (; board[++j];)
                    {
                        if (j % B_COLS == 10)
                        {
                            lines_cleared++;

                            for (; j % B_COLS; board[j--] = 0)
                                ;
                            c = update();

                            for (; --j; board[j + B_COLS] = board[j])
                                ;
                            c = update();
                        }
                    }
                }
                shape = next_shape();
                if (!fits_in(shape, pos = 17))
                    c = keys[KEY_QUIT];
            }
        }

        if (c == keys[KEY_LEFT])
        {
            if (!fits_in(shape, --pos))
                ++pos;
        }

        if (c == keys[KEY_ROTATE])
        {
            backup = shape;
            shape = &shapes[4 * *shape]; /* Rotate */
            /* Check if it fits, if not restore shape from backup */
            if (!fits_in(shape, pos))
                shape = backup;
        }

        if (c == keys[KEY_RIGHT])
        {
            if (!fits_in(shape, ++pos))
                --pos;
        }

        if (c == keys[KEY_DROP])
        {
            for (; fits_in(shape, pos + B_COLS); ++points)
                pos += B_COLS;
        }

        if (c == keys[KEY_PAUSE] || c == keys[KEY_QUIT])
        {
            /* TODO: pause */

            if (c == keys[KEY_QUIT])
            {
                clrscr();
                gotoxy(0, 0);
                break;
            }

            for (j = B_SIZE; j--; shadow[j] = 0)
                ;

            while (getch() - keys[KEY_PAUSE])
                ;

            /* unfreeye */
        }

        place(shape, pos, 7);
        c = update();
        place(shape, pos, 0);
    }

    return 0;
}