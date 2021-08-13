/*
 * memdump.c
 *
 * Memory dump.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 02.05.2021   tstih
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BYTES_PER_ROW 16

void pad(char *s, int len, char c) {
    char *p=(char *)(s+strlen(s));
    while(len) {
        *p=c; p++; len--;
    }
    *p=0;
}

int main(int argc, char *argv[]) {

    /* Check command line arguments. */
    if (argc!=3) {
        printf("Usage: memdump <addr> <size>\n");
        printf("       addr ... decimal address\n");
        printf("       size ... number of bytes to dump (max 1024)\n");
        exit(1);
    }

    /* Get the address */
    unsigned int addr=atoi(argv[1]), size=atoi(argv[2]), eaddr=addr+size;

    /* size==0 */
    if (size==0) {
        printf("Size can't be zero.\n");
        exit(2);
    }

    if (size>1024) {
        printf("This would take too long to print. Max size is 1024.\n");
        exit(3);
    }

    /* overflow? */
    if (eaddr<addr) {
        printf("Overflow. Your parameters pass known memory space.\n");
        exit(4);
    }

    /* Pointer to memomry location. */
    unsigned char *p;
    /* Ascii string */
    char ascii[BYTES_PER_ROW + 1]; memset(ascii,0,BYTES_PER_ROW + 1);
    /* Bytes string */
    char bytes[3 * BYTES_PER_ROW + 1 ]; memset(bytes,0,3 * BYTES_PER_ROW + 1);

    /* Dump it. */
    printf("\nMemory dump 0x%04x - 0x%04x\n", addr, eaddr);

    /* Iterate */
    int missing=0;
    do {
        /* Print memory address. */
        printf("\n%04x  ",addr);

        /* Calculate row end address. */
        int erow = addr + BYTES_PER_ROW;
        if (erow > eaddr) {
            /* Missing bytes from last row */
            missing = erow - eaddr;
            /* Send erow */
            erow = eaddr;
        }

        int i=0;
        while(addr < erow) {
            p=(unsigned char*)addr;
            /* Add byte */
            sprintf((bytes + 3 * i),"%02x ", *p);

            /* And char */
            if (*p>=32 && *p<=128)
                ascii[i]=*p;
            else
                ascii[i]='.';

            i++;
            addr++;
        }

        /* Missing chars? */
        if (missing > 0)
            pad(bytes, 3 * missing, ' ');

        /* Print row */
        printf("%s  %s", bytes, ascii);

        /* And reset strings... */
        memset(ascii,0,BYTES_PER_ROW + 1);
        memset(bytes,0,3 * BYTES_PER_ROW + 1);

    } while (addr<eaddr);

    /* Newline */
    printf("\n");
    return 0;
}