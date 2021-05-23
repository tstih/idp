/*
 * rand.c
 *
 * idp standard C library rand functions. 
 * 
 * NOTES:
 *  the idp standard library is based on the cpmlibc
 *  https://github.com/dmo9000/cpmlibc
 * 
 * TODO:
 *  use idp real time clock for seed
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 28.04.2021   tstih
 *
 */

unsigned long int next = 1;

int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}