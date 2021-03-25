  
/*
 * ef9367.h
 * 
 * a library of graphical primitives for the thompson ef9367 card. 
 * 
 * tomaz stih tue mar 23 2021
 * 
 */
#include "ef9367.h"

/* wait until gr. card is ready, return status register. */
uint8_t  ef9367_wait_ready() __z88dk_fastcall {
#asm
        push af                     ; store flags
busy:
        in a,(EF9367_STS)           ; read the status register
        and EF9367_STS_READY        ; get ready flag
        jr z,busy
        ld l,a                      ; return status
        pop af
        ret
#endasm
}

