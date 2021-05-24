		;; bcd.s
        ;; 
        ;; 8 bit binary coded decimal conversions
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 22.06.2021    tstih
		.module bcd

		.globl	__bin2bcd
		.globl 	__bcd2bin

        .area	_CODE

        ;; -----------------------------
		;; uint8_t _bin2bcd(uint8_t bin)
        ;; -----------------------------
        ;; convert a number to bcd
		;; input:	number
		;; output:	bcd number
        ;; affect:  hl
__bin2bcd::
        push af
        push bc
        ld hl,#6
        add hl,sp
        ld a,(hl)               ; get bin to a
        ld c, a
        ld b, #0x08
        xor a
b2bloop:
        sla	c
        adc	a, a
        daa
        djnz b2bloop
        ld l,a                  ; store result
        pop bc
        pop af
        ret

        ;; -----------------------------
		;; uint8_t _bcd2bin(uint8_t bcd)
        ;; -----------------------------
        ;; convert a bcd number to bin
		;; input:	bcd number
		;; output:	number
        ;; affect:  hl
__bcd2bin::
        push af
        push bc
        ld hl,#6
        add hl,sp
        ld a,(hl)
        ld	c,a
        and	#0xf0
        srl	a
        ld	b,a
        srl	a
        srl	a
        add	a,b
        ld	b,a
        ld	a,c
        and	#0x0f
        add	a,b
        ld l,a                  ; store result to l
        pop bc
        pop af
        ret