/*
 * gpx1-xp.c
 *
 * Graphical functions test.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 12.08.2021   tstih
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <gpx.h>

void main() {
    /* enter gpx mode */
    gpx_t *g=gpx_init();

    /* query graphics capabilities */
    gpx_cap_t *cap=gpx_cap(g);
    printf("GRAPHICS PROPERTIES\n\n");
    printf("No. colors %d\nBack color %d\nFore color %d\n",
        cap->num_colors,
        cap->back_color,
        cap->fore_color);
    printf("Sup. pages %d\n", cap->num_pages);
    /* enum. pages */
    for(int p=0; p<cap->num_pages; p++)
        /* enum resolutions (for page) */
        for (int r=0; r<cap->pages[p].num_resolutions; r++)
            printf(" P%d Resol. %dx%d\n",
                p,
                cap->pages[p].resolutions[r].width,
                cap->pages[p].resolutions[r].height);
    
    /* leave gpx mode */
    gpx_exit(NULL);
}