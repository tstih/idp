/*
 * vmemset.c
 *
 * Set char and attribute at video address(es).
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 19.06.2021   tstih
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

__sfr __at 0x39 SCN2674_CMD;            /* W: command */
__sfr __at 0x39 SCN2674_STS;            /* R: status */
__sfr __at 0x38 SCN2674_INIT;           /* W: write to current IR */
#define SCN2674_STS_RDY 0x20            /* Ready status bit. */
__sfr __at 0x34 SCN2674_CHR;            /* R/W: character register */
__sfr __at 0x35 SCN2674_AT;             /* R/W: attribute register */

typedef union
{
    uint16_t u16;
    uint8_t u8[2];
} U16_U8;

void wait_avdc() {
    uint8_t ready=0;
    while((ready&SCN2674_STS_RDY)==0) {
        ready=SCN2674_STS;
    }
}

void set_display_pointer(uint16_t address) {
    U16_U8 val;
    val.u16=address;
    wait_avdc();                        
    /* write to IR 10, 11 */
    SCN2674_CMD=0x1A;
    wait_avdc();     
    SCN2674_INIT=val.u8[0];
    wait_avdc();     
    SCN2674_INIT=val.u8[1];
}

void write_char_at_display_pointer(uint8_t chr) {
    wait_avdc();  
    SCN2674_CHR=chr;
    wait_avdc();   
    SCN2674_CMD=0xA2;
}

int main(int argc, char *argv[]) {

    /* Check command line arguments. */
    if (argc<3||argc>5) {
        printf("Usage: vmemset <start> <first ascii> [<last ascii>] [<repeat>]\n\r");
        printf("       start        ... the address\n\r");
        printf("       first ascii  ... first value to write\n\r");
        printf("       last ascii   ... last value to write, default=first ascii\n\r");
        printf("       repeat       ... repeat the pattern, default=1\n\r");
        exit(1);
    }

    /* Get the args */
    uint16_t addr=atoi(argv[1]);
    uint8_t first=atoi(argv[2]);
    uint8_t last=first;
    if (argc==3) last=atoi(argv[3]);
    uint8_t repeat=1;
    if (argc==4) repeat=atoi(argv[4]);

    /* Iterate */
    while (repeat) {
        
        /* Repeat pattern from first to inclusive last */
        for (uint8_t ch=first; ch<=last; ch++) {
            set_display_pointer(addr);
            write_char_at_display_pointer(ch);
            addr++; /* Next address for each char. */
        }  
        repeat--;
    } 

    return 0;
}