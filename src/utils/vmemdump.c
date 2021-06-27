/*
 * vmemdump.c
 *
 * Video memory dump.
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
    SCN2674_INIT=val.u8[0];
    SCN2674_INIT=val.u8[1];
}

void read_at_display_pointer(uint8_t *chr, uint8_t *attr) {
    wait_avdc();   
    SCN2674_CMD=0xA4;
    wait_avdc();  
    *chr=SCN2674_CHR;
    wait_avdc();
    *attr=SCN2674_AT;
}

int main(int argc, char *argv[]) {

    /* Check command line arguments. */
    if (argc!=3) {
        printf("Usage: vmemdump <from> <to>\n\r");
        printf("       from ... from address\n\r");
        printf("       to   ... to address\n\r");
        exit(1);
    }

    /* Get the address */
    uint16_t from_addr=atoi(argv[1]), to_addr=atoi(argv[2]);

    /* overflow? */
    if (from_addr<0 || from_addr>0x3fff) {
        printf("Overflow. From address must be from 0 to 16383.\n\r");
        exit(2);
    }
    if (to_addr<0 || to_addr>0x3fff) {
        printf("Overflow. To address must be from 0 to 16383.\n\r");
        exit(2);
    }

    /* To port must be larger */
    if (to_addr<from_addr) {
        printf("To address must be equal or larger than from address.\n\r");
        exit(2);
    }

    /* Dump it. */
    printf("\n\rVideo memory dump 0x%04x - 0x%04x\n\r", from_addr, to_addr);

    /* Iterate */
    do {
        
        uint8_t chr, attr;
        set_display_pointer(from_addr);
        read_at_display_pointer(&chr,&attr);
        
        if (chr>=32 && chr<=128) 
            printf("\n\r%04x:  %02x '%c' (%02x)", from_addr, chr, chr, attr);
        else
            printf("\n\r%04x:  %02x (%02x)", from_addr, chr, attr);
        
        from_addr++;

    } while (from_addr<=to_addr);

    /* Newline */
    printf("\n\r");
    return 0;
}