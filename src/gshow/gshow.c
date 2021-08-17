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

/* stride */
extern int _stridexy(
    coord x, 
    coord y, 
    void *data, 
    uint8_t start, 
    uint8_t end);

typedef struct raster_glyph_s {
    uint8_t stride;
    uint8_t width;
    uint8_t height;
    uint8_t reserved;
} raster_glyph_t;

raster_glyph_t *raster;

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
    raster=malloc(statbuf.st_size);

    /* And load the file into it. */
    int fd=open(fname,O_RDONLY);
    read(fd,raster,statbuf.st_size);
    close(fd);

    /* enter graphics mode */
    gpx_t *g=gpx_init();

    /* Pointer arith.. */
    uint8_t *rasterdata=(uint8_t *)raster;
    rasterdata += 4;
    int tpos=0;

    /* Draw */
    for(int y=ypos;y<ypos+raster->height;y++) {
        _stridexy(xpos,y,rasterdata,0,raster->width);
        rasterdata = rasterdata + (raster->stride + 1);
    }

    /* leave graphics */
    gpx_exit(g);

    /* Free raster memory. */
    free(raster);
}