/*
 * gshow.c
 *
 * Show gph image.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 02.05.2021   tstih
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <gpx.h>

glyph_t *glyph;

int main(int argc, char *argv[]) {

    /* First check the arguments. */
    if (argc!=4) {
        printf("Usage: gshow <gph-file> <x> <y>\n\n");
        return -1;
    }
    char *fname=argv[1];
    int xpos=atoi(argv[2]), ypos=atoi(argv[3]);

    /* Get file len. */
    struct stat statbuf;
    if (stat(fname, &statbuf)<0) {
        printf("Unable to get file len for %s, exiting.\n", fname);
        return -1;
    }

    /* Now that we've got the length, allocate memory. */
    glyph=malloc(statbuf.st_size);

    /* And load the file into it. */
    int fd=open(fname,O_RDONLY);
    int bread=read(fd,glyph,statbuf.st_size);
    close(fd);

    /* enter graphics mode */
    gpx_t *g=gpx_init();
    
    gpx_draw_glyph(g, xpos, ypos, glyph);

    /* leave graphics */
    gpx_exit(g);

    /* Free raster memory. */
    free(glyph);
}