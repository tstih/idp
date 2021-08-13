/*
 * portdump.c
 *
 * Port dump.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 29.05.2021   tstih
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t _port_read(uint8_t addr) __naked {
    addr; /* compiler will think it's unreferenced, so reference it */
    __asm
        ld      hl,#2                   ; skip ret address
        add     hl,sp                   ; hl points to first arg
        ld      c,(hl)                  ; port to c
        in      a,(c)                   ; read value to a
        ld      h,#0                    ; hl=low
        ld      l,a                     ; return value to l
        ret
    __endasm;
}

int main(int argc, char *argv[]) {

    /* Check command line arguments. */
    if (argc!=3) {
        printf("Usage: portdump <from> <to>\n");
        printf("       from ... from port\n");
        printf("       to   ... to port\n");
        exit(1);
    }

    /* Get the address */
    int from_port=atoi(argv[1]), to_port=atoi(argv[2]);

    /* overflow? */
    if (from_port<0 || from_port>0xff) {
        printf("Overflow. From port must be from 0 to 255.\n");
        exit(2);
    }
    if (to_port<0 || to_port>0xff) {
        printf("Overflow. To port must be from 0 to 255.\n");
        exit(2);
    }

    /* To port must be larger */
    if (to_port<from_port) {
        printf("To port must be equal or larger than from port.\n");
        exit(2);
    }

    /* Dump it. */
    printf("\nPort dump 0x%02x - 0x%02x\n", from_port, to_port);

    /* Iterate */
    do {
        
        uint8_t value=_port_read(from_port);
        
        if (value>=32 && value<=128) /* Printable char? */
            printf("\n%02x:  %02x '%c'", from_port, value, value);
        else
            printf("\n%02x:  %02x", from_port, value);

        from_port++;

    } while (from_port<=to_port);

    /* Newline */
    printf("\n");
    return 0;
}