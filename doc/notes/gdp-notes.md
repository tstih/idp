# Notes from 06. apr 2021

## Thomson code analysis in bios

~~~
pio_init::
        ;; interrupt control word (0x0111)
        ;; bit 7: interrupt enable (0)
        ;; bit 6: and/or mode (0 ... or)
        ;; bit 5: hi/lwo trigger (0 ... low)
        ;; bit 4: mask follows (0 ... it doesn't)
        ld a,#0x07                      ; 0b00000111
        out (#0x31),a                   ; write pio port A ctrl. reg. 
        out (#0x33),a                   ; and pio port B ctrl. deg.
        ;; set operation mode (0x1111)
        ;; bits 7-6: 00 (output)
        ld a,#0x0f                      ; 0b00001111
        out (#0x31),a                   ; write pio port A ctrl. reg. 
        out (#0x33),a                   ; and pio port B ctrl. deg.
        ret

thomson_init:
        ;; set resolution
        ld a,#0x18                      ; 0b00011000
        out (#0x30),a                   ; set resolution to 11 

        ;; set common text attributes for thomson
        ;; bit 7: 24/16Mhz text video clock (16Mhz)
        ;; bit 6-5 : dots per char (11=3)
        ;; bit 4: reverse screen / green (0 green)
        ;; bit 3: force background / blue foreground (1 blue)
        ;; bit 2: mono/colour (1=colour)
        ;; bit 1: cursor mode (0)
        ;; bit 0: dot stretch (1=yes)
        ld a,#0x6d                      ; 0b 0110 1101
        out (#0x32),a                   ; initialize graphical card

        xor a                           ; a=0
        out(#0x36),a                    ; scroll 0 lines

        ld a,#0x03
        out (#0x21),a                   ; select pen, pen up

        ld a,#0x04                      ; cls
        call thomson_cmd

        ld a,#0x05
        call thomson_cmd                ; goto 0,0
        ret

        ;; not really thomson call
text_init:
        xor a 
        out (#0x39),a                   ;  
        ret

        ;; the fuck?
reset:
	    xor a
	    out	(#0x39),a                     ; master reset?! text
	    out	(#0x30),a                     ; resolution to 0 thomson
~~~