/*
 * hello.c
 *
 * Prints 'Hello Partner!'
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 02.05.2021   tstih
 *
 */
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    FILE *fp;
    char *s="There'll be blue birds over\r\nthe white cliffs of Dover.\r\n";

    if (argc<=1) {
        puts("Please provide filename as the first argument.\r\n");
        exit(1);
    }

    puts("Creating file "); puts(argv[1]); puts("\r\n");
    fp = fopen( argv[1] , "w" );
    puts(strerror(errno)); puts("\r\n");

    puts("Writing to file.\r\n");
    fwrite(s , 1 , strlen(s) , fp );
    puts(strerror(errno)); puts("\r\n");

    puts("Closing file.\r\n");
    fclose(fp);
    puts(strerror(errno)); puts("\r\n");

    puts("The end.\r\n");

    return 0;
}