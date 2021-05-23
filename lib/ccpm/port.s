		;; port.s
        ;; 
        ;; access i/o ports
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 22.06.2021    tstih
		.module port

		.globl	__port_read
		.globl 	__port_write

        .area	_CODE

        ;; --------------------------------
		;; uint8_t _port_read(uint8_t addr)
        ;; --------------------------------
        ;; reads a byte from port addr
		;; input:	port addr (byte) on stack
		;; output:	l contains value
        ;; affect:  hl
__port_read::
        push af                 ; store context
        push bc                 ; -"-
        ld hl,#2                ; skip ret address
        add hl,sp               ; hl points to first arg
        ld c,(hl)               ; port to c
        in a,(c)                ; read value to a
        ld l,a                  ; return value to l
        pop bc                  ; restore context
        pop af                  ; -"-
        ret

        ;; ---------------------------------------------
		;; void _port_write(uint8_t addr, uint8_t value)
        ;; ---------------------------------------------
        ;; writes a byte to the port
		;; input:	port addr and value on stack
		;; output:	-
        ;; affect:  -
__port_write::
        push af                 ; store context
        push bc                 ; -"-
        push hl                 ; -"-
        ld hl,#2                ; skip ret address
        add hl,sp               ; hl points to first arg.
        ld c,(hl)               ; port to c
        inc hl                  ; next byte arg.
        ld a,(hl)               ; value to a
        out (c),a               ; write to port
        pop hl                  ; restore context
        pop bc                  ; -"-
        pop af                  ; -"-
        ret                     