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

        .globl  _scn2674_init
        .globl  _scn2674_cursor_on
	    .globl  _scn2674_cursor_off
        .globl  _scn2674_cls

        .include "scn2674.inc"

        .area	_CODE


        ;; ---------------
		;; void SCN2674_init()
        ;; ---------------
        ;; initializes the SCN2674, sets up SCN2674 IR registers, cursor position, 
        ;; screen start position
		;; input:	-
		;; output:	-
        ;; affect:  -
_scn2674_init::
        ;; store registers
        push af
        push bc
        push hl
        ;; first do a master reset
        ld a,#SCN2674_CMD_RESET
		out	(#SCN2674_CMD),a
        ;; give the card time to recover
        ld b,#0xff
SCN2674_delay:
        nop
        nop
        nop
        djnz SCN2674_delay
        ;; reset the IR pointer to 0 (just in case)
		ld a,#SCN2674_CMD_SET_IR						
		out (#SCN2674_CMD),a
        ;; now initialize registers IR0-IR11
        ld hl,#SCN2674_rtbl			    ;; table for registers IR0-IR11
		ld b,#0x0c						;; 12 bytes
		ld c,#SCN2674_INIT              ;; out port will be 0x38 i.e. IR initialization
		otir							;; and loop
		;; set screen start to 0 (for both screens)
		xor a
		out (#SCN2674_SS1_LO),a
		out (#SCN2674_SS1_HI),a
		out (#SCN2674_SS2_LO),a
		out (#SCN2674_SS2_HI),a
        ;; move cursor to 0,0
        xor a
        out (#SCN2674_CUR_LO),a
        out (#SCN2674_CUR_HI),a
        ;; restore registers
        pop hl
        pop bc
        pop af
        ;; and return
        ret
        ;; initialization table
SCN2674_rtbl:
		;; decoding IR table
		;; IR0 = 0xd0 (0b11010000) 
		;; bit 7 double width (yes)
		;; bits 6-3 scan lines per char row (10)
		;; bit 2 vsync/csync pulses (csync)
		;; bits 1-0 buffer mode select (00 - independent)
		.db 0xd0
		;; IR1 = 0x2f (0b00101111)
		;; bit 7 interlace (no)
		;; bits 6-0 equalizing const for csync (2f)
		.db 0x2f 
		;; IR2 = 0x0d (0b00001101)
		;; bit 7 row table mode enable (0)
		;; bits 6-3 horizontal sync pulse width (1)
		;; bits 2-0 horizontal back porch (5)
		.db 0x0d 
		;; IR3 = 0x05 (0b00000101)
		;; bits 7-5 vertical front porch (0)
		;; bits 4-0 vertical back porch (5)
		.db 0x05 
		;; IR4 = 0x99 (0b10011001)
		;; bit 7 char blink rate (1)
		;; bit 6-0 char rows per screen (25)
		.db 0x99 
		;; IR5 = 0x4f (0b01001111)
		;; bits 7-0 active chars per row (79)
		.db 0x4f 
		;; IR6 = 0x0a (0b00001010)
		;; bits 7-4 first scan line of cursor 0
		;; bits 3-0 last scan line of cursor 10
		.db 0x0a 
		;; IR7 = 0xea (0b11101010)
		;; bits 7-6 vertical sync pulse width (3)
		;; bit 5 cursor blink enable (1)
		;; bit 4 cursor rate (0 = 1/32)
		;; bits 3-0 underline position (10)
		.db 0xea 
		;; IR8 = 0x00 (0b00000000)
		;; bits 7-0 LSB of display buffer first address
		.db 0x00
		;; IR9 = 0x30 (0b00110000)
		;; bits 7-4 display buffer last address (8191)
		;; bits 3-0 display buffer first address msb (0)
		.db 0x30
        ;; IR10, IR11 = 0x1fff (effectively 8191)
        ;; lower byte first
        .db 0xff, 0x1f


        ;; ---------------------
		;; void SCN2674_cursor_on()
        ;; ---------------------
        ;; shows SCN2674 cursor
		;; input:	-
		;; output:	-
        ;; affect:  -
_scn2674_cursor_on::
        push af
        ld a, #SCN2674_CMD_CURS_ON
        out (#SCN2674_CMD), a
        pop af
        ret


        ;; ----------------------
		;; void SCN2674_cursor_off()
        ;; ----------------------
        ;; hides SCN2674 cursor
		;; input:	-
		;; output:	-
        ;; affect:  -
_scn2674_cursor_off::
        push af
        ld a, #SCN2674_CMD_CURS_OFF
        out (#SCN2674_CMD), a
        pop af
        ret


        ;; ---------------
		;; void SCN2674_cls()
        ;; ---------------
        ;; clears screen, the cursor position is a mess after this operation
		;; input:	-
		;; output:	-
        ;; affect:  -
_scn2674_cls::
        push af                         ; make sure we dont destroy the acc.
        ;; move cursor to 0,0
        xor a
        out (#SCN2674_CUR_LO),a
        out (#SCN2674_CUR_HI),a
        ;; fill screen with char ' ', attr 0
		ld a,#' '
		out (#SCN2674_CHR), a           ; use space chars!
		ld a,#SCN2674_AT_NONE				
		out (#SCN2674_AT), a            ; and no attributes
        ;; fill from cursor to display pointer
        ld a,#SCN2674_CMD_WC2P
        out (#SCN2674_CMD), a
        pop af                          ; restore the acc.
        ret