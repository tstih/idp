/*
 * scn2674.c
 * 
 * a library of text mode (only) primitives for the signetics scn2674 card.
 *
 * tomaz stih tue mar 23 2021
 * 
 */

/* command register and bits */
#define SCN2647_CMD             0x39
#define SCN2674_CMD_RESET       0x00

/* execute master reset on the hardware */
uint8_t  scn2674_reset() __z88dk_fastcall {
#asm
        la a, SCN2674_CMD_RESET     ; store flags
        out (SCN2647_CMD), a
        ret
#endasm
}