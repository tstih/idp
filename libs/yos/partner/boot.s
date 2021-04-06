		;; partner/boot.s
        ;; boot sector for iskra delta partner 
		;;
		;; tomaz stih, mon apr 05 2021
		.module boot

        .equ    BOOT_STACK, 0xffff

        .area   _CODE
		;; boot sector is loaded to 0xe000
        .org    0xe000
boot::
        di                              ; no interrupts

        ;; switch off ROM.
        xor a
        out (#080),a

        ;; read operating system from disk.
        

        ;; the boot is done. jump to OS.
        jp 0x0000

        .area   _INIT
        ;; but startup is at 0xf600
	    .org    0xf600
init::
        ;; set operating system stack pointer
        ;; to the top of shared memory
        ld sp,#BOOT_STACK

        ;; jump to boot 
        jp boot