		;; cpm0.s
		;; cp/m app startup code
		;;
		;; tomaz stih, fri mar 26 2021
		.module crt0

       	.globl	_main
	    
        .area	_CODE
	    .org	0x0100
init:
        ;; define a stack   
        ld	sp,#stack

        ;; SDCC init global variables
        ;; no need to copy data seg. in this scenario
        call gsinit

	    ;; call the main
	    call _main

        ;; BDOS exit.
        ld c,#0
	    call 0x0005

        ;; Ordering of segments for the linker.
		.area _HOME
		.area _CODE
		.area _INITIALIZER
		.area _INITFINAL
	    .area _GSINIT
	    .area _GSFINAL	
		.area _DATA
		.area _INITIALIZED
        .area _BSS
        .area _STACK
	    .area _HEAP

	    
        .area	_STACK
	    .ds	512
stack:

        .area   _GSINIT
gsinit::
        .area   _GSFINAL
        ret
	    .db	0xe5                        ;; but why?