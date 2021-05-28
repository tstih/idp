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
#include <pong.h>

struct text_info ti;
short r1x, r2x, r1y, r2y; /* racket positions */
short bx, by; /* ball position */
short bdx,bdy; /* ball delta x and y */
char *keys="pl q";

void sleep (clock_t clks) {
    clock_t c1=clock();
    clock_t c2=c1;
    while (c2-c1<clks) { c2=clock(); };
}

short random(short max) {
    return 1+rand()%max;
}

void puts_center(char *s, short y, short x1, short x2, short mode) {
    /* Calculate correct position. */
    short l = strlen(s);
    if (mode!=M_NORMAL)
        l = l*2;
    short x = x1 + (x2-x1)/2 - l/2;
    if (mode!=M_NORMAL)
        x=x/2;
    /* Configure lines. */
    gotoxy(x,y);
    if (mode==M_DWDH) decdhl_top();
    else if (mode==M_DWSH) decdwl();
    cputs(s);
    /* Double height? */
    if (mode==M_DWDH) {
        gotoxy(x,y+1);
        decdhl_bottom();
        cputs(s);
    }
}

void welcome() {

    clrscr();

    puts_center("P O N G", ti.screenheight/2-1, 0, ti.screenwidth, M_DWDH);
    puts_center("Press space to start, q to quit", ti.screenheight/2+1,0,ti.screenwidth, M_DWSH);

    draw_rackets();

    while(true) {
        char c=kbhit();
        if (c==keys[KEY_QUIT]) quit();
        if (c==keys[KEY_START]) break;
    }
}

void victory(short player) {
    clrscr();

    if (player==1)
        puts_center("Player 1 (the computer) won!", ti.screenheight/2-1, 0, ti.screenwidth, M_DWDH);
    else
        puts_center("Player 2 (the human) won!", ti.screenheight/2-1, 0, ti.screenwidth, M_DWDH);

    puts_center("Press space to play again, q to quit", ti.screenheight/2+1,0,ti.screenwidth, M_DWSH);

    while(true) {
        char c=kbhit();
        if (c==keys[KEY_QUIT]) quit();
        if (c==keys[KEY_START]) break;
    }
}

void quit() {
    clrscr();
    cputs("Goodbye.\n\r");
    showcursor();
    exit(0);
}

void draw_racket(short x, short y1, short y2) {
    for(int y=y1;y<y2;y++) {
        gotoxy(x,y); putch(' ');
    }
}

void draw_rackets() {

    textattr(AT_INVERSE); textattr(AT_HIGHLIGHT);

    draw_racket(r1x,r1y,r1y+RACKET_HEIGHT);
    draw_racket(r2x,r2y,r2y+RACKET_HEIGHT);

    textattr(AT_NONE);
    
}

void racket_up(short rx,short *ry) {
    if ((*ry)>TOP) {
        /* Delete bottom part. */
        gotoxy(rx,(*ry)+RACKET_HEIGHT-1); putch(' ');
        (*ry)--;
        /* Draw upper part. */
        textattr(AT_INVERSE); textattr(AT_HIGHLIGHT);
        gotoxy(rx,(*ry)); putch(' ');
        textattr(AT_NONE);
    }
}

void racket_down(short rx,short *ry) {
    if ((*ry) < ti.screenheight-RACKET_HEIGHT) {
        /* Delete top part. */
        gotoxy(rx,(*ry)); putch(' ');
        /* Draw upper part. */
        textattr(AT_INVERSE); textattr(AT_HIGHLIGHT);
        gotoxy(rx,(*ry)+RACKET_HEIGHT); putch(' ');
        textattr(AT_NONE);
        (*ry)++;
    }
}

void draw_ball() {
    textattr(AT_BOLD_HIGHLIGHT); textattr(AT_HIGHLIGHT);
    gotoxy(bx,by);
    putch(' ');
    textattr(AT_NONE);
}

int move_ball() {

    /* the terminal seems terribly slow. 
    perhaps this is unnecessary. */
    sleep(1);

    /* check for victory condition */
    if (bx<r1x) return 2; /* player 2 won */
    else if (bx>r2x) return 1; /* player 1 won */ 
    
    /* delete old ball */
    gotoxy(bx,by); putch(' ');
    
    /* calculate new position */
    if (bx>r1x && bx+bdx<r1x) bx=r1x;  
    else if (bx<r2x && bx+bdx>r2x) bx=r2x; 
    else bx=bx+bdx;
    by=by+bdy;
    
    /* is the racket waiting for the ball? */
    if (bx==r1x && by>=r1y && by<=r1y+RACKET_HEIGHT) bdx=-bdx;
    else if (bx==r2x && by>=r2y && by<=r2y+RACKET_HEIGHT) bdx=-bdx;

    /* check top and bottom corners */    
    if (by<TOP) { by = TOP; bdy=-bdy; }
    if (by>BOTTOM) { by = BOTTOM; bdy=-bdy; }
    
    /* finally, draw the ball */
    draw_ball();

    return 0;
}

void ai_move() {
    /* ball going away from you... */
    bool do_nothing;
    if (bdx>0) do_nothing=(random(2)==1);
    else do_nothing=(random(5)==1);

    if (!do_nothing) {
        /* move in the direction of ball */
        if (bdy>0) racket_down(r1x,&r1y);
        else racket_up(r1x,&r1y);
    }
}

void main() {
    
    /* set random seed */
    srand(clock());

    /* Hide cursor, it's a game! */
    hidecursor();

    /* Get screen info. */
    gettextinfo(&ti);
    r1x=ti.screenwidth/2-RACKET_DISTANCE/2-1; r2x=r1x+RACKET_DISTANCE;
    r1y=ti.screenheight/2-START_OFFS;r2y=ti.screenheight/2+START_OFFS-RACKET_HEIGHT+1;
    
    /* Game menu loop */
    while(true) {
        /* Welcome screen. */
        welcome();
        /* If we are here, we started! */
        clrscr();
        draw_rackets();

        /* Play the game. */
        int winner=0;
        /* Randomize the ball */
        if (random(2)==1) bdx=-random(2); else bdx=random(2);
        if (random(2)==1) bdy=-random(2); else bdy=random(2);
        bx=ti.screenwidth/2-(random(BALLSPRDX)-random(BALLSPRDX)); 
        by=ti.screenheight/2-(random(BALLSPRDY)-random(BALLSPRDY));

        /* Draw the ball */
        draw_ball();

        while (!winner) {
            /* Handle player 1 */
            char c=kbhit();
            if (c==keys[KEY_UP]) racket_up(r2x,&r2y);
            if (c==keys[KEY_DOWN]) racket_down(r2x,&r2y);
            if (c==keys[KEY_QUIT]) quit();

            /* Move the computer opponent. */
            ai_move();

            /* Move the ball */
            winner=move_ball();
        }

        victory(winner);
    }
}