#include "utils.h"

int16_t _timer_start;
int16_t _timer_offset;

extern uint8_t _bcd2bin(uint8_t);

int16_t _timer() {
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
	_timer_start = _timer();
	_timer_offset = offset;
}

int16_t timer_diff() {
	int16_t now = _timer();
	if (now >= _timer_start) { return now - (_timer_start + _timer_offset); }
	return (now + 6000) - (_timer_start + _timer_offset); 
}

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