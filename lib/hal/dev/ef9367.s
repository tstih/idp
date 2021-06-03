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
        ld hl,#100
        push hl
        ld hl,#200
        push hl
        call _ef9367_xy
        ret

        jp draw_delta_line




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
        ;; start dx calculation
        ld c,(ix)                       ; bc=x0
        ld b,1(ix)
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
        ;; store longer one to stack
        push hl
        ;; set mode
        push af                         ; store draw command
        ld b,8(ix)                      ; mode to b
        call ef9367_set_dmode
        pop af                          ; restore draw command
        ;; find out how many delta lines are required?
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
        jr finalize
four_lines:

finalize:
        ;; restore regs
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
ddl_wait_sts:
        ;; wait for GDP
        in a,(#EF9367_STS)
        and #EF9367_STS_READY
        jr z,ddl_wait_sts
        ;; set deltas!
        ld a,b
        out (#EF9367_DX),a
        ld a,c
        out (#EF9367_DY),a
        pop af
        ;; command is already in a
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
        ;; affect:  af, hl, bc, byte@sdm_cache
ef9367_set_dmode:
        ;; wait for GDP
        in a,(#EF9367_STS)              ; read the status register
        and #EF9367_STS_READY           ; get ready flag
        jr z,ef9367_set_dmode
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