/*
 * vprint.c
 *
 * Prints string with attr.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 23.06.2021   tstih
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

void write_at_display_pointer(uint8_t chr, uint8_t attr) {
    wait_avdc();  
    SCN2674_CHR=chr;
    wait_avdc();   
    SCN2674_AT=attr;
    wait_avdc();
    SCN2674_CMD=0xA2;
}

void wait(char *s) {
    printf("%s\n\r", s);
    while (!kbhit());
}

int main(int argc, char *argv[]) {

    signed char a;
    a=-1;

    /* Check command line arguments. */
    if (argc<3||argc>5) {
        printf("Usage: print <address> <text> [<attr.value>] [<repeat>]\n\r");
        printf("       address      ... vmem address\n\r");
        printf("       text         ... single word to print\n\r");
        printf("       attr.value   ... opt. attribute default=2\n\r");
        printf("       repeat       ... opt.repeat. default=1\n\r");
        exit(1);
    }

    /* Get the args */
    uint8_t addr=atoi(argv[1]);
    char* s=argv[2];
    uint8_t attr=2;
    if (argc==4) attr=atoi(argv[3]);
    uint8_t repeat=1;
    if (argc==5) repeat=atoi(argv[4]);

    /* Iterate */
    for(int count=0; count<repeat; count++) {
        for(int i=0;i<strlen(s);i++) {
            set_display_pointer(addr);
            write_at_display_pointer(s[0], attr);
            addr++;
        }
    }
    return 0;
}