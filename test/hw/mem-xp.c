/*
 * mem-xp.c
 *
 * Test malloc.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 25.05.2021   tstih
 *
 */
#include <stdio.h>
#include <stdlib.h>

extern int heap;

void main() {
    
    /* Heap addr? */
    printf("Heap is %u\n\r", heap);

    /* Allocate kilobyte. */
    void *p=malloc(1024);
    printf("p is %u\n\r", p);

    /* Allocate 256 bytes */
    void *q=malloc(256);
    printf("q is %u\n\r", q);

    /* Release kilobyte. Next alloc will be at the same address. */
    printf("Releasing p\n\r");
    free(p);

    /* Same amount */
    void *r=malloc(512);
    printf("r is %u\n\r", r);

    void *s=malloc(256);
    printf("s is %u\n\r", s);

}