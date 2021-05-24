/*
 * init.c
 *
 * standard C library initialization
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.04.2021   tstih
 *
 */
extern void _cpm_sysfunc_init();
extern void _fds_init();

void _stdlib_init() {

    /* initialize comamnd line args and cp/m defaults */
    _cpm_sysfunc_init();

    /* initialize file system */
    _fds_init();

    /* initialize conio */
    _conio_init();
}