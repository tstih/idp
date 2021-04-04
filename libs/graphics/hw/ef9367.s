		;; ef9367.s
        ;; a library of graphical primitives for the thompson ef9367 card. 
		;;
		;; tomaz stih, sun apr 04 2021
		.module ef9367

		.globl	_ef9367_wait_sts_ready
		.globl	_ef9367_set_1024x512
       	.globl	_ef9367_xy
		.globl 	_ef9367_cls
	    
		;; consts
		.equ	EF9367_STS, 0x20 
		.equ	EF9367_STS_READY, 0x04	; NOTE: ignore light pen!

		.equ	EF9367_CMD, 0x20
		.equ 	EF9367_CMD_CLS00, 0x06 

		.equ	EF9367_XPOS_HI, 0x28
		.equ	EF9367_XPOS_LO, 0x29
		.equ	EF9367_YPOS_HI, 0x2a
		.equ	EF9367_YPOS_LO, 0x2b

		.equ	EF9367_GR_CMN, 0x30
		.equ	EF9367_GR_CMN_1024x512, 0x18

		;; hires
		.equ	MAXX, 1024
		.equ	MAXY, 512

        .area	_CODE


		;; uint8_t ef9367_wait_sts_ready() __naked 
_ef9367_wait_sts_ready::
		push af                         ; store affected regs
sts_nready:
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,sts_nready
        ld l,a                          ; return status
        pop af
        ret

		;; init graphic mode, set resolution to 1024x512
_ef9367_set_1024x512::
		ld a,#EF9367_GR_CMN_1024x512
		out (#EF9367_GR_CMN),a
		ret


		;; clear graphic screen, move cursor to 0,0 (left, bottom)
_ef9367_cls::
		ld a,#EF9367_CMD_CLS00
		out (#EF9367_CMD),a
		ret


		;; void ef9367_xy(uint16_t x, uint16_t y) __naked
_ef9367_xy:: 
		ld hl,#2						; hl=sp+2 (skip over ret addr)
		add hl,sp						
		ld a,(hl)						; a=low (x)
		out (#EF9367_XPOS_LO),a
		inc hl
		ld a,(hl)						; a=hi (x)
		out (#EF9367_XPOS_HI),a
		inc hl
		;; now reverse y coordinate
		ld e,(hl)
		inc hl
		ld d,(hl)
		ld hl,#MAXY
		sbc hl,de						; hl=512-y
		ld c,#EF9367_YPOS_HI
		out (c),h
		inc c
		out (c),l
		ret