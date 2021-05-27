		;; crt0cpm.s
		;; cp/m app startup code
		;;
		;; tomaz stih, fri mar 26 2021
		.module crt0cpm

       	.globl  _main
        .globl  _argc
        .globl  _argv
        .globl  _heap
        .globl  _exit
        .globl  __stdlib_init

        ;; consts.
        .equ    TBUFF,  0x80
        .equ    TFCB,   0x5c

		;; Ordering of segments for the linker (after header)
		.area   _CODE
		.area   _HOME
		.area   _GSINIT
	    .area   _GSFINAL
		.area   _INITIALIZER
		.area   _INITFINAL	
		.area   _DATA
		.area   _INITIALIZED
        .area   _BSS
        .area   _STACK
	    .area   _HEAP

  		.area _CODE
start:
        ;; define a stack   
        ld	sp,#stack

        ;; SDCC init global variables
        ;; no need to copy data seg. in this scenario
        call gsinit

        ;; load argc and argv to stack for the main function
        ld hl, #_argv
        push hl
        ld hl, (#_argc)
        push hl

        ;; call the main
	    call _main

        ;; exit (also a place for the exit() function
        ;; we ignore the return code AND the stack as reset
        ;; will handle both...
_exit::
        ;; BDOS exit (reset) return control to CP/M.
        ld c,#0
	    jp 5

        ;; init code for functions/var.
        .area   _GSINIT
gsinit::      
        ;; copy statics.
        ld bc, #l__INITIALIZER
        ld a, b
        or a, c
        jr Z, gsinit_next
        ld de, #s__INITIALIZED
        ld hl, #s__INITIALIZER
        ldir
gsinit_next:
        ;; and call initialize function
        call __stdlib_init

        .area   _GSFINAL
        ret


        .area   _DATA
_argc::
        .dw 1                           ; default argc is 1 (filename!)
_argv::
        .ds 16                          ; max 8 argv arguments

        .area	_STACK
	    .ds	512
stack:


        .area   _HEAP
_heap::                                 ; start of our heap.
