#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <time.h>
#include <stdint.h>

// basics 

typedef int8_t sbyte; 
typedef uint8_t byte;
typedef uint8_t* pstr;
typedef uint8_t bool;

#define TRUE 1
#define FALSE 0

#define NULL (void *)0

// state

typedef enum {
	STATE_PLAY,
	STATE_PAUSE,
	STATE_GAME_OVER
} state;

state gameState;
bool showNext;
byte level;
byte steps;
long score;
byte fullLines;
clock_t timer;
long timeLeft;
uint16_t stats[7];

void stateInit();

// key

typedef enum {
	KEY_NONE,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ROTATE,
	KEY_DROP,
	KEY_PAUSE,
	KEY_SHOW_NEXT,
	KEY_SPEED_UP,
	KEY_RESTART,
	KEY_OTHER
} key;

key getKey();

// playfield

byte playfieldBkgr[20][10];

void playfieldInit();
void playfieldUpdateBkgr();
byte playfieldCollapse();

// render

void renderInit();
void renderPlayfield();
void renderPause();
void renderClearPause();
void renderNextBlock();
void renderClearNextBlock();
void renderLevel();
void renderScore();
void renderFullLines();
void renderGameOver();
void renderGoodbye();
void renderBlock();
void renderClearBlock();
void renderStats();

// block

pstr blockShapes[][4][4] = {
	{
		{ "0000","0111","0100","0000" },
		{ "0010","0010","0011","0000" },
		{ "0001","0111","0000","0000" },
		{ "0110","0010","0010","0000" }
	},
	{
		{ "0000","2222","0000","0000" },
		{ "0020","0020","0020","0020" },
		{ "0000","2222","0000","0000" },
		{ "0020","0020","0020","0020" }
	},
	{
		{ "0000","0333","0030","0000" },
		{ "0030","0033","0030","0000" },
		{ "0030","0333","0000","0000" },
		{ "0030","0330","0030","0000" }
	},
	{
		{ "0000","0044","0440","0000" },
		{ "0040","0044","0004","0000" },
		{ "0000","0044","0440","0000" },
		{ "0040","0044","0004","0000" }
	},
	{
		{ "0000","0550","0055","0000" },
		{ "0005","0055","0050","0000" },
		{ "0000","0550","0055","0000" },
		{ "0005","0055","0050","0000" }
	},
	{
		{ "0000","0660","0660","0000" },
		{ "0000","0660","0660","0000" },
		{ "0000","0660","0660","0000" },
		{ "0000","0660","0660","0000" }
	},
	{
		{ "0000","0777","0007","0000" },
		{ "0077","0070","0070","0000" },
		{ "0700","0777","0000","0000" },
		{ "0070","0070","0770","0000" }
	}
};

sbyte blockType;
byte blockRot;
sbyte blockPosX;
sbyte blockPosY;

sbyte nextBlockType;

void blockInit();
bool blockMoveLeft();
bool blockMoveRight();
bool blockMoveDown();
bool blockRotate();
bool blockDrop();
bool blockCheck(sbyte posX, sbyte posY, byte rot);
bool blockNext();

// timer

void timerReset();
bool timerDone();

// init game

void init();

// game logic

bool play();

#endif