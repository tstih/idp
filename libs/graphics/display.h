/*
 * display.h
 * 
 * abstraction layer on top of partner graphical capabilities.
 *
 * tomaz stih tue mar 23 2021
 * 
 */
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

/* display properties */
typedef struct display_s {
    uint16_t width;
    uint16_t height;
} display_t;


/* initialize display()
 * clears text and graph. screens, disables text mode (and hides 
 * the cursor) enables graph.mode and sets resolution to 1024x512
 */
extern display_t* display_init();

#endif /* _DISPLAY_H */