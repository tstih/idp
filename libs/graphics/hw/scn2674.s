		;; scn2674.s
        ;; a library of text mode (only) primitives for the signetics 
        ;; scn2674 card.
		;;
		;; tomaz stih, sun apr 04 2021
		.module scn2674

        .globl	_scn2674_reset
        .globl  _scn2674_display_off
	    .globl  _scn2674_cursor_off

		;; consts
		.equ	SCN2647_CMD, 0x39
        .equ	SCN2674_CMD_RESET, 0x00
        .equ    SCN2674_CMD_CURS_OFF, 0x30

        .area	_CODE
        ;; void  scn2674_reset() __naked 
_scn2674_reset::

        ld a, #SCN2674_CMD_RESET        ; store flags
        jr scn2674_exec_cmd             ; subroutine will perform ret


_scn2674_display_off::
        ld a, #0b00101000               ; display off (0x28)
        jr scn2674_exec_cmd


_scn2674_cursor_off::
        ld a, #SCN2674_CMD_CURS_OFF     ; cursor off
        jr scn2674_exec_cmd

        ;; execute command in a
scn2674_exec_cmd:
        out (#SCN2647_CMD), a
        ret