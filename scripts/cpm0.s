		;; cpm0.s
		;; cp/m app startup code
		;;
		;; tomaz stih, fri mar 26 2021
		.module crt0

       	.globl  _main
        .globl  _argc
        .globl  _argv
        .globl  _heap
        .globl  __cpm_sysfunc_init

        ;; consts.
        .equ    TBUFF,  0x80
        .equ    TFCB,   0x5c

        ;; Ordering of segments for the linker.
		.area   _HOME
		.area   _CODE
		.area   _INITIALIZER
		.area   _INITFINAL
	    .area   _GSINIT
	    .area   _GSFINAL	
		.area   _DATA
		.area   _INITIALIZED
        .area   _BSS
        .area   _STACK
	    .area   _HEAP


        .area	_CODE
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

        ;; BDOS exit.
        ld c,#0
	    call 0x0005


        ;; init code for functions/var.
        .area   _GSINIT
gsinit::        
        ;; and call initialize function
        call __cpm_sysfunc_init

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