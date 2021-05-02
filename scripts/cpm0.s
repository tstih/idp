		;; cpm0.s
		;; cp/m app startup code
		;;
		;; tomaz stih, fri mar 26 2021
		.module cpm0

       	.globl  _main
        .globl  _heapaddr
        .globl  _rtargc
        .globl  _rtargv
        .globl  _stktop
        .globl  __cpm_sysfunc_init
	    
        .area	_CODE
init:
        ;; define a stack   
        ld	sp,#_stktop

        ;; SDCC init global variables
        ;; no need to copy data seg. in this scenario
        call gsinit
        call __cpm_sysfunc_init

        ;; cmd line args to the stack
        ld hl,#0x0050
        push hl
        ld hl,(_rtargc)
        push hl

	    ;; call the main
	    call _main

        ;; BDOS exit.
        ld c,#0
	    call 0x0005

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

        .area   _DATA
_rtargc::
        .dw 0xFFFF
_rtargv::
        .ds 16

        .area	_STACK
	    .ds	512
_stktop::

        .area   _GSINIT
gsinit::
        .area   _GSFINAL
        ret
	    .db	0xe5                        ;; but why?

_heapaddr::
        .db 0x00