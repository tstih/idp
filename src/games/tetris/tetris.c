#include <stdlib.h>
#include <string.h>
#include "tetris.h"
#include "avdc.h"
#include "utils.h"

// string buffer

uint8_t str_buffer[133];

// state

state game_state;
bool show_next;
bool show_text;
uint8_t level;
uint8_t steps;
long score;
uint8_t full_lines;
long time_passed;
uint16_t stats[7];

void state_init() {
	game_state = STATE_PLAY;
	show_next = false;
	show_text = true;
	level = 0;
	steps = 0;
	score = 0;
	full_lines = 0;
	timer_reset(0); 
	time_passed = 0;
	srand(timer());
	for (uint8_t i = 0; i < 7; i++) { 
		stats[i] = 0; 
	}
}

// key

key key_get() {
	char key;
	if (!(key = cpm_getchar_nonblock())) { return KEY_NONE; }
	switch (key) {
	case 'a':
	case 'A':
	case '7':
		return KEY_LEFT;
	case 'd':
	case 'D':
	case '9':
		return KEY_RIGHT;
	case 'w':
	case 'W':
	case '8':
		return KEY_ROTATE;
	case 's':
	case 'S':
	case '5':
	case ' ': 
		return KEY_DROP;
	case 'n':
	case 'N':
		return KEY_RESTART;
	case 'p':
	case 'P':
		return KEY_PAUSE;
	case '1':
		return KEY_SHOW_NEXT;
	case '4':
		return KEY_SPEED_UP;
	case '0': 
		return KEY_HIDE_TEXT;
	case 'x':
	case 'X':
		return KEY_EXIT;
	default:
		return KEY_OTHER;
	}
}

// playfield

uint8_t playfield_bkgr[20][10];

void playfield_init() {
	for (uint8_t i = 0; i < 20; i++) {
		playfield_clear_row(i);
	}
}

void playfield_update_bkgr() {
	for (uint8_t r = 0; r < 4; r++) {
		if (block_pos_y + r >= 0 && block_pos_y + r < 20) {
			for (uint8_t c = 0; c < 4; c++) {
				if (block_pos_x + c >= 0 && block_pos_x + c < 10) {
					if (block_shapes[block_type][block_rot][r][c] != '0') {
						playfield_bkgr[block_pos_y + r][block_pos_x + c] = block_shapes[block_type][block_rot][r][c];
					}
				}
			}
		}
	}
}

void playfield_clear_row(uint8_t row) {
	for (uint8_t j = 0; j < 10; j++) {
		playfield_bkgr[row][j] = '0';
	}
}

void playfield_move_row(uint8_t row_src, uint8_t row_dst) {
	for (uint8_t j = 0; j < 10; j++) {
		playfield_bkgr[row_dst][j] = playfield_bkgr[row_src][j];
	}
	playfield_clear_row(row_src);
}

uint8_t playfield_collapse() {
	uint8_t lines = 0;
	for (int8_t y = 19; y >= 0; y--) {
		bool full_line = true;
		for (uint8_t x = 0; x < 10; x++) {
			if (playfield_bkgr[y][x] == '0') { full_line = false; break; }
		}
		if (full_line) {
			lines++;
			// clear line y
			playfield_clear_row(y);
			render_playfield_row(y);
			// shift lines
			for (int8_t yy = y - 1; yy >= 0; yy--) {
				// copy line from yy to yy + 1
				playfield_move_row(yy, yy + 1);
				render_playfield_row(yy);
				render_playfield_row(yy + 1);
			}
			kbd_beep(FALSE);
			y++;
		}
	}
	return lines;
}

// render

// Č 132 84 č 148 94
// Š 140 8C š 156 9C
// Ž 142 8E ž 158 9E

const uint8_t V_POS = 2; 
const uint8_t H_POS = 3; 

void render_init() {
	avdc_write_str_at_cursor_pos(V_POS, H_POS,     "POLNIH VRSTIC:  0", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 1, H_POS, "STOPNJA:        0", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 2, H_POS, "  TO\x84KE:     0", NULL);
	for (uint8_t i = 0; i < 20; i++) {
		avdc_write_str_at_cursor_pos(V_POS + i, H_POS + 25, "<! . . . . . . . . . .!>", NULL);
	}
	avdc_write_str_at_cursor_pos(V_POS + 20, H_POS + 25, "<!====================!>", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 21, H_POS + 27, "\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/", NULL);
	avdc_write_str_at_cursor_pos(V_POS,     H_POS + 55, "7: LEVO    9: DESNO", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 1, H_POS + 55, "     8: OBRNI", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 2, H_POS + 55, "4: POJA\x84""AJ 5: ZABIJ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 3, H_POS + 55, "1: POKA\x8EI NASLEDNJO", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 4, H_POS + 55, "0: SKRIJ  TA  TEKST", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 5, H_POS + 55, "PRESLEDNICA - ZABIJ", NULL);
	render_clear_next_block();
	// clear "game over"
	avdc_write_str_at_cursor_pos(V_POS + 18, H_POS, "            ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 19, H_POS, "            ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 20, H_POS, "        ", NULL);
}

void render_playfield_row(uint8_t row) {
	avdc_set_cursor(V_POS + row, H_POS + 27);
	for (uint8_t j = 0; j < 10; j++) {
		avdc_write_str_at_cursor(
			playfield_bkgr[row][j] == '0' ? " ." : "[]", 
			AVDC_DEFAULT_ATTR
		);	
	}
}

void render_pause() {
	avdc_write_str_at_cursor_pos(V_POS + 18, H_POS, "PAVZA ZA KAVO", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 19, H_POS, "P/N: NADALJUJ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 20, H_POS, "X: IZHOD", NULL);
}

void render_clear_pause() {
	avdc_write_str_at_cursor_pos(V_POS + 18, H_POS, "             ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 19, H_POS, "             ", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 20, H_POS, "        ", NULL);
}

void render_next_block() {
	for (uint8_t r = 0; r < 4; r++) {
		avdc_set_cursor(V_POS + 9 + r, H_POS + 16);
		for (uint8_t c = 0; c < 4; c++) {
			avdc_write_str_at_cursor(
				block_shapes[block_type_next][0][r][c] == '0' ? "  " : "[]",
				AVDC_DEFAULT_ATTR
			);
		}
	}
}

void render_clear_next_block() {
	for (uint8_t r = 0; r < 4; r++) {
		avdc_write_str_at_cursor_pos(V_POS + 9 + r, H_POS + 16, "        ", NULL);
	}
}

void render_level() {
	itoa(level, str_buffer);
	avdc_write_str_at_cursor_pos(V_POS + 1, H_POS + 17 - strlen(str_buffer), str_buffer, NULL);
}

void render_score() {
	itoa(score, str_buffer);
	avdc_write_str_at_cursor_pos(V_POS + 2, H_POS + 14 - strlen(str_buffer), str_buffer, NULL);
}

void render_full_lines() {
	itoa(full_lines, str_buffer);
	avdc_write_str_at_cursor_pos(V_POS, H_POS + 17 - strlen(str_buffer), str_buffer, NULL);
}

void render_game_over() {
	avdc_write_str_at_cursor_pos(V_POS + 18, H_POS, "IGRA KON\x84""ANA", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 19, H_POS, "N: NOVA IGRA", NULL);
	avdc_write_str_at_cursor_pos(V_POS + 20, H_POS, "X: IZHOD", NULL);
}

void render_clear_block() {
	for (uint8_t r = 0; r < 4; r++) {
		if (block_pos_y + r >= 0 && block_pos_y + r < 20) {
			avdc_set_cursor(V_POS + block_pos_y + r, H_POS + 27 + 2 * ((block_pos_x < 0 ? 0 : block_pos_x)));
			for (uint8_t c = 0; c < 4; c++) {
				if (block_pos_x + c >= 0 && block_pos_x + c < 10) {
					avdc_write_str_at_cursor(playfield_bkgr[block_pos_y + r][block_pos_x + c] == '0' ? " ." : "[]", AVDC_DEFAULT_ATTR);
				}
			}
		}
	}
}

void render_block() {
	for (int8_t r = -1; r < 4; r++) {
		if (block_pos_y + r >= 0 && block_pos_y + r < 20) {
			avdc_set_cursor(V_POS + block_pos_y + r, H_POS + 27 + 2 * (((block_pos_x - 1) < 0 ? 0 : (block_pos_x - 1))));
			for (int8_t c = -1; c < 5; c++) {
				if (block_pos_x + c >= 0 && block_pos_x + c < 10) {
					if (c < 0 || c > 3 || r < 0 || r > 3) {
						avdc_write_str_at_cursor(playfield_bkgr[block_pos_y + r][block_pos_x + c] == '0' ? " ." : "[]", AVDC_DEFAULT_ATTR);
					} else {
						avdc_write_str_at_cursor(
							block_shapes[block_type][block_rot][r][c] == '0' 
								? (playfield_bkgr[block_pos_y + r][block_pos_x + c] == '0' ? " ." : "[]")
								: "[]",
							AVDC_DEFAULT_ATTR
						);
					}
				}
			}
		}
	}
}

void render_stats() {
	//for (uint8_t i = 0; i < 7; i++) {
	//	avdc_write_str_at_cursor_pos(6 + i, 0, itoa(stats[i], str_buffer), NULL);
	//}
}

void render_show_text() {
	avdc_write_str_at_cursor_pos(V_POS + 4, H_POS + 55, "0: SKRIJ  TA  TEKST", NULL);
}

void render_hide_text() {
	avdc_write_str_at_cursor_pos(V_POS + 4, H_POS + 55, "                   ", NULL);
}

// block

p_uint8_t block_shapes[][4][4] = {
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

int8_t block_type;
uint8_t block_rot;
int8_t block_pos_x;
int8_t block_pos_y;

int8_t block_type_next;

void block_init() {
	block_type = -1;
	block_rot = 0;
	block_pos_x = 3;
	block_pos_y = -1;

	block_type_next = -1;
}

bool block_move_left() {
	if (block_check(block_pos_x - 1, block_pos_y, block_rot)) {
		block_pos_x--;
		render_block();
		return true;
	}
	return false;
}

bool block_move_right() {
	if (block_check(block_pos_x + 1, block_pos_y, block_rot)) {
		block_pos_x++;
		render_block();
		return true;
	}
	return false;
}

bool block_check(int8_t pos_x, int8_t pos_y, uint8_t rot) {
	for (int8_t block_y = 0, grid_y = pos_y; block_y < 4; block_y++, grid_y++) {
		for (int8_t block_x = 0, grid_x = pos_x; block_x < 4; block_x++, grid_x++) {
			if (block_shapes[block_type][rot][block_y][block_x] != '0' && 
				(grid_x < 0 || grid_y < 0 || grid_x >= 10 || grid_y >= 20 || playfield_bkgr[grid_y][grid_x] != '0')) {
				return false;
			}
		}
	}
	return true;
}

bool block_move_down() {
	if (block_check(block_pos_x, block_pos_y + 1, block_rot)) {
		block_pos_y++;
		render_block();
		return true;
	}
	return false;
}

bool block_rotate() {
	uint8_t rot = (block_rot + 1) % 4;
	if (block_check(block_pos_x, block_pos_y, rot)) {
		block_rot = rot;
		render_block();
		return true;
	}
	return false;
}

bool block_drop() {
	bool success = block_check(block_pos_x, block_pos_y + 1, block_rot);
	if (success) {
		render_clear_block();
		while (block_check(block_pos_x, block_pos_y + 1, block_rot)) { block_pos_y++; }
		render_block();
	}
	return success;
}

bool block_next() {
	if (block_type_next == -1) {
		block_type_next = rand() % 7;
	}
	block_type = block_type_next;
	block_rot = 0;
	block_pos_x = 3;
	block_pos_y = -1;
	stats[block_type]++;
	if (stats[block_type] > 1428) { stats[block_type] = 1428; } // prevent overflow (also of the overall sum)
	render_stats();
	block_type_next = rand() % 7;
	bool success = block_check(block_pos_x, block_pos_y, block_rot);
	render_block();
	if (show_next) { render_next_block(); }
	return success;
}

// timer

bool timer_done() {
	long span = (10 - level) * 5;
	return timer_diff() >= span;
}

// game

void game_init() {
	state_init();
	playfield_init();
	block_init();
	render_init();
	block_next();
}

bool game_play() {
	key key = key_get();
	if (key == KEY_HIDE_TEXT) {
		show_text = !show_text;
		if (show_text) { render_show_text(); } 
		else { render_hide_text(); }
	}
	if (game_state == STATE_PLAY) {
		switch (key) {
		case KEY_LEFT:
			block_move_left();
			break;
		case KEY_RIGHT:
			block_move_right();
			break;
		case KEY_ROTATE:
			block_rotate();
			break;
		case KEY_DROP:
			block_drop();
			break;
		case KEY_PAUSE: 
			{
				time_passed = timer_diff();
				render_pause();
				game_state = STATE_PAUSE;
			}
			break;
		case KEY_SHOW_NEXT:
			show_next = !show_next;
			if (show_next) { render_next_block(); }
			else { render_clear_next_block(); }
			break;
		case KEY_SPEED_UP:
			level++;
			if (level > 9) { level = 9; }
			render_level();
			break;
		}
		if (timer_done()) {
			if (!block_move_down()) {
				long points = (21 + 3 * (level + 1)) - steps;
				score += points;
				if (score > 99999) { score = 99999; } // prevent overflow
				render_score();
				steps = 0;
				playfield_update_bkgr();
				full_lines += playfield_collapse();
				if (full_lines > 99) { full_lines = 99; } // prevent overflow
				render_full_lines();
				uint8_t computed_level = (full_lines - 1) / 10;
				if (computed_level > level) {
					level = computed_level;
					kbd_beep(TRUE);
				}
				render_level();
				if (!block_next()) {
					render_game_over();
					game_state = STATE_GAME_OVER;
					return true;
				}
			}
			else { steps++; }
			timer_reset(0);
		}
	} else if (game_state == STATE_PAUSE) {
		if (key == KEY_RESTART || key == KEY_PAUSE) {
			render_clear_pause(); 
			timer_reset(-time_passed);
			game_state = STATE_PLAY;
		} else if (key == KEY_EXIT) {
			return false;
		}
	}
	else if (game_state == STATE_GAME_OVER) {
		if (key == KEY_RESTART) {
			game_init();
		} else if (key == KEY_EXIT) {
			return false;
		}
	}
	return true;
}

// main game loop

int main() {
	avdc_init();
	gdp_cls();

	game_init();
	while (game_play());

	avdc_done();
	return 0;
}