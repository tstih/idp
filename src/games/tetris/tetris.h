#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <stdint.h>
#include <stdbool.h>

// types 

typedef uint8_t* puint8_t;

// state

typedef enum {
	STATE_PLAY,
	STATE_PAUSE,
	STATE_GAME_OVER
} state;

extern state gameState;
extern bool showNext;
extern uint8_t level;
extern uint8_t steps;
extern long score;
extern uint8_t fullLines;
extern clock_t timer;
extern long timeLeft;
extern uint16_t stats[7];
//extern uint16_t row_addr[26];

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

key keyGet();

// playfield

extern uint8_t playfieldBkgr[20][10];

void playfieldInit();
void playfieldUpdateBkgr();
void playfieldClearRow(uint8_t row);
void playfieldMoveRow(uint8_t rowSrc, uint8_t rowDst);
uint8_t playfieldCollapse();

// render

void renderInit();
void renderPlayfieldRow(uint8_t row);
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
void renderTimer();

// block

extern puint8_t blockShapes[][4][4];

extern int8_t blockType;
extern uint8_t blockRot;
extern int8_t blockPosX;
extern int8_t blockPosY;

extern int8_t nextBlockType;

void blockInit();
bool blockMoveLeft();
bool blockMoveRight();
bool blockMoveDown();
bool blockRotate();
bool blockDrop();
bool blockCheck(int8_t posX, int8_t posY, uint8_t rot);
bool blockNext();

// timer

void timerReset();
bool timerDone();

// game

void gameInit();
bool gamePlay();

#endif