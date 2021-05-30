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

extern uint8_t _port_read(uint8_t addr);

int main(int argc, char *argv[]) {

    /* Check command line arguments. */
    if (argc!=3) {
        printf("Usage: portdump <from> <to>\n\r");
        printf("       from ... from port\n\r");
        printf("       to   ... to port\n\r");
        exit(1);
    }

    /* Get the address */
    int from_port=atoi(argv[1]), to_port=atoi(argv[2]);

    /* overflow? */
    if (from_port<0 || from_port>0xff) {
        printf("Overflow. From port must be from 0 to 255.\n\r");
        exit(2);
    }
    if (to_port<0 || to_port>0xff) {
        printf("Overflow. To port must be from 0 to 255.\n\r");
        exit(2);
    }

    /* To port must be larger */
    if (to_port<from_port) {
        printf("To port must be equal or larger than from port.\n\r");
        exit(2);
    }

    /* Dump it. */
    printf("\n\rPort dump 0x%02x - 0x%02x\n\r", from_port, to_port);

    /* Iterate */
    do {
        
        uint8_t value=_port_read(from_port);
        
        if (value>=32 && value<=128) /* Printable char? */
            printf("\n\r%02x:  %02x '%c'", from_port, value, value);
        else
            printf("\n\r%02x:  %02x", from_port, value);

        from_port++;

    } while (from_port<=to_port);

    /* Newline */
    printf("\n\r");
    return 0;
}