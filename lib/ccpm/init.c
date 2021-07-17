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
extern void _conio_init();
extern void _memory_init();
extern void  _stdio_init(); 

void _stdlib_init() {

    /* initialize memory management */
    _memory_init();

    /* initialize comamnd line args and cp/m defaults */
    _cpm_sysfunc_init();

    /* initialize file system */
    _fds_init();

#ifndef LEAN
    /* initialize stdio */
    _stdio_init();

    /* initialize conio */
    _conio_init();
#endif
}