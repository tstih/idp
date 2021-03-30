/*
 * yos.c
 * y operating system. for now this is just a library, providing fake syscalls on 
 * top of CP/M.
 *
 * tomaz stih thu mar 25 2021
 */

#include "memory.h"
#include "../../include/yos.h"


/* initialized yos api */
/* interna function to initialize the operating system */
void yos_init() {
    
    /* initialize subsystems... */
    /* mem_init(); */

    /* ...build api pointer table... */

    /* ...and register it. */
    /* register_api(YOS_API); */

    /* finally, start a shell, whatever it is */
    /* shell(); */
}

/* external function to query api from yos */
void *query_api(char *api) {

}

/* external register your own api so that it can be accessed from yos api */
void register_api(char *api, void *name) {

}