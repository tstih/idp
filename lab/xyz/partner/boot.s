        ;; boot.s
        ;;
        ;; boot sector for id partner
        ;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
        ;; 27.03.2021   tstih
		.module boot

        .globl  _boot
        .globl  _start

        .equ    OS_STACK, 0xffff


        ;; --- the boot  ------------------------------------------------------
        .area   _BOOT(ABS)
        .org    0xe000
_boot::   
        ;; check if the SETUP button is pressed?

        ;; load the operating system to 0x0000

        ;; and jump to it
        jp      0x0000


        ;; --- start ----------------------------------------------------------
        ;; after loading the boto sector, ROM jumps to 0xf600
        .area   _START(ABS)
	    .org    0xf600
_start::
        ;; set operating system stack pointer
        ;; to the top of shared memory
        ;; ROM checks if the first instruction is JP or LD SP,#nnnn
        ld sp,#OS_STACK

        ;; don't surprise us while we're setting up things
        di

        ;; switch off ROM, make space...
        xor a
        out (#0x80),a

        ;; and jump to boot
        jp _boot