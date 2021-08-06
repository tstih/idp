/*
 * conio.c
 *
 * parts of conio.h, implemented by libpartner
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.08.2021   tstih
 *
 */
#include <partner/conio.h>

int kbhit(void) {
    return bdos(C_RAWIO,0xff);
}