        ;; boot-rts.s
        ;;
        ;; switches RTS on and off everz second
        ;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
        ;; 19.04.2021   tstih
		.module boot_rts

        .globl  _boot
        .globl  _start

        .equ    OS_STACK,       0xffff

        ;; real time clock second 
        .equ    RTC_SEC,        0xa2
        
        ;; SIO port B (Partner DB25 "LPT"!)
        .equ    SIO_B_DATA,     0xda
        .equ    SIO_B_CTRL,     0xdb


        ;; --- the boot  ------------------------------------------------------
        .area   _BOOT(ABS)
        .org    0xe000
_boot::   
        call sio_init
b_loop:
        call sio_rts_on
        call wait1sec
        call sio_rts_off
        call wait1sec
        jr b_loop                    	; and loop        


        ;; initialize port B to RX/TX 8-N-1
sio_init:
        ld	c,#(SIO_B_CTRL)  
        ld	hl,#sio_init_seq            ; initial. sequence
        ld	b,#7                        ; 7 bytes
        otir
        ret
sio_init_seq:
        .db	0x18, 0x04 ,0x44, 0x03, 0xC1, 0x05, 0x68


        ;; set port B RTS on
sio_rts_on:
        ld a,#0x05
        out (#SIO_B_CTRL),a
        ld a,#0xea                      ; rts bit on
        out (#SIO_B_CTRL),a             
        ret


        ;; set port B RTS on
sio_rts_off:
        ld a,#0x05
        out (#SIO_B_CTRL),a
        ld a,#0xe8                      ; rts bit off
        out (#SIO_B_CTRL),a             
        ret


        ;; wait 1 second
wait1sec:
        push af
        push bc
        in a,(#RTC_SEC)                 ; read the current second
        ld b,a                          ; store to the b register
w1s_loop:
        in a,(#RTC_SEC)                 ; read the second again
        cp b                            ; compare to prev. read
        jr z,w1s_loop                   ; if equal, loop...
        pop bc
        pop af
        ret


        ;; --- start ----------------------------------------------------------
        ;; after loading the boto sector, ROM jumps to 0xf600
        .area   _START(ABS)
	    .org    0xf600
_start::
        ;; set operating system stack pointer
        ;; to the top of shared memory
        ;; ROM checks if the first instruction is JP or LD SP,#nnnn
        ld sp,#OS_STACK

        ;; and jump to boot
        jp _boot
