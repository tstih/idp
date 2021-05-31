		;; ef9367.s
        ;; 
        ;; a library of primitives for the thompson ef9367 card (GDP)
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 04.04.2021    tstih
		.module ef9367

		.globl	_ef9367_init
		.globl 	_ef9367_cls
        .globl	_ef9367_xy
        .globl  _ef9367_put_pixel
        .globl  _ef9367_put_raster
        .globl  _ef9367_draw_line
	    
		.include "ef9367.inc"

        .area	_CODE

        ;; ------------------
		;; void ef9367_init()
        ;; ------------------
        ;; initializes the ef9367, sets the 1024x512 graphics mode
		;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_init::
        push af
        xor a
        out (#EF9367_CR1),a             ;; control reg 1 to default
        out (#EF9367_CR2),a             ;; control reg 2 to default
        out (#EF9367_CH_SIZE),a         ;; scaling to none
        ld a,#PIO_GR_CMN_1024x512       ;; resolution to 1024x512
		out (#PIO_GR_CMN),a
        pop af
		ret


        ;; ------------------------------------
		;; void ef9367_xy(int16_t x, int16_t y)
        ;; ------------------------------------
        ;; move the cursor to x,y
        ;; notes:   y is transformed from bottom to top to top to bottom coord.
		;;          using the EF9367_HIRES_HEIGHT const.
        ;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_xy::
        ;; store regs
        push af
        push bc
        push de

        ld hl,#0x0a                     ; hl=sp+10 (skip regs and ret addr)
        add hl,sp                       ; hl points to the first parameter
        
        ;; get x and y into bc=x, de=y
        ld c,(hl)
        inc hl
        ld b,(hl)
        inc hl
        ld e,(hl)
        inc hl
        ld d,(hl)
        inc hl

        call ef9367_xy_dirty            ; first go to x,y

        ;; restore regs
        pop de
        pop bc
        pop af

		ret


        ;; -------------------------------------------
		;; void ef9367_put_pixel(int16_t x, int16_t y)
        ;; -------------------------------------------
        ;; draw pixel at x,y
        ;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_put_pixel::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp

        ;; get x and y into bc=x, de=y
        ld c,(ix)
        ld b,1(ix)
        ld e,2(ix)
        ld d,3(ix)
        call ef9367_xy_dirty            ; first go to x,y

        ;; pen down...
        ld a, #(EF9367_CR1_PENDOWN+EF9367_CR1_PEN)
        out (#EF9367_CR1),a

        ;; and draw pixel!
        ld a,#0b10000000                ; #0b00010000
        call ef9367_call_cmd

        ;; restore regs
        pop ix

		ret




        ;; ---------------------------------------------------
		;; void ef9367_draw_line(
        ;;     int16_t x0, int16_t y0, int16_t x1, int16_t y1)
        ;; ---------------------------------------------------
        ;; draw fast line from x0, y0 to x1, y1
        ;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_draw_line::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp
        ;; store other regs
        push bc
        push de
        ;; get x0 and y0 to bc, de
        ld c,(ix)                       ; bc=x0
        ld b,1(ix)
        ld e,2(ix)                      ; de=y0
        ld d,3(ix)
        call ef9367_xy_dirty            ; first go to x,y
        ;; find delta signs and mex line len
        ld a,#0x11                      ; a will hold the deltas
        ld l,6(ix)                      ; hl=y1
        ld h,7(ix)
        push hl                         ; store y1.
        sbc hl,de                       ; hl=y1-y0-c (C=0)
        ;; note: partner has reverse y axis
        jr c, negative_dy               ; y1<y0, no change to delta sign
        pop de                          ; clean the stack (remove y1)
        ;; set flag (remember, reverse y axis!)
        or #4                           ; set flag (bit 2 of a)
        jr calc_dy_done                 ; we're done 
negative_dy:
        pop hl                          ; hl=y1 (again)
        ex de,hl                        ; reverese equation
        sbc hl,de                       ; and make result positive
        inc hl                          ; +1
calc_dy_done:
        push hl                         ; abs(y1-y0) to stack
        ;; start dy calculation
        ld l,4(ix)                      ; hl=x1
        ld h,5(ix)
        push hl                         ; store the x1 
        sbc hl,bc                       ; hl=x1-x0
        jr nc,positive_dx               ; x1>=x0, sign 0 is ok    
        or #2                           ; set bit 1 of delta to -, C=0
        pop de                          ; de=x1       
        push bc                         ; bc to hl
        pop hl                          ; hl=x0
        sbc hl,de                       ; hl=abs(x0-x1)
        jr calc_dx_done    
positive_dx:
        pop de                          ; clean the stack (remove x1)
calc_dx_done:
        ;; hl = abs(x1-x0) and abs(y1-y0) is already on stack
        pop de
        ;; but push back for later
        push de
        push hl                         ; both distances to stack
        ;; now find longer to find out how many lines 
        ;; hl=dx, de=dy
        sbc hl,de
        jr c,dy_longer
        pop hl                          ; hl is the longer one
        push hl                         ; put it back
        jr gen_lines
dy_longer:
        ex de,hl                        ; move longer one to hl
gen_lines:
        ;; find out how many lines we need?
        push hl
        ;; pen down, ready to draw
        ld b,a
        ld a,#EF9367_CMD_SEL_DOWN
        call ef9367_call_cmd
        ld a,#EF9367_CMD_SEL_PEN
        call ef9367_call_cmd
        ld a,b
        ; first test agains 2 * delta - 1
        ld de,#2*(EF9367_MAX_DELTA+1)
        sbc hl,de                       ; test against hl...
        jr nc,four_lines                ; de>=limit
        pop hl
        push hl
        ld de,#EF9367_MAX_DELTA         ; test delta
        sbc hl,de                       
        jr nc,two_lines
one_line:
        pop hl                          ; clean stack
        pop hl                          ; hl=dx
        pop de                          ; de=dy
        ld b,l                          ; b=dx
        ld c,e                          ; c=dy                  
        call draw_delta_line            ; and draw it...
        jr finalize
two_lines:
        pop hl                          ; clean stack
        pop hl                          ; hl=dx
        pop de                          ; de=dy
        ;; halve hl and de, reminder to bc
        push af                         ; store command 
        ld a,l                          ; get x coordinate
        and #1                          ; reminder (div by 2)
        ld b,a                          ; store to b
        ld a,e                          ; get y coordinate
        and #1                          ; reminder
        ld c,a                          ; store to c
        pop af                          ; restore command
        srl h                           ; hl=hl/2
        rr l
        srl d                           ; de=de/2
        rr e                       
        ;; first line length l.e, TODO: second l-remainder, e-remainder
        push bc                         ; store remainders
        ld b,l
        ld c,e
        call draw_delta_line
        pop hl                          ; remaindes to hl
        ld d,a                          ; store a
        ; if coord is 0 don't apply the reminder
        ld a,b
        or a
        jr z,skip2xr
        ld a,h                          ; x reminder to a 
        or a                            ; test for 0?
        jr nz, skip2xr                  ; not zero, no need for dec.
        dec b
skip2xr:
        ld a,d                          ; restore a
        call draw_delta_line
        jr finalize
four_lines:
        pop hl                          ; clean stack
        pop hl
        pop de
        ;; divide by 4
        srl h                           ; hl=hl/4
        rr l
        srl h
        rr l
        srl d                           ; de=de/4
        rr e 
        srl d
        rr e     
        ;; load deltas
        ld b,l
        ld c,e   
        call draw_delta_line
        call draw_delta_line
        call draw_delta_line
        call draw_delta_line
finalize:
        ;; restore regs
        pop de
        pop bc
        pop ix

		ret

        ;; superfast line drawing (delta method)
        ;; the routine assumes that the cursor is at 
        ;; the right position, because sign is separated
        ;; from the byte delta number, max delta can be
        ;; from -255 to 255.
        ;; input:	b=delta x
        ;;          c=delta y
        ;;          a=bit 0 sign for x, bit 1 sign for y
		;; output:	
        ;; affect:  -
draw_delta_line:
        push af
        ld a,b
        out (#EF9367_DX),a
        ld a,c
        out (#EF9367_DY),a
        pop af
        ;; command is already in a
        call ef9367_call_cmd
        ret


        ;; ------------------------------------------
		;; void ef9367_put_raster(
        ;;     uint8_t *raster,
        ;;     int16_t x, 
        ;;     int16_t y, 
        ;;     uint8_t width,
        ;;     uint8_t height)
        ;; ------------------------------------------
        ;; draws a raster bitmap at x,y
        ;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_put_raster::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp
        ;; switch the screen refresh off, all attributes off.
        ;ld a,#EF9367_CR1_WO 
        ;out (#EF9367_CR1),a
        ;; get coords.
        ld c,2(ix)                      ; bc=x
        ld b,3(ix)
        ld e,4(ix)                      ; de=y
        ld d,5(ix)
        call ef9367_xy_dirty            ; cursor to x,y
        ;; we'll draw left to right, then right to left
        ld c,6(ix)                      ; c=width
        ld b,7(ix)                      ; b=height
        ld l,(ix)                       ; hl=raster
        ld h,1(ix)
        ;; for each row
raster_row_loop:
        ;; store current height (b) and width (c)
        push bc
        ld b,c                          ; width to b
        ld c,#8                         ; bit counter
        ld a,(hl)                       ; first byte to a
raster_col_loop:
        rlca                            ; bit 8 to C
        push af                         ; we will lose a...
        jr nc,clrpix
        ;; pen down, fast drawing
        ld a, #(EF9367_CR1_PENDOWN+EF9367_CR1_PEN)
        out (#EF9367_CR1),a
        jr drawpix
clrpix:
        ; eraser down, fast drawing
        ld a, #(EF9367_CR1_PENDOWN)     
        out (#EF9367_CR1),a
drawpix:
        ld a,#0b10100000                ; draw pixel and move 1 right
		call ef9367_call_cmd            ; execute it
        pop af                          ; and a
        dec c                           ; c=c-1
        jr nz,nextcol
        ;; last pixel in this byte, take next byte
        ld c,#8
        inc hl
        ld a,(hl)
nextcol:
        djnz raster_col_loop
        ;; before looping make sure we don't destroy hl and a
        push af 
        ;; is there a remainder (stride?)
        ld a,c
        cp #8
        jr z, raster_newline
        inc hl
raster_newline:
        push hl
        ;; cursor to next line
        ld c,2(ix)                      ; bc=x
        ld b,3(ix)
        inc de                          ; y=y+1
        call ef9367_xy_dirty            ; position to next row
        ;; restore a and hl
        pop hl
        pop af
        ;; get row loop state and width back!
        pop bc
        ;; next row
        djnz raster_row_loop
        ;; screen refresh back on.
        ;xor a
        ;ld (#EF9367_CR1),a
        ;; restore stack.
        pop ix
        ret


        ;; -----------------
		;; void ef9367_cls()
        ;; -----------------
		;; clear graphic screen
		;; input:	-
		;; output:	-
        ;; affect:  -
_ef9367_cls::
        push af
		ld a,#EF9367_CMD_CLS
		jr ef9367_jr_cmd



        ;; executes ef9367 command & wait for status
        ;; notes:   push af to stack and jr to this function to save 
        ;;          a pop af and ret call
        ;; input:	a=command, af is stored on the stack
		;; output:	af is restored from the stack
        ;; affect:  the stack
ef9367_jr_cmd:
        call ef9367_wait_sts_ready      ; wait until ef9367 is ready
        out (#EF9367_CMD), a            ; exec. command
        pop af
        ret                             ; ret so that callers don't have to


        ;; executes ef9367 command & wait for status
        ;; input:	a=command
		;; output:	-
        ;; affect:  flags
ef9367_call_cmd:
        call ef9367_wait_sts_ready      ; wait until ef9367 is ready
        out (#EF9367_CMD), a            ; exec. command
        ret


        ;; wait until graphic card is ready for next command
		;; input:	-
		;; output:	-
		;; affect:	-
ef9367_wait_sts_ready:
		push af                         ; store affected reg
sts_not_ready:
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,sts_not_ready
        pop af
        ret


        ;; goto x,y
		;; input:	bc=x, de=y
		;; output:	-
		;; affect:	a, hl, flags
ef9367_xy_dirty:
        ;; bc=x, de=y
        ld a,c
        out (#EF9367_XPOS_LO),a
        ld a,b
        out (#EF9367_XPOS_HI),a
        ld hl,#EF9367_HIRES_HEIGHT
        dec hl                          ; hl=max y
        sbc hl,de                       ; hl=max y-y
        ld a,l
        out (#EF9367_YPOS_LO),a
        ld a,h
        out (#EF9367_YPOS_HI),a
        ret