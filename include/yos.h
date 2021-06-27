/*
 * yos.h
 *
 * the yos api. include this into programs that use the yos system calls.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 07.04.2021   tstih
 *
 */
#ifndef __YOS_H__
#define __YOS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* yos specific types */
typedef void *          handle;
typedef uint8_t         result;

/* NULL def. as int */
#ifndef NULL
#define NULL 0
#endif

/* yos API */
#define YOS_API "yos"

typedef struct yos_s
{
    /* memory allocation */
    void *(*malloc)(size_t size);
    void (*free)(void *p);

    /* string */
    size_t (*strlen)(char *str);
    char *(*strcpy)(char *dest, char *src);
    int (*strcmp)(char *s1, char *s2);

    /* system lists */
	void* (*lappend)(void **first, void *el);
	void* (*linsert)(void **first, void *el);
	void* (*lremove)(void **first, void *el);
	void* (*lremfirst)(void **first);

    /* system wide errors */
    result (*geterr)();
    result (*seterr)(result err);
} yos_t;

/* error codes */
#define SUCCESS         0
#define OUT_OF_MEMORY   1

/* call this to obtain the yos interface
 *   yos_t* api=(yos*)query_api("yo00"); // 2 letters + 2 dig. version
 *   void* mem_block=api->malloc(1024);
 * this function is usually defined in crt0.s for yos application
 * and follows standard protocol (such as RST 0x10 call) to obtain the address.
 */
void *query_api(char *name);

#endif /* __YOS_H__ */