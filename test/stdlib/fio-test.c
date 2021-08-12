/*
 * fio-test.c
 *
 * Standard C library file I/O tests.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 08.08.2021   tstih
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../test.h"

extern int all_tests();
static int tests_run = 0;

/* Structures for testing */
static uint8_t buf[512];
struct stat statbuf;


void main() {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n\r");
    printf("Tests run: %d\n\r", tests_run);
}


/* Should parse filenames correctly 
   filname is */
int fname_parse_test() {
    
    /* result placeholders */
    int user;
    char fname[MAX_FNAME+1];
    char drive;
    char ext[MAX_EXT+1];
    
    /* must fail - invalid chars*/
    const char *fn1=":TEST.C[0]";
    ASSERT(splitpath(fn1, &drive, &user, fname, ext));
 
    /* must fail - too long */
    const char *fn2="jojorabbit.txt";
    ASSERT(splitpath(fn2, &drive, &user, fname, ext));

    /* must succeed */
    const char *fn3="P:TEST.C[12]";
    ASSERT(!splitpath(fn3, &drive, &user, fname, ext));

    /* must succeed */
    const char *fn4="MAMAMIA.TXT";
    ASSERT(!splitpath(fn4, &drive, &user, fname, ext));

    return 0;
}


int read1_test() {
    int fd=open("fio1.tst",O_RDONLY); ASSERT(fd>2);
    /* Read 1 byte. */
    int bread=read(fd,buf,1); ASSERT(bread==1);
    ASSERT(close(fd)==0);
    return 0;
}


int read100_test() {
    int fd=open("fio100.tst",O_RDONLY); ASSERT(fd>2);
    /* Read too many bytes! EOF should work!
       Returned bytes will be 128 because this is
       DMA size (CP/M problem with file sizes) */
    int bread=read(fd,buf,256); ASSERT(bread==128);
    /* Read again. Should still be EOF. */
    bread=read(fd,buf,256); ASSERT(bread==0);
    ASSERT(close(fd)==0);
    return 0;
}


int read400_test() {
    /* Heavy burden for the stack. */
    int fd=open("fio400.tst",O_RDONLY); ASSERT(fd>2);
    /* Read exactly 400 bytes. */
    int bread=read(fd,buf,400); ASSERT(bread==400);
    ASSERT(close(fd)==0);
    return 0;
}


int read100k_test() {
    /* Read 2k, chunk by chunk */
    uint16_t pos=0;
    int fd=open("fio100k.tst",O_RDONLY); ASSERT(fd>2);
    while (pos<50000) {
        /* Next 500 bytes. */
        int bread=read(fd,buf,500); ASSERT(bread==500);
        pos+=500;
    }
    ASSERT(close(fd)==0);
    return 0;
}


int stat_test() {

    ASSERT(stat("fio1.tst", &statbuf)==0);
    ASSERT(statbuf.st_size==1);
    ASSERT(stat("fio100.tst", &statbuf)==0);
    ASSERT(statbuf.st_size==100);
    ASSERT(stat("fio400.tst", &statbuf)==0);
    ASSERT(statbuf.st_size==400);
    ASSERT(stat("fio16k.tst", &statbuf)==0);
    ASSERT(statbuf.st_size==16384);
    ASSERT(stat("fio100k.tst", &statbuf)==0);
    /* NOTE: Files over 64K don't have
       exact size? */
    
    return 0;
}


int all_tests() {
    VERIFY(fname_parse_test);
    VERIFY(read1_test);
    VERIFY(read100_test);
    VERIFY(read400_test);
    VERIFY(read100k_test);
    VERIFY(stat_test);
    return 0;
}