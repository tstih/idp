/*
 * std-test.c
 *
 * Standard C library tests.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 22.05.2021   tstih
 *
 */
#include <stdint.h>
#include <stdio.h>

extern uint8_t _port_read(uint8_t addr);
extern void _port_write(uint8_t addr, uint8_t value);

int main() {
    printf("set up values are\n\r");

    for(uint8_t port=0xa0; port<0xc0; port++) {
        uint8_t value=_port_read(port);
        if ((port-0xa0)%8==0) printf("\n\r");
        printf("%02x:%02x ", port, value);
    }

    printf("\n\r");
    return 0;
}