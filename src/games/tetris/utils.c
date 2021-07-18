#include "utils.h"

// timer

int16_t timer_start;
int16_t timer_offset;

extern uint8_t _bcd2bin(uint8_t);

int16_t timer() {
	while (true) {
		uint8_t seconds = CTC_SECONDS;
		uint8_t hundreds = CTC_HUNDREDS;
		uint8_t seconds_check = CTC_SECONDS;
		if (seconds == seconds_check) {
			return _bcd2bin(hundreds) + 100 * _bcd2bin(seconds);
		}
	}
}

void timer_reset(int16_t offset) {
	timer_start = timer();
	timer_offset = offset;
}

int16_t timer_diff() {
	int16_t now = timer();
	if (now >= timer_start) { return now - (timer_start + timer_offset); }
	return (now + 6000) - (timer_start + timer_offset); 
}

// GDP

void gdp_wait_ready() {
    uint8_t status = 0;
    while ((status & GDP_STATUS_READY) == 0) {
        status = GDP_STATUS;
    }
}

void gdp_cls() {
    gdp_wait_ready();
    GDP_CMD = GDP_CMD_CLS;
}

// other

uint8_t* itoa(long val, uint8_t* buffer) {
	if (val < 0) { *buffer = '-'; itoa(-val, buffer + 1); return buffer; }
	uint8_t* ptr = buffer;
	do {
		*ptr = '0' + (val % 10);
		ptr++;
		val /= 10;
	} while (val != 0);
	*ptr = 0;
	ptr--;
	// reverse
	uint8_t* ptr2 = buffer;
	while (ptr > ptr2) {
		uint8_t t = *ptr;
		*ptr = *ptr2;
		*ptr2 = t;
		ptr2++;
		ptr--;
	}
	return buffer;
}