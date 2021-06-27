		;; ef9367.s
        ;; 
        ;; a library of primitives for the thompson ef9367 card (GDP)
        ;; 
        ;; TODO: 
        ;;  - cache drawing modes and pen up/down status to 
        ;;    avoid multiple writes to registers 
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
        .globl  _test
	    
		.include "ef9367.inc"

        .area	_CODE

        
        ;; TODO: proxy function for tests
_test::
        ret


        ;; ------------------
		;; void ef9367_init()
        ;; ------------------
        ;; initializes the ef9367, sets the 1024x512 graphics mode
        ;; affect:  a, b, flags
_ef9367_init::
        xor a
        out (#EF9367_CR1),a             ;; control reg 1 to default
        out (#EF9367_CR2),a             ;; control reg 2 to default
        out (#EF9367_CH_SIZE),a         ;; scaling to none
        ld a,#PIO_GR_CMN_1024x512       ;; resolution to 1024x512
		out (#PIO_GR_CMN),a
        ld b,#EF9367_DWM_SET            ;; default drawing mode to SET 
        call ef9367_set_dmode
        ret


        ;; ------------------------------------
		;; void ef9367_xy(int16_t x, int16_t y)
        ;; ------------------------------------
        ;; move the cursor to x,y
        ;; notes:   y is transformed from bottom to top to top to bottom coord.
		;;          using the EF9367_HIRES_HEIGHT const.
        ;; affect:  af, de, hl
_ef9367_xy::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp
        call xy_internal                ; call internal implementation of ix
        ;; restore regs
        pop ix
		ret
xy_internal:
        ;; make sure GDP is free
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,xy_internal
        ;; there is additional call on stack hence
        ;; first argument is at 2(ix)
        ld a,(ix)
        out (#EF9367_XPOS_LO),a
        ld a,1(ix)
        out (#EF9367_XPOS_HI),a
        ld hl,#EF9367_HIRES_HEIGHT
        dec hl                          ; hl=max y
        ld e,2(ix)                      ; de=y
        ld d,3(ix)      
        sbc hl,de                       ; hl=max y-y
        ld a,l
        out (#EF9367_YPOS_LO),a
        ld a,h
        out (#EF9367_YPOS_HI),a
        ret


        ;; ------------------------------------------------------
		;; void ef9367_put_pixel(int16_t x, int16_t y, byte mode)
        ;; ------------------------------------------------------
        ;; draw pixel at x,y
        ;; affect:  af, de, hl
_ef9367_put_pixel::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp
        ;; internal set xy
        call xy_internal
        ;; set mode
        ld b,4(ix)
        call ef9367_set_dmode   
        ;; draw pixel!
        ld a,#0b10000000
        call ef9367_cmd
        ;; restore ix!
        pop ix
		ret


        ;; -----------------
		;; void ef9367_cls()
        ;; -----------------
		;; clear graphic screen
        ;; affect:  af
_ef9367_cls::
		ld a,#EF9367_CMD_CLS
		call ef9367_cmd
        ret



        ;; -----------------------
		;; void ef9367_put_raster(
        ;;     uint8_t *raster,
        ;;     uint8_t stride,
        ;;     uint16_t x, 
        ;;     uint16_t y, 
        ;;     uint8_t width,
        ;;     uint8_t height,
        ;;     uint8_t mode);  
        ;; -----------------------
		;; puts raster on screen fast
        ;; affect:  -
_ef9367_put_raster::
        ret


        ;; ----------------------
		;; void ef9367_draw_line(
        ;;     uint16_t x0, 
        ;;     uint16_t y0, 
        ;;     uint16_t x1,
        ;;     uint16_t y1,
        ;;     uint8_t mode,
        ;;     uint8_t mask);  
        ;; ----------------------
		;; draws line fast
        ;; affect:  -
_ef9367_draw_line::
        ;; store ix to stack, we'll use it to access args.
        push ix
        ld ix,#4                        ; first arg.
        add ix,sp
        ;; goto xy
        call xy_internal
        ;; y0 to de
        ld e,2(ix)                      ; de=y0
        ld d,3(ix)
        ;; find delta signs and mex line len
        ld a,#0x11                      ; a will hold the deltas
        or a                            ; clear carry flag
        ld l,6(ix)                      ; hl=y1
        ld h,7(ix)
        push hl                         ; store y1.
        sbc hl,de                       ; hl=y1-y0-c (C=0)
        ;; note: partner has reverse y axis
        jr c, dli_negat_dy              ; y1<y0, no change to delta sign
        pop de                          ; clean the stack (remove y1)
        ;; set flag (remember, reverse y axis!)
        or #4                           ; set flag (bit 2 of a)
        jr dli_dy_done                  ; we're done 
dli_negat_dy:
        pop hl                          ; hl=y1 (again)
        ex de,hl                        ; reverese equation
        sbc hl,de                       ; and make result positive
        inc hl                          ; +1, because of carry
dli_dy_done:
        ex de,hl                         ; de=abs(y1-y0)
        ;; start dx calculation
        ld c,(ix)                       ; bc=x0
        ld b,1(ix)
        ld l,4(ix)                      ; hl=x1
        ld h,5(ix)
        pop ix                          ; restore ix forever to cln. stack
        push de                         ; store abs(y1-y0) to stack
        push hl                         ; store the x1 
        or a                            ; clear carry flag
        sbc hl,bc                       ; hl=x1-x0
        jr nc,dli_posit_dx              ; x1>=x0, sign 0 is ok    
        or #2                           ; set bit 1 of delta to -, C=0
        pop de                          ; de=x1       
        push bc                         ; bc to hl
        pop hl                          ; hl=x0
        sbc hl,de                       ; hl=abs(x0-x1)
        jr dli_dx_done    
dli_posit_dx:
        pop de                          ; clean the stack (remove x1)
dli_dx_done:
        ;; hl = abs(x1-x0) and abs(y1-y0) is already on stack
        pop de                          ; de=abs(y1-y0)
        ;; but push back for later
        push de
        push hl                         ; both distances to stack
        ;; now find longer to find out how many lines 
        ;; hl=dx, de=dy
        or a                            ; clear carry
        sbc hl,de
        jr c,dli_dy_longer
        pop hl                          ; hl is the longer one
        push hl                         ; put it back
        jr dli_draw_lines
dli_dy_longer:
        ex de,hl                        ; move longer one to hl
dli_draw_lines:
        ;; store longer one to stack
        push hl
        ;; set mode
        push af                         ; store draw command
        ld b,8(ix)                      ; mode to b
        call ef9367_set_dmode
        pop af                          ; restore draw command
        ;; start the recursion. there are four parameters
        ;; on stack (in the pop order): longest coordinate, 
        ;; abs(dx), abs(dy), and return
dli_recursion:
        pop hl                          ; get longest coordinate
        push hl                         ; and store to stack for consistency
        ld de,#EF9367_MAX_DELTA         ; max line we can draw
        or a                            ; clear carry flag
        sbc hl,de                       ; commpare
        jr c, dli_draw_delta            ; end of recursion
        ;; we can't draw this
        ;; find mid point and divide 
        ;; the line to two lines        ;
        exx                             ; we'll need more registers
        pop de                          ; get longest coordinate into de
        push de
        pop hl                          ; AND into hl
        srl d                           ; de=long. coord/2
        rr e
        or a                            ; clear carry
        sbc hl,de                       ; hl=clong. oord-de (/2 for odd numbers!)
        exx
        ;; do the same for dx with std. register sets
        pop de                          ; de=dx
        push de
        pop hl                          ; AND into hl
        srl d                           ; de=dx/2
        rr e
        or a                            ; clear carry
        sbc hl,de                       ; hl=dx-de (/2 for odd numbers!)
        ;; and, finally, for dy use bc and bc' as storage!
        pop bc                          ; bc=dy
        push bc                         ; store back
        push hl                         ; store hl...
        exx 
        pop bc                          ; ...into alt bc
        exx
        pop hl                          ; hl=(also) dy
        srl b                           ; bc=dy/2
        rr c
        or a                            ; clear carry
        sbc hl,bc                       ; hl=dy-dy/2
        ;; we have it all!
        ;; only return address left on staqck at this point, leave it!
        ;; nicely put halved args back to stack in reverse order, longer first!
        push hl                         ; dy2
        exx
        push bc                         ; dx2
        push hl                         ; longer coord. 2
        exx
        ;; now the shorter line
        ld hl,#dli_recursion            ; restart the recursion upon return
        push hl
        push bc                         ; dy1
        push de                         ; dx1
        exx
        push de                         ; shorter longest coord.
        exx
        jr dli_recursion

dli_draw_delta:
        pop hl                          ; longest coord. ... discharge it
        pop hl                          ; hl=dx
        pop de                          ; de=dy
        ld b,l                          ; b=dx
        ld c,e                          ; c=dy 
        ;; superfast line drawing (delta method)
        push af                         ; store command
dli_wait_gdp:
        ;; wait for GDP
        in a,(#EF9367_STS)
        and #EF9367_STS_READY
        jr z,dli_wait_gdp
        ;; set deltas!
        ld a,b
        out (#EF9367_DX),a
dli_wait_gdp2:
        ;; wait for GDP
        in a,(#EF9367_STS)
        and #EF9367_STS_READY
        jr z,dli_wait_gdp2
        ld a,c
        out (#EF9367_DY),a
        pop af
        ;; command is in a
        call ef9367_cmd
        ret


        ;; executes ef9367 command & wait for status
        ;; input:	a=command
ef9367_cmd:
        push af
cmd_wait_sts:
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,cmd_wait_sts
        pop af
        out (#EF9367_CMD), a            ; exec. command
        ret


        ;; set drawing mode, uses bits:
        ;;  bit 0: 1=set, 0=clr
        ;;  bit 1: 1=xor, 0=normal
        ;;  0   ... clr
        ;;  1   ... set
        ;;  2   ... xor (or with clr or set)
        ;; input:   b = mode
        ;; affects: af, hl, bc, byte@sdm_cache
ef9367_set_dmode:
        ;; wait for GDP
        call wait_for_gdp
        ;; ready!
        ld a,(#sdm_cache)               ; get cached value
        cp b                            ; compare to current mode
        ret z                           ; all done!
        ld a,b                          ; store new mode
        ld (#sdm_cache),a               ; to cache
        and #EF9367_DWM_XOR             ; xor?
        jr z,sdm_not_xor
        ;; set xor
        in a,(#PIO_GR_CMN)              ; get current reg. to a
        or #PIO_GR_CMN_XOR_MODE         ; set xor bit
        out (#PIO_GR_CMN),a             ; write it back
        jr sdm_test_set
sdm_not_xor:
        ;; reset xor
        in a,(#PIO_GR_CMN)              ; get current reg. to a
        and #~PIO_GR_CMN_XOR_MODE       ; clr xor bit
        out (#PIO_GR_CMN),a             ; write it back
        ;; proceed with test
        ;; remember: xor can be ORed!
sdm_test_set:
        ld a,b                          ; get parameter once again!
        and #EF9367_DWM_SET
        jr z,sdm_is_clr
        ld a,#EF9367_CMD_DMOD_SET       ; mode to SET     
        call ef9367_cmd
        jr sdm_done
sdm_is_clr:
        ld a,#EF9367_CMD_DMOD_CLR       ; mode to CLR     
        call ef9367_cmd
sdm_done:
        ;; now put pen down (default!)
        ld a,#EF9367_CMD_PEN_DOWN
        call ef9367_cmd
        ret
sdm_cache:
        .db 0xff                        ; default mode is undefined (0xff)


        ;; wait for the GDP to finish 
        ;; previous operation
        ;; affects: a
wait_for_gdp:
        ;; make sure GDP is free
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,wait_for_gdp
        ret