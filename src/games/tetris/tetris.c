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

#include <conio.h>
#include "tetris.h"
#include "avdc.h"

// random.c
int rand(void);
void srand(unsigned int seed);

// print.c
int sprintf(char *buf, char *fmt, ...);

// string str_buffer

byte str_buffer[133];

// state

void stateInit() {
	gameState = STATE_PLAY;
	showNext = FALSE;
	level = 0;
	steps = 0;
	score = 0;
	fullLines = 0;
	timer = clock();
	timeLeft = 0;
	srand(clock());
	for (byte i = 0; i < 7; i++) { 
		stats[i] = 0; 
	}
	for (byte i = 0; i < 24; i++) {
		row_addr[i] = avdc_get_pointer(i, 0);
	}
	avdc_done();
}

// key

key getKey() {
	char key;
	if (!(key = kbhit())) { return KEY_NONE; }
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

void playfieldInit() {
	for (byte i = 0; i < 20; i++) {
		for (byte j = 0; j < 10; j++) {
			playfieldBkgr[i][j] = '0';
		}
	}
}

void playfieldUpdateBkgr() {
	for (byte r = 0; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			for (byte c = 0; c < 4; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					if (blockShapes[blockType][blockRot][r][c] != '0') {
						playfieldBkgr[blockPosY + r][blockPosX + c] = blockShapes[blockType][blockRot][r][c];
					}
				}
			}
		}
	}
}

byte playfieldCollapse() {
	//int[, ] tmp = new int[20, 10];
	//int yTmp = 19;
	//bool render = false;
	//int fullLines = 0;
	//for (int y = 19; y >= 0; y--)
	//{
	//	bool fullLine = true;
	//	for (int x = 0; x < 10; x++)
	//	{
	//		if (mGrid[y, x] == 0) { fullLine = false; break; }
	//	}
	//	if (fullLine)
	//	{
	//		Array.Clear(mGrid, y * 10, 10);
	//		Renderer.RenderRow(y);
	//		render = true;
	//		fullLines++;
	//	}
	//	else
	//	{
	//		Array.Copy(mGrid, y * 10, tmp, yTmp * 10, 10);
	//		yTmp--;
	//	}
	//}
	//if (render)
	//{
	//	Array.Copy(tmp, mGrid, tmp.Length);
	//	Renderer.RenderPlayfield();
	//}
	//return fullLines;

	return 0;
}

// render

void renderInit() {
	avdc_set_cursor_write_str(0, 0, "This is renderInit", NULL);
	avdc_done();
}

void renderPlayfield() {
	for (byte i = 0; i < 20; i++) {
		avdc_set_cursor(i, 30);
		for (int j = 0; j < 10; j++) {
			avdc_write_at_cursor(playfieldBkgr[i][j], DEFAULT_ATTR);
		}
	}
	avdc_done();
}

void renderPause() {
	avdc_set_cursor_write_str(1, 0, "PAUSED", NULL);
	avdc_done();
}

void renderClearPause() {
	avdc_set_cursor_write_str(1, 0, "      ", NULL);
	avdc_done();
}

void renderNextBlock() {
	for (byte r = 0; r < 4; r++) {
		avdc_set_cursor(10 + r, 4);
		for (byte c = 0; c < 4; c++) {
			avdc_write_at_cursor(
				blockShapes[nextBlockType][0][r][c] == '0' ? ' ' : blockShapes[nextBlockType][blockRot][r][c],
				DEFAULT_ATTR
			);
		}
	}
	avdc_done();
}

void renderClearNextBlock() {
	for (byte r = 0; r < 4; r++) {
		avdc_set_cursor_write_str(10 + r, 4, "    ", NULL);
	}
	avdc_done();
}

void renderLevel() {
	sprintf(str_buffer, "Level: %d", level);
	avdc_set_cursor_write_str(2, 0, str_buffer, NULL);
	avdc_done();
}

void renderScore() {
	sprintf(str_buffer, "Score: %d", score);
	avdc_set_cursor_write_str(3, 0, str_buffer, NULL);
	avdc_done();
}

void renderFullLines() {
	sprintf(str_buffer, "Full lines: %d", fullLines);
	avdc_set_cursor_write_str(4, 0, str_buffer, NULL);
	avdc_done();
}

void renderGameOver() {
	avdc_set_cursor_write_str(5, 0, "GAME OVER", NULL);
	avdc_done();
}

void renderGoodbye() {
	avdc_set_cursor_write_str(5, 0, "GOODBYE !", NULL);
	avdc_done();
}

void renderClearBlock() {
	for (byte r = 0; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			avdc_set_cursor(blockPosY + r, 30 + (blockPosX < 0 ? 0 : blockPosX));
			for (byte c = 0; c < 4; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					avdc_write_at_cursor(playfieldBkgr[blockPosY + r][blockPosX + c], DEFAULT_ATTR);
				}
			}
		}
	}
	avdc_done();
}

void renderBlock() {
	for (sbyte r = -1; r < 4; r++) {
		if (blockPosY + r >= 0 && blockPosY + r < 20) {
			avdc_set_cursor(blockPosY + r, 30 + ((blockPosX - 1) < 0 ? 0 : (blockPosX - 1)));
			for (sbyte c = -1; c < 5; c++) {
				if (blockPosX + c >= 0 && blockPosX + c < 10) {
					if (c < 0 || c > 3 || r < 0 || r > 3) {
						avdc_write_at_cursor(playfieldBkgr[blockPosY + r][blockPosX + c], DEFAULT_ATTR);
					} else {
						avdc_write_at_cursor(
							blockShapes[blockType][blockRot][r][c] == '0' ? playfieldBkgr[blockPosY + r][blockPosX + c] : blockShapes[blockType][blockRot][r][c],
							DEFAULT_ATTR
						);
					}
				}
			}
		}
	}
	avdc_done();
}

void renderStats() {
	for (byte i = 0; i < 7; i++) {
		sprintf(str_buffer, "%d", stats[i]);
		avdc_set_cursor_write_str(6 + i, 0, str_buffer, NULL);
	}
	avdc_done();
}

// block

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
		return TRUE;
	}
	return FALSE;
}

bool blockMoveRight() {
	if (blockCheck(blockPosX + 1, blockPosY, blockRot)) {
		blockPosX++;
		renderBlock();
		return TRUE;
	}
	return FALSE;
}

bool blockCheck(sbyte posX, sbyte posY, byte rot) {
	for (sbyte blockY = 0, gridY = posY; blockY < 4; blockY++, gridY++) {
		for (sbyte blockX = 0, gridX = posX; blockX < 4; blockX++, gridX++) {
			if (blockShapes[blockType][rot][blockY][blockX] != '0' && 
				(gridX < 0 || gridY < 0 || gridX >= 10 || gridY >= 20 || playfieldBkgr[gridY][gridX] != '0')) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

bool blockMoveDown() {
	if (blockCheck(blockPosX, blockPosY + 1, blockRot)) {
		blockPosY++;
		renderBlock();
		return TRUE;
	}
	return FALSE;
}

bool blockRotate() {
	byte rot = (blockRot + 1) % 4;
	if (blockCheck(blockPosX, blockPosY, rot)) {
		blockRot = rot;
		renderBlock();
		return TRUE;
	}
	return FALSE;
}

bool blockDrop() {
	bool success = blockCheck(blockPosX, blockPosY + 1, blockRot);
	if (success) {
		renderClearBlock();
		while (blockCheck(blockPosX, blockPosY + 1, blockRot)) { blockPosY++; }
		playfieldUpdateBkgr();
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

void timerReset() {
	timer = clock();
}

bool timerDone() {
	clock_t now = clock();
	long span = (10 - level) * 50;
	return now - timer >= span;
}

// init

void init() {
	stateInit();
	playfieldInit();
	blockInit();

	renderInit();
	renderPlayfield();
	blockNext();
	//timerReset(); // redundant?
}

// game logic

bool play() {
	clock_t now;
	key key;
	if (gameState == STATE_PLAY) {
		key = getKey();
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
				now = clock();
				timeLeft = now - timer;
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
				fullLines += playfieldCollapse();
				if (fullLines > 99) { fullLines = 99; } // prevent overflow
				renderFullLines();
				byte computedLevel = (fullLines - 1) / 10;
				level = computedLevel > level ? computedLevel : level;
				renderLevel();
				playfieldUpdateBkgr();
				if (!blockNext()) {
					renderGameOver();
					gameState = STATE_GAME_OVER;
					return TRUE;
				}
			}
			else { steps++; }
			timerReset();
		}
	} else if (gameState == STATE_PAUSE) {
		key = getKey();
		if (key == KEY_RESTART || key == KEY_PAUSE) {
			renderClearPause();
			now = clock(); 
			timer = now - timeLeft;
			gameState = STATE_PLAY;
		}
	}
	else if (gameState == STATE_GAME_OVER) {
		key = getKey();
		if (key != KEY_NONE) {
			if (key == KEY_RESTART) {
				stateInit();
				renderInit();
				playfieldInit();
				renderPlayfield();
				blockNext();
				timerReset();
				gameState = STATE_PLAY;
			} else {
				renderGoodbye();
				return FALSE;
			}
		}
	}
	return TRUE;
}

// main game loop

int main() {

	// init();
	// while (play());
	avdc_cursor_off();
	for (int j = 0; j < 1000; j++) {
		for (byte i = 0; i < 24; i++) {
			row_addr[i] = avdc_get_pointer(i, 0);
		}
	}

	for (int j = 0; j < 1000; j++)
	{
		avdc_set_cursor_write_str(0, 0, "This is renderInit", NULL);
	}

	avdc_done();
	avdc_cursor_on();

	// sbyte a = -1;
	// sbyte b = -1;
	// sprintf(str_buffer, "%d", a);
	// avdc_write_str_at_pointer(avdc_set_cursor(3, 1), str_buffer, NULL);
	// sprintf(str_buffer, "%d", b);
	// avdc_write_str_at_pointer(avdc_set_cursor(0, 0), str_buffer, NULL);
	// avdc_write_str_at_pointer(avdc_set_cursor(2, 0), str_buffer, NULL);
	// char k;
	// while (!(k = kbhit()));

	// sprintf(str_buffer, "%c", k);
	// avdc_write_str_at_pointer(avdc_set_cursor(4, 0), str_buffer, NULL);

	// clock_t t;
	// for (int i = 0; i < 5; i++)
	// {
	// 	t = clock();
	// 	sprintf(str_buffer, "%d", t);
	// 	avdc_write_str_at_pointer(avdc_set_cursor(5 + i, 0), str_buffer, NULL);	
	// }

	// srand(clock());
	// for (int i = 0; i < 5; i++)
	// {
	// 	byte r = rand() % 7;
	// 	sprintf(str_buffer, "%d", r);
	// 	avdc_write_str_at_pointer(avdc_set_cursor(10 + i, 0), str_buffer, NULL);	
	// }


	return 0;
}