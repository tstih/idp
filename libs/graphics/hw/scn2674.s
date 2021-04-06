        ;; scn2674.s
        ;; a library of text mode (only) primitives for the signetics 
        ;; scn2674 card.
	;;
	;; tomaz stih, sun apr 04 2021
	.module scn2674

        .globl	_scn2674_reset
        .globl  _scn2674_wait_sts_ready
        .globl  _scn2674_display_off
	.globl  _scn2674_cursor_off
        .globl  _scn2674_cls

	;; consts
	.equ	SCN2647_CMD, 0x39
        .equ	SCN2674_CMD_RESET, 0x00
        .equ    SCN2674_CMD_CURS_OFF, 0x30
        .equ    SCN2647_CMD_WC2P, 0xbb
        .equ    SCN2647_CMD_WAC, 0xab
        .equ	SCN2647_CMD_LIR10, 0b00011010
        .equ	SCN2647_CMD_LIR11, 0b00011011

        .equ    SCN2647_INIT, 0x38
        .equ    SCN2647_CHR, 0x34
        .equ    SCN2647_AT, 0x35

        .equ    SCN2647_STS, 0x39
        .equ    SCN2647_STS_RDY, 0x20

        .area	_CODE
        ;; void  scn2674_reset() __naked 
_scn2674_reset::
        ld a, #SCN2674_CMD_RESET        ; store flags
        jr scn2674_exec_cmd             ; subroutine will perform ret


_scn2674_wait_sts_ready::
	push af                         ; store affected regs
sts_nready:
        in a,(#SCN2647_STS)              ; read the status register
        and #SCN2647_STS_RDY           ; get ready flag
        jr z,sts_nready
        ld l,a                          ; return status
        pop af
        ret


_scn2674_display_off::
        ld a, #0b00101000               ; display off (0x28)
        jr scn2674_exec_cmd


_scn2674_cursor_off::
        ld a, #SCN2674_CMD_CURS_OFF     ; cursor off
        jr scn2674_exec_cmd


_scn2674_cls::
        push af
        ld a,#0x20                      
        out (#SCN2647_CHR),a            ; write space
        xor a                        
        out (#SCN2647_AT),a             ; attr=0 (no effects)
        
        ; set pointer 
        ld a,#SCN2647_CMD_LIR10         ; set IR10
        out (#SCN2647_CMD),a
        ld a,#0xff                      ; to 0xff
        out (#SCN2647_INIT),a

        ld a,#SCN2647_CMD_LIR11         ; set IR11
        out (#SCN2647_CMD),a
        ld a,#0x0f                      ; to 0x0f
        out (#SCN2647_INIT),a 

        ld a,#SCN2647_CMD_WC2P
        out (#SCN2647_CMD),a
        ld a,#SCN2647_CMD_WAC           ; compensate for emul. bug
        out (#SCN2647_CMD),a
        pop af
        ret


        ;; execute command in a
scn2674_exec_cmd:
        call _scn2674_wait_sts_ready    ; wait for ready status
        out (#SCN2647_CMD), a
        ret
