		;; glyph.s
        ;; 
        ;; a library of glyph utility routines
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 22.04.2021   tstih
        .module glyph

        .globl _gpy_query_glyph

        .equ    GPY_HDR_SIZE,   8

        .area _CODE

        ;; calculates glyphs width and position
        ;; input:	a=command, hl=font
		;; output:	a=glyph width, b=glyph height, hl=glyph address
        ;; affect:  af, bc, de, hl, hl', de', ix
_gpy_query_glyph::
        ;; first get ascii span
        ld c,a                  ; store a for later
        push hl
        pop ix                  ; ld ix,hl
        ld a,7(ix)              ; a=last ascii
        ld b,6(ix)              ; b=first ascii
        sub b                   ; a=number of characters-1
        inc a                   ; +1
        ld d,#0                 ; 
        ld e,a                  ; de=16 bit ascii span
        push de                 ; store it
        ;; now get and store the end of header
        ld bc,#GPY_HDR_SIZE     ; header size
        add hl,bc               ; hl points to table of glyph offsets
        push hl                 ; store end of header
        ;; get width
        ld a,c                  ; get original ascii back
        sub b                   ; b still has first ascii
        ld e,a                  ; d is still 0, de=index 
        add hl,de               ; get to pointer to glyph offset
        ld c,(hl)               ; get glyph offset into bc
        inc hl
        ld b,(hl)               
        ;; jump over table of offsets
        ;; at this point
        ;; de=index, bc=glyph offset 
        ;; stack=tbl of offsets. 16 bit ascii span
        ;; get the width
        exx                     ; alter set
        pop hl                  ; start of tbl. of offsets
        pop de                  ; start of ascii span
        push de                 ; we'll need ascii span once more
        sla e                   ; de=de*2
        rl d
        add hl,de               ; hl now points to table of widths
        push hl                 ; store it
        push hl                 ; twice
        exx                     ; back to reality
        ;; now get the width...
        pop hl                  ; hl is table of width, bc is index
        add hl,bc               ; hl points to correct glyph
        ld a,(hl)               ; a is width
        ;; and point hl to start of glyph data
        pop hl                  ; hl points to table of widths (again)
        pop de                  ; de=ascii span
        add hl,de               ; hl=start of glyph data
        add hl,bc               ; plus offset
        ;; and glyph height into b
        ld b,3(ix)
        ;; we're done!
        ret