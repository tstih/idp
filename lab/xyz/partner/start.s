        ;; partner/start.s
        ;; 
        ;; startup code for partner at address 0x0000
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 05.04.2021    tstih
		.module start


        .area   _RESET(ABS)
        ;; --- rst vectors ----------------------------------------------------
        .org    0x0000
rst00: 
        ;; interrupts are disabled, but do it again (for safe reset)
        di
        ;; and jump to init
        jr	init			            ; relative jump to init!
		.db	0,0,0,0,0			
		;; rst 0x08
rst08:  reti
		.db	0,0,0,0,0,0
		;; rst 0x10
rst10:  reti
		.db	0,0,0,0,0,0
		;; rst 0x18
rst18:  reti
		.db	0,0,0,0,0,0
		;; rst 0x20
rst20:  reti
		.db	0,0,0,0,0,0
		;; rst 0x28
rst28:  reti
		.db	0,0,0,0,0,0
		;; rst 0x30
rst30:  reti
		.db	0,0,0,0,0,0
		;; rst 0x38
rst38:  reti
        ;; 46 bytes - RETI opcode lenght = 44 bytes until NMI 0x66
        .dw 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        ;; non maskable interrupt (and 1 byte of space for JP instruction)
nmiret: retn
        .db 0


        ;; --- operating system init  -----------------------------------------
init:   
        ;; first initialize the vector table for im2
        ld b,#0xff
        ld de,#preti                    ; point to reti
        ld hl,#im2vectors               ; address of the vector table
im2loop:
        ld (hl),e
        inc hl
        ld (hl),d
        inc hl
        djnz im2loop
        ;; now the vector table for im1
        ld b,#0x08
im1loop:
        ld (hl),e
        inc hl
        ld (hl),d
        inc hl
        djnz im2loop
        ;; and finally the nmi vector table
        ld de,#pretn
        ld (hl),e
        inc hl
        ld (hl),d

        ;; now we can safely enable interrupts
        ld hl,#im2vectors
        inc hl
        ld a,(hl)                       ; get high order of vector table
        ld i,a                          ; vector table is 0xff
        im 2                            ; enter interrupt mode 2
        ei                              ; and enable interrupts


        ;; --- operating system start  ----------------------------------------

preti:
        reti
pretn:
        retn


        ;; --- im2 vectors ----------------------------------------------------
        .org    0x0400
im2vectors:
        .ds 512                         ; 256 vectors, 2 bytes each
im1vectors:
        .ds 16                          ; 8 vectors, 2 bytes each
nmivector:
        .ds 2