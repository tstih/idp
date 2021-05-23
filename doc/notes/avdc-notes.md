# Notes from 05. apr 2021

## Understanding the AVDC 

Here is complete AVDC initialization routine.

~~~
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
~~~


