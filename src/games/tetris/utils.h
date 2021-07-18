#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdbool.h>

// timer

__sfr __at 0xa1 CTC_HUNDREDS; // R: clock, hundreds 
__sfr __at 0xa2 CTC_SECONDS;  // R: clock, seconds

extern int16_t timer_start;
extern int16_t timer_offset;

int16_t timer();

void timer_reset(int16_t offset);
int16_t timer_diff();

// GDP

#define GDP_STATUS_READY 0x04
#define GDP_CMD_CLS      0x04 

__sfr __at 0x20 GDP_STATUS; // R: status (ready)
__sfr __at 0x20 GDP_CMD;    // W: command 

void gdp_wait_ready();
void gdp_cls();

// other

// random.c
int rand(void);
void srand(unsigned int seed);

// cpm_sysfunc.c
char cpm_getchar_nonblock(void);

uint8_t *itoa(long val, uint8_t *buffer);

#endif