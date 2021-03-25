#include "hw/ef9367.h"
#include "hw/scn2674.h"

display_t* display_init() {

    /* clear screen, hide cursor, disable tty */
    scn2674_init();

    /* using z80 pio call to enter 1024x512 hires mode */
}