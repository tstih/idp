/*
 * rnd-xp.c
 *
 * Random experiments.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 25.05.2021   tstih
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main() {
    srand(clock());
    for(int i=0;i<20;i++)
        printf("%d\n\r", rand());
}