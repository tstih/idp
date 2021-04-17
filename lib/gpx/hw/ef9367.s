		;; ef9367.s
        ;; 
        ;; a library of primitives for the thompson ef9367 card
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 04.04.2021   tstih
		.module ef9367

		.globl	_gdp_init
		.globl 	_gdp_cls
        .globl	_gdp_xy
	    
		.include "ef9367.inc"

        ;; this constants are valid only for hires mode
        .equ    HIRES_WIDTH,        1024
        .equ    HIRES_HEIGHT,       512

        .area	_CODE


        ;; ---------------
		;; void gdp_init()
        ;; ---------------
        ;; initializes the gdp, set the 1024x512 graphics mode
		;; input:	-
		;; output:	-
        ;; affect:  -
_gdp_init::
        push af
		ld a,#PIO_GR_CMN_1024x512
		out (#PIO_GR_CMN),a
        pop af
		ret


        ;; --------------
		;; void gdp_cls()
        ;; --------------
		;; clear graphic screen
		;; input:	-
		;; output:	-
        ;; affect:  -
_gdp_cls::
        push af
		ld a,#GDP_CMD_CLS
		jr gdp_jr_cmd


        ;; ---------------------------------
		;; void gdp_xy(int16_t x, int16_t y)
        ;; ---------------------------------
        ;; move the cursor to x,y
        ;; notes:   y is transformed from bottom to top to top to bottom coord.
		;;          using the B2T_TRAN_Y const.
        ;; input:	-
		;; output:	-
        ;; affect:  -
_gdp_xy::
        ;; store regs
        push af
        push bc
        push de
        push hl

        ;; set x coordinate
		ld hl,#2						; hl=sp+2 (skip over ret addr)
		add hl,sp						
		ld a,(hl)						; a=low (x)
		out (#GDP_XPOS_LO),a
		inc hl
		ld a,(hl)						; a=hi (x)
		out (#GDP_XPOS_HI),a
		inc hl

		;; now reverse amd set y
		ld e,(hl)
		inc hl
		ld d,(hl)
		ld hl,#HIRES_HEIGHT
		sbc hl,de						; hl=512-y
        dec hl                          ; we really need 511-y
		ld c,#GDP_YPOS_HI
		out (c),h
		inc c
		out (c),l

        ;; restore regs
        pop hl
        pop de
        pop bc
        pop af

		ret


        ;; executes gdp command & wait for status
        ;; notes:   push af to stack and jr to this function to save 
        ;;          a pop af and ret call
        ;; input:	a=command, af is stored on the stack
		;; output:	af is restored from the stack
        ;; affect:  the stack
gdp_jr_cmd:
        call gdp_wait_sts_ready         ; wait until GDP is ready
        out (#GDP_CMD), a               ; exec. command
        pop af
        ret                             ; ret so that callers don't have to


        ;; executes gdp command & wait for status
        ;; input:	a=command
		;; output:	-
        ;; affect:  flags
gdp_call_cmd:
        call gdp_wait_sts_ready         ; wait until GDP is ready
        out (#GDP_CMD), a               ; exec. command
        ret


        ;; wait until graphic card is ready for next command
		;; input:	-
		;; output:	-
		;; affect:	-
gdp_wait_sts_ready:
		push af                         ; store affected reg
sts_not_ready:
        in a,(#GDP_STS)                 ; read the status register
        and #GDP_STS_READY              ; get ready flag
        jr z,sts_not_ready
        pop af
        ret