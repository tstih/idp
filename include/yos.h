/*
 * yos.h
 * y operating system. for now this is just a library, providing fake syscalls on 
 * top of CP/M.
 *
 * tomaz stih thu mar 25 2021
 */
#ifndef _YOS_H
#define _YOS_H

#define YOS_API "yos"

/* core yos api */
struct yos_s {

    /* memory management functions */
    void * (*malloc)();
    void (*free)();

} yos_t;

/* function to query api from yos, returns pointer to api */
extern void *query_api(char *api);

/* register your own api so that it can be accessed from yos api */
extern void register_api(char *api, void *name);

#endif // _YOS_H