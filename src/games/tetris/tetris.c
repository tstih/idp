#include <stdlib.h>
#include "tetris.h"
#include "avdc.h"
#include "gdp.h"
#include "utils.h"

// string buffer

uint8_t str_buffer[133];

// state

state gameState;
bool showNext;
uint8_t level;
uint8_t steps;
long score;
uint8_t fullLines;
long timePassed;
uint16_t stats[7];

void stateInit() {
	gameState = STATE_PLAY;
	showNext = false;
	level = 0;
	steps = 0;
	score = 0;
	fullLines = 0;
	timer_reset(0); 
	timePassed = 0;
	srand(_timer());
	for (uint8_t i = 0; i < 7; i++) { 
		stats[i] = 0; 
	}
}

// key

key keyGet() {
	char key;
	if (!(key = cpm_getchar_nonblock())) { return KEY_NONE; }
	switch (key) {
	case 'a':
	case 'A':
		return KEY_LEFT;
	case 'd':
	case 'D':
		return KEY_RIGHT;
	case 'w':
	case 'W':
		return KEY_ROTATE;
	case 's':
	case 'S': 
		return KEY_DROP;
	case 'r':
	case 'R':
		return KEY_RESTART;
	case 'p':
	case 'P':
		return KEY_PAUSE;
	case 'n':
	case 'N':
		return KEY_SHOW_NEXT;
	case '+':
		return KEY_SPEED_UP;
	default:
		return KEY_OTHER;
	}
}

// playfield

uint8_t playfieldBkgr[20][10];

void playfieldInit() {
	for (uint8_t i = 0; i < 20; i++) {
		playfieldClearRow(i);
	}
}

void playfieldUpdateBkgr() {
	for (uint8_t r = 0; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			for (uint8_t c = 0; c < 4; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					if (blockShapes[blockType][blockRot][r][c] != '0') {
						playfieldBkgr[blockPosY + r][blockPosX + c] = blockShapes[blockType][blockRot][r][c];
					}
				}
			}
		}
	}
}

void playfieldClearRow(uint8_t row) {
	for (uint8_t j = 0; j < 10; j++) {
		playfieldBkgr[row][j] = '0';
	}
}

void playfieldMoveRow(uint8_t rowSrc, uint8_t rowDst) {
	for (uint8_t j = 0; j < 10; j++) {
		playfieldBkgr[rowDst][j] = playfieldBkgr[rowSrc][j];
	}
	playfieldClearRow(rowSrc);
}

uint8_t playfieldCollapse() {
	uint8_t lines = 0;
	for (int8_t y = 19; y >= 0; y--) {
		bool fullLine = true;
		for (uint8_t x = 0; x < 10; x++) {
			if (playfieldBkgr[y][x] == '0') { fullLine = false; break; }
		}
		if (fullLine) {
			lines++;
			// clear line y
			playfieldClearRow(y);
			renderPlayfieldRow(y);
			// shift lines
			for (int8_t yy = y - 1; yy >= 0; yy--) {
				// copy line from yy to yy + 1
				playfieldMoveRow(yy, yy + 1);
				renderPlayfieldRow(yy);
				renderPlayfieldRow(yy + 1);
			}
			y++;
		}
	}
	return lines;
}

// render

void renderInit() {
	avdc_set_cursor_write_str(0, 0, "This is renderInit", NULL);
}

void renderPlayfieldRow(uint8_t row) {
	avdc_set_cursor(row, 30);
	for (uint8_t j = 0; j < 10; j++) {
		avdc_write_at_cursor(playfieldBkgr[row][j], AVDC_DEFAULT_ATTR);
	}
}

void renderPlayfield() {
	for (uint8_t i = 0; i < 20; i++) {
		renderPlayfieldRow(i);
	}
}

void renderPause() {
	avdc_set_cursor_write_str(1, 0, "PAUSED", NULL);
}

void renderClearPause() {
	avdc_set_cursor_write_str(1, 0, "      ", NULL);
}

void renderNextBlock() {
	for (uint8_t r = 0; r < 4; r++) {
		avdc_set_cursor(10 + r, 4);
		for (uint8_t c = 0; c < 4; c++) {
			avdc_write_at_cursor(
				blockShapes[nextBlockType][0][r][c] == '0' ? ' ' : blockShapes[nextBlockType][0][r][c],
				AVDC_DEFAULT_ATTR
			);
		}
	}
}

void renderClearNextBlock() {
	for (uint8_t r = 0; r < 4; r++) {
		avdc_set_cursor_write_str(10 + r, 4, "    ", NULL);
	}
}

void renderLevel() {
	avdc_set_cursor_write_str(2, 0, "Level: ", NULL);
	avdc_set_cursor_write_str(2, 7, itoa(level, str_buffer), NULL);
}

void renderScore() {
	avdc_set_cursor_write_str(3, 0, "Score: ", NULL);
	avdc_set_cursor_write_str(3, 7, itoa(score, str_buffer), NULL);
}

void renderFullLines() {
	avdc_set_cursor_write_str(4, 0, "Full lines: ", NULL);
	avdc_set_cursor_write_str(4, 12, itoa(fullLines, str_buffer), NULL);
}

void renderGameOver() {
	avdc_set_cursor_write_str(5, 0, "GAME OVER", NULL);
}

void renderGoodbye() {
	avdc_set_cursor_write_str(5, 0, "GOODBYE !", NULL);
}

void renderClearBlock() {
	for (uint8_t r = 0; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			avdc_set_cursor(blockPosY + r, 30 + (blockPosX < 0 ? 0 : blockPosX));
			for (uint8_t c = 0; c < 4; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					avdc_write_at_cursor(playfieldBkgr[blockPosY + r][blockPosX + c], AVDC_DEFAULT_ATTR);
				}
			}
		}
	}
}

void renderBlock() {
	for (int8_t r = -1; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			avdc_set_cursor(blockPosY + r, 30 + ((blockPosX - 1) < 0 ? 0 : (blockPosX - 1)));
			for (int8_t c = -1; c < 5; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					if (c < 0 || c > 3 || r < 0 || r > 3) {
						avdc_write_at_cursor(playfieldBkgr[blockPosY + r][blockPosX + c], AVDC_DEFAULT_ATTR);
					} else {
						avdc_write_at_cursor(
							blockShapes[blockType][blockRot][r][c] == '0' ? playfieldBkgr[blockPosY + r][blockPosX + c] : blockShapes[blockType][blockRot][r][c],
							AVDC_DEFAULT_ATTR
						);
					}
				}
			}
		}
	}
}

void renderStats() {
	for (uint8_t i = 0; i < 7; i++) {
		avdc_set_cursor_write_str(6 + i, 0, itoa(stats[i], str_buffer), NULL);
	}
}

// block

puint8_t blockShapes[][4][4] = {
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

int8_t blockType;
uint8_t blockRot;
int8_t blockPosX;
int8_t blockPosY;

int8_t nextBlockType;

void blockInit() {
	blockType = -1;
	blockRot = 0;
	blockPosX = 3;
	blockPosY = -1;

	nextBlockType = -1;
}

bool blockMoveLeft() {
	if (blockCheck(blockPosX - 1, blockPosY, blockRot)) {
		blockPosX--;
		renderBlock();
		return true;
	}
	return false;
}

bool blockMoveRight() {
	if (blockCheck(blockPosX + 1, blockPosY, blockRot)) {
		blockPosX++;
		renderBlock();
		return true;
	}
	return false;
}

bool blockCheck(int8_t posX, int8_t posY, uint8_t rot) {
	for (int8_t blockY = 0, gridY = posY; blockY < 4; blockY++, gridY++) {
		for (int8_t blockX = 0, gridX = posX; blockX < 4; blockX++, gridX++) {
			if (blockShapes[blockType][rot][blockY][blockX] != '0' && 
				(gridX < 0 || gridY < 0 || gridX >= 10 || gridY >= 20 || playfieldBkgr[gridY][gridX] != '0')) {
				return false;
			}
		}
	}
	return true;
}

bool blockMoveDown() {
	if (blockCheck(blockPosX, blockPosY + 1, blockRot)) {
		blockPosY++;
		renderBlock();
		return true;
	}
	return false;
}

bool blockRotate() {
	uint8_t rot = (blockRot + 1) % 4;
	if (blockCheck(blockPosX, blockPosY, rot)) {
		blockRot = rot;
		renderBlock();
		return true;
	}
	return false;
}

bool blockDrop() {
	bool success = blockCheck(blockPosX, blockPosY + 1, blockRot);
	if (success) {
		renderClearBlock();
		while (blockCheck(blockPosX, blockPosY + 1, blockRot)) { blockPosY++; }
		renderBlock();
	}
	return success;
}

bool blockNext() {
	if (nextBlockType == -1) {
		nextBlockType = rand() % 7;
	}
	blockType = nextBlockType;
	blockRot = 0;
	blockPosX = 3;
	blockPosY = -1;
	stats[blockType]++;
	if (stats[blockType] > 1428) { stats[blockType] = 1428; } // prevent overflow (also of the overall sum)
	renderStats();
	nextBlockType = rand() % 7;
	bool success = blockCheck(blockPosX, blockPosY, blockRot);
	renderBlock();
	if (showNext) { renderNextBlock(); }
	return success;
}

// timer

bool timerDone() {
	long span = (10 - level) * 5;
	return timer_diff() >= span;
}

// game

void gameInit() {
	stateInit();
	playfieldInit();
	blockInit();

	renderInit();
	renderPlayfield();
	blockNext();
}

bool gamePlay() {
	key key;
	if (gameState == STATE_PLAY) {
		key = keyGet();
		switch (key) {
		case KEY_LEFT:
			blockMoveLeft();
			break;
		case KEY_RIGHT:
			blockMoveRight();
			break;
		case KEY_ROTATE:
			blockRotate();
			break;
		case KEY_DROP:
			blockDrop();
			break;
		case KEY_PAUSE: 
			{
				timePassed = timer_diff();
				renderPause();
				gameState = STATE_PAUSE;
			}
			break;
		case KEY_SHOW_NEXT:
			showNext = !showNext;
			if (showNext) { renderNextBlock(); }
			else { renderClearNextBlock(); }
			break;
		case KEY_SPEED_UP:
			level++;
			if (level > 9) { level = 9; }
			renderLevel();
			break;
		}
		if (timerDone()) {
			if (!blockMoveDown()) {
				long points = (21 + 3 * (level + 1)) - steps;
				score += points;
				if (score > 99999) { score = 99999; } // prevent overflow
				renderScore();
				steps = 0;
				playfieldUpdateBkgr();
				fullLines += playfieldCollapse();
				if (fullLines > 99) { fullLines = 99; } // prevent overflow
				renderFullLines();
				uint8_t computedLevel = (fullLines - 1) / 10;
				level = computedLevel > level ? computedLevel : level;
				renderLevel();
				if (!blockNext()) {
					renderGameOver();
					gameState = STATE_GAME_OVER;
					return true;
				}
			}
			else { steps++; }
			timer_reset(0);
		}
	} else if (gameState == STATE_PAUSE) {
		key = keyGet();
		if (key == KEY_RESTART || key == KEY_PAUSE) {
			renderClearPause(); 
			timer_reset(-timePassed);
			gameState = STATE_PLAY;
		}
	}
	else if (gameState == STATE_GAME_OVER) {
		key = keyGet();
		if (key != KEY_NONE) {
			if (key == KEY_RESTART) {
				stateInit();
				renderInit();
				playfieldInit();
				renderPlayfield();
				blockNext();
				gameState = STATE_PLAY;
			} else {
				renderGoodbye();
				return false;
			}
		}
	}
	return true;
}

// main game loop

int main() {
	avdc_init();
	gdp_cls();

	gameInit();
	while (gamePlay());

	avdc_done();
	return 0;
}