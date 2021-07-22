#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <stdint.h>
#include <stdbool.h>

// types 

typedef uint8_t* p_uint8_t;

// state

typedef enum {
	STATE_PLAY,
	STATE_PAUSE,
	STATE_GAME_OVER
} state;

extern state game_state;
extern bool show_next;
extern bool show_text;
extern uint8_t level;
extern uint8_t steps;
extern long score;
extern uint8_t full_lines;
extern long time_passed;
extern uint16_t stats[7];

void state_init();

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
	KEY_HIDE_TEXT,
	KEY_EXIT,
	KEY_OTHER
} key;

key key_get();

// playfield

extern uint8_t playfield_bkgr[20][10];

void playfield_init();
void playfield_update_bkgr();
void playfield_clear_row(uint8_t row);
void playfield_move_row(uint8_t row_src, uint8_t row_dst);
uint8_t playfield_collapse();

// render

void render_init();
void render_playfield_row(uint8_t row);
void render_playfield();
void render_pause();
void render_clear_pause();
void render_next_block();
void render_clear_next_block();
void render_level();
void render_score();
void render_full_lines();
void render_game_over();
void render_block();
void render_clear_block();
void render_stats();
void render_show_text();
void render_hide_text();

// block

extern p_uint8_t block_shapes[][4][4];

extern int8_t block_type;
extern uint8_t block_rot;
extern int8_t block_pos_x;
extern int8_t block_pos_y;

extern int8_t block_type_next;

void block_init();
bool block_move_left();
bool block_move_right();
bool block_move_down();
bool block_rotate();
bool block_drop();
bool block_check(int8_t pos_x, int8_t pos_y, uint8_t rot);
bool block_next();

// timer

bool timer_done();

// game

void game_init();
bool game_play();

#endif