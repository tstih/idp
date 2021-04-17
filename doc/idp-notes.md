# Notes from 21. mar 2021

## Floppy

Floppy disk size is 672768 bytes (146 tracks, 18 sectors, 256 bytes each). 

* TODO: is following info for HDD or FDD?
First two tracks (9216 bytes) are reserved for CPMLDR, followed by 2 blocks 
of directory each block is 8 sectors i.e. 2048 bytes).

## Emulated start up

HDD controller is not emulated - the emulator instead redirects BIOS calls.

ROM loads CPM loader (from boot sector) to 0xe000 and jumps to 0xf600. The emulator 
expects first byte to have a value of 31h (opcode for LD SP,). Suspected length of loader 
is 0x1F00 bytes. Code at 0xf600 copies the loader to 0x100.

* TODO: Find real length of the loader.
* TODO: Find out whay it copies itself to 0x100.

The loader uses virtual 128 byte disk blocks and does not implement write (or is not simulated).
Due to the emulator shortcomings you should avoid addresses between 0xb00 and 0xb72.

* TODO: Understand this.


# Notes from 05. apr 2021

## Understanding the AVDC 

Here is complete AVDC initialization routine.
AVDCInit1::
		ld	a,#0x00
		out	(#0x39),a					;; master reset!
		
		;; delay
		call delay						;; master reset takes time...
		call delay						;; one delay is 255 x nop in a djnz loop 
		call delay
		
		;; set screen start 0 and 1 to 0
		xor a
		out (#0x3e),a
		out (#0x3f),a
		out (#0x3a),a
		out (#0x3b),a
		
		;; reset IR pointer to 0
		ld a,#0x10						
		out (#0x39),a
		
		ld hl,#ir_tbl					;; ir registers IR0-IR9
		ld b,#0x0a						;; 10 bytes
		ld c,#0x38						;; out port will be 0x38 i.e. IR initialization
		otir							;; and loop
		ret
		
ir_tbl:
		;; decoding IR table
		;; IR0 = 0xd0 (0b11010000) 
		;; bit 7 double width (yes)
		;; bits 6-3 scan lines per char row (10)
		;; bit 2 vsync/csync pulses (csync)
		;; bits 1-0 buffer mode select (00 - independent)
		.db 0xd0
		;; IR1 = 0x2f (0b00101111)
		;; bit 7 interlace (no)
		;; bits 6-0 equalizing const for csync (2f)
		.db 0x2f 
		;; IR2 = 0x0d (0b00001101)
		;; bit 7 row table mode enable (0)
		;; bits 6-3 horizontal sync pulse width (1)
		;; bits 2-0 horizontal back porch (5)
		.db 0x0d 
		;; IR3 = 0x05 (0b00000101)
		;; bits 7-5 vertical front porch (0)
		;; bits 4-0 vertical back porch (5)
		.db 0x05 
		;; IR4 = 0x99 (0b10011001)
		;; bit 7 char blink rate (1)
		;; bit 6-0 char rows per screen (25)
		.db 0x99 
		;; IR5 = 0x4f (0b01001111)
		;; bits 7-0 active chars per row (79)
		.db 0x4f 
		;; IR6 = 0x0a (0b00001010)
		;; bits 7-4 first scan line of cursor 0
		;; bits 3-0 last scan line of cursor 10
		.db 0x0a 
		;; IR7 = 0xea (0b11101010)
		;; bits 7-6 vertical sync pulse width (3)
		;; bit 5 cursor blink enable (1)
		;; bit 4 cursor rate (0 = 1/32)
		;; bits 3-0 underline position (10)
		.db 0xea 
		;; IR8 = 0x00 (0b00000000)
		;; bits 7-0 LSB of display buffer first address
		.db 0x00
		;; IR9 = 0x30 (0b00110000)
		;; bits 7-4 display buffer last address (8191)
		;; bits 3-0 display buffer first address msb (0)
		.db 0x30
		
AVDCInit2::
		;; show cursor
		ld a,#0x3d						; 0b00111101
		out (#0x39),a

		;; move cursor to 0,0
		xor a
		out (#0x3d),a
		out (#0x3c),a
		
		;; set display address
		ld hl,#0x1fff
		ld a,#0x1a						; 0b00011010
		out (#0x39),a					; select IR10
		;; write 8191 to display pointer address
		ld a,l
		out (#0x38),a					; ff to IR10 (display ptr lower address)
		ld a,h
		out (#0x38),a					; 1f to IR11 (display ptr upper address (31)
		
		;; now clear screen!
		ld a,#0x20
		out (#0x34),a					; use space chars!
		xor a							; a=0
		out (#0x35),a					; attribute is 0
		
		;; finally, write from cursor to pointer with spaces
		ld a,#0xbb
		out	(#0x39),a
		
		ret
		
## Links

### Xebecorama 

http://ftpmirror.your.org/pub/misc/bitsavers/pdf/xebec/
https://github.com/Anamon/pcem/blob/0a8b0ac50de7a089ac23007acbad2dfdffc75228/src/mfm_xebec.c
https://github.com/WildfireDEV/android_kernel_htc_m7/blob/25d8fee0f898d41eee3e57db47c184ccbaa0647d/drivers/block/xd.h
https://github.com/eunuchs/unix-archive/blob/a80c800a6288d8e613e057fae8313d532836598f/PDP-11/Trees/2.11BSD/usr/src/sys/OTHERS/scsi2/README
https://github.com/eunuchs/unix-archive/blob/a80c800a6288d8e613e057fae8313d532836598f/PDP-11/Trees/2.11BSD/usr/src/sys/OTHERS/scsi2/xe.c

## Floppy dissasembly

		push af
wait:
		in a,(#0xf0)					; disk controller read port 0
		and #0xc0						; get bits 0b11000000
		cp #0x80						; test if bit 7 is set AND bit 6 is not
		jp nz,wait
		pop af
		out (#0xf1),a					; disk controler write port 1 (reset pulse!), a=irelevant
		ret 


# Notes from 06. apr 2021

## Thomson code analysis in bios

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
        out (#0x21),a                   ; select pen, pen down

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

## Z80 SIO galore

https://github.com/dcollens/ZED-80/blob/523859c66f22f01e962289b1a9ae0e3ca3ac6167/emulator/src/io_sio.mm
https://github.com/BirchJD/advancemame-0.106.1-RPi/blob/43064d73e5e7a28dc6a76922969adfd0ca5b6c86/src/machine/z80sio.h
https://github.com/SHARPENTIERS/EmuZ-700-1500/blob/19ede7e09588ab368312abb08559b2beb84f473a/source/src/vm/z80sio.cpp
https://github.com/KailoKyra/Clay/tree/faa3142fb87a76f54cedc509b8e02e306cea1c39/rom
https://github.com/pdzion/fc-100/blob/a29f1a048c1cee65c18519bbf5290c45cd86d88b/source/src/vm/mz2500/mouse.cpp
https://github.com/DSkywalk/gmame-custom-0.178/blob/bdeba7e98124d5ba19b8e119419bbd6f203986d0/src/devices/machine/z80sio.cpp
https://github.com/anchorz/z80-uart/tree/main/src
https://github.com/anchorz/jkcemu/tree/master/src/z80emu
