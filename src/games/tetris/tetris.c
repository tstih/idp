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

#include <time.h>

 // defs

typedef unsigned char byte;

const byte TRUE = 1;
const byte FALSE = 0;

// state

const byte STATE_PLAY = 1;
const byte STATE_PAUSE = 2;
const byte STATE_GAME_OVER = 4;

byte state;
byte showNext;
byte level;
time_t timer;
long timeLeft;

// key

const byte KEY_OTHER = 0;
const byte KEY_LEFT = 1;
const byte KEY_RIGHT = 2;
const byte KEY_ROTATE = 3;
const byte KEY_DROP = 4;
const byte KEY_PAUSE = 5;
const byte KEY_SHOW_NEXT = 6;
const byte KEY_SPEED_UP = 7;

// block

byte blocks[][4][2] = {
	{
		{ /*0b00000111*/'\007', /*0b01000000*/'\064' },
		{ /*0b00100010*/'\034', /*0b00110000*/'\048' },
		{ /*0b00010111*/'\023', /*0b00000000*/'\000' },
		{ /*0b01100010*/'\098', /*0b00100000*/'\032' }
	}//,
	//...
};

byte blockMoveLeft()
{
	// TODO
	return TRUE;
}

byte blockMoveRight()
{
	// TODO
	return TRUE;
}

byte blockMoveDown()
{
	// TODO
	return TRUE;
}

byte blockRotate()
{
	// TODO
	return TRUE;
}

byte blockDrop()
{
	// TODO
	return TRUE;
}

byte blockNew()
{
	// TODO
	return TRUE;
}

// render

void renderInit()
{
	// TODO
}

void renderPlayfield()
{
	// TODO
}

void renderPause()
{
	// TODO
}

void renderNextBlock()
{
	// TODO
}

void renderClearNextBlock()
{
	// TODO
}

void renderLevel()
{
	// TODO
}

// timer

void resetTimer()
{
	time(&timer);
}

byte timerDone()
{
	time_t now;
	time(&now);
	return difftime(now, timer)/*WARNME: milliseconds?*/ >= (10UL - (unsigned long)level) * 50UL; // WARNME
}

// init

void init()
{
	// init state
	state = STATE_PLAY;
	showNext = FALSE;
	level = 0;
	time(&timer);
	timeLeft = 0.0f;

	renderInit();
	renderPlayfield();
	blockNew();
	resetTimer();
}

// keyboard input

byte getKey()
{
	return KEY_OTHER;
}

// game logic

void play()
{
	if (state == STATE_PLAY)
	{
		switch (getKey())
		{
		KEY_LEFT:
			blockMoveLeft();
			break;
		KEY_RIGHT:
			blockMoveRight();
			break;
		KEY_ROTATE:
			blockRotate();
			break;
		KEY_DROP:
			blockDrop();
			break;
		KEY_PAUSE:
			{
				time_t now;
				time(&now);
				timeLeft = difftime(now, timer);
				renderPause();
				state = STATE_PAUSE;
			}
			break;
		KEY_SHOW_NEXT:
			showNext = !showNext;
			if (showNext) { renderNextBlock(); }
			else { renderClearNextBlock(); }
			break;
		KEY_SPEED_UP:
			level++;
			if (level > 9) { level = 9; }
			renderLevel();
			break;
		}
		if (timerDone())
		{
			if (!blockMoveDown())
			{
				// TODO
			}
		}
		else if (state == STATE_PAUSE)
		{
			// TODO
		}
		else if (state == STATE_GAME_OVER)
		{
			// TODO
		}
	}
}

// main game loop

void game()
{
	init();
	while (TRUE)
	{
		play();
		// sleep?
	}
}

// main program

int main()
{
	// TODO
	return 0;
}

// extern char cpm_getchar_nonblock();

// void main() {
//     clrscr();
//     cputs("Press any key to abort...\n\r");
//     while (!kbhit());
//     cputs("\n\r");
// }