		;; scn2674.s
        ;; 
        ;; a library of text mode (only) primitives for the signetics 
        ;; SCN2674 (AVDC) card
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 04.04.2021   tstih
	    .module scn2674

        .globl  __scn2674_cursor_on
	    .globl  __scn2674_cursor_off

        .include "scn2674.inc"

        .area	_CODE


        ;; wait for avdc int
        ;; affects: af
avdc_wait_int:
        ;; if low, wait for high
        in      a, (#SCN2674_GR_CMNI)
        and     #SCN2674_GR_CMNI_SCN2674A
        jr      z,avdc_wait_int
        ;; ...wait for high to end
awi:
        in      a, (#SCN2674_GR_CMNI)
        and     #SCN2674_GR_CMNI_SCN2674A
        jr      nz,avdc_wait_int
        ret


        ;; wait for avdc ready
        ;; affects: af
avdc_wait_rdy:
        in      a, (#SCN2674_STS)
        and     #SCN2674_STS_RDYI
        jr      z,avdc_wait_rdy
        ret


        ;; -------------------------
		;; void _scn2674_cursor_on()
        ;; -------------------------
        ;; shows cursor
        ;; affect:  af
__scn2674_cursor_on::
        call    avdc_wait_int
        ld      a, #SCN2674_CMD_CURS_ON
        out     (#SCN2674_CMD), a
        ret


        ;; --------------------------
		;; void _scn2674_cursor_off()
        ;; --------------------------
        ;; hides cursor
        ;; affect:  af
__scn2674_cursor_off::
        call    avdc_wait_int
        ld      a, #SCN2674_CMD_CURS_OFF
        out     (#SCN2674_CMD), a
        pop     af
        ret