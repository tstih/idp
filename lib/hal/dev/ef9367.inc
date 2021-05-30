		;; ef9367.inc
        ;; 
        ;; thompson ef9367 graphics display processor (EF9367) 
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 04.04.2021   tstih


        .equ    EF9367_HIRES_WIDTH,     1024
        .equ    EF9367_HIRES_HEIGHT,    512

        ;; --- 0x20 - W: command, R:the status register -----------------------
        .equ    EF9367_STS,             0x20    ; status reg. (no interrupt reset)
        .equ    EF9367_STS_LPEN,        0x01    ; light pen end
        .equ    EF9367_STS_VBLANK,      0x02    ; vertical blanking
        .equ    EF9367_STS_READY,       0x04    ; ready
        .equ    EF9367_STS_POD,         0x08    ; pen out of display
        .equ    EF9367_STS_LPI,         0x10    ; light pen irq
        .equ    EF9367_STS_VBI,         0x20    ; vb irq
        .equ    EF9367_STS_RDYI,        0x40    ; ready irq
        .equ    EF9367_STS_IRQ,         0x80    ; IRQ

		.equ	EF9367_CMD,             0x20    ; command register (W)
        .equ    EF9367_CMD_SEL_PEN,     0x00    ; select pen (set bit 1 of CR1)
        .equ    EF9367_CMD_SEL_ERS,     0x01    ; select eraser (clr bit 1 of CR1)
        .equ    EF9367_CMD_SEL_DOWN,    0x02    ; pen/eraser down
        .equ    EF9367_CMD_SEL_UP,      0x03    ; pen/eraser up
        .equ    EF9367_CMD_CLS,         0x04    ; clear screen
        .equ    EF9367_CMD_00,          0x05    ; x=y=0
        .equ    EF9367_CMD_CLS00,       0x06    ; clear screen, x=y=0
        .equ    EF9367_CMD_CLEAR,       0x07    ; cls, CSIZE to min., all regs to 0
        .equ    EF9367_CMD_LP_INIT_W,   0x08    ; light pen init, WHITE to low
        .equ    EF9367_CMD_LP_INIT,     0x09    ; light pen init
        .equ    EF9367_CMD_5x8,         0x0a    ; set 5x8 block drawing (CSIZE)
        .equ    EF9367_CMD_4x4,         0x0b    ; set 4x4 block drawing (CSIZE)
        .equ    EF9367_CMD_SCAN,        0x0c    ; screen scan
        .equ    EF9367_CMD_X0,          0x0d    ; x=0
        .equ    EF9367_CMD_Y0,          0x0e    ; y=0
        .equ    EF9367_CMD_DMEM,        0x0f    ; direct memory access 4 next cyc.


        ;; --- 0x21 - RW: control register, bit 7 is reserved (always 0) ------
        .equ    EF9367_CR1,             0x21
        .equ    EF9367_CR1_PENDOWN,     0x01    ; 1 pen down, 0 pen up
        .equ    EF9367_CR1_PEN,         0x02    ; 1 pen, 0 eraser
        .equ    EF9367_CR1_WO,          0x04    ; 1 write only
        .equ    EF9367_CR1_CYCLIC,      0x08    ; cyclic screen if pen out...
        .equ    EF9367_CR1_LPEN_ENBLE,  0x10    ; enable light pen irq
        .equ    EF9367_CR1_VB_ENBLE,    0x20    ; enable vblank
        .equ    EF9367_CR1_RDY_ENBLE,   0x40    ; enable ready irq   


        ;; --- 0x22 - RW: control register 2, bit 7-4 are not used ------------
        .equ    EF9367_CR2,             0x22
        ;; bits 1 and 2 are vector type
        .equ    EF9367_CR2_SOLID,       0b00    ; solid vector type
        .equ    EF9367_CR2_DOTTED,      0b01    ; dotted vector type
        .equ    EF9367_CR2_DASHED,      0b10    ; dashed
        .equ    EF9367_CR2_DOT_DASH,    0b11    ; dot and dash pattern*/  


        ;; --- 0x23 - RW: character size --------------------------------------
        ;; low nibble is scaling on x, high nibble is scaling on y)
        .equ    EF9367_CH_SIZE,         0x23


        ;; --- 0x25 - RW: delta x ---------------------------------------------
        .equ    EF9367_DX,              0x25
        ;; --- 0x27 - RW: delta y ---------------------------------------------
        .equ    EF9367_DY,              0x27


        ;; --- 0x28 - RW: x pos hi --------------------------------------------
        .equ    EF9367_XPOS_HI,         0x28
        ;; --- 0x29 - RW: x pos low -------------------------------------------
        .equ    EF9367_XPOS_LO,         0x29
        ;; --- 0x2a - RW: y pos hi --------------------------------------------
        .equ    EF9367_YPOS_HI,         0x2a
        ;; --- 0x2b - RW: y pos low -------------------------------------------
        .equ    EF9367_YPOS_LO,         0x2b


        ;; --- 0x2c - R: xlp --------------------------------------------------
        .equ    EF9367_XLP,             0x2c    ; x light pen
        ;; --- 0x2d - R: ylp --------------------------------------------------
        .equ    EF9367_YLP,             0x2d    ; y light pen


        ;; --- 0x2f - R:the status (NI) register and its bitmask --------------
        .equ    EF9367_STS_NI,          0x2f    ; status reg. (no interrupt reset)
        .equ    EF9367_STS_NI_LPEN,     0x01    ; low when executing light pen seq.
        .equ    EF9367_STS_NI_VBLANK,   0x02    ; high during vertical blanking.
        .equ    EF9367_STS_NI_READY,    0x04    ; low when executing command
        .equ    EF9367_STS_NI_OUTSCR,   0x08    ; high when x or y is out of screen     


        ;; --------------------------------------------------------------------
        ;; TODO: understand why these are here for Iskra Delta Partner
        ;; --------------------------------------------------------------------

        ;; --- 0x30 - R/W:PIO gr. common register -----------------------------
        .equ    PIO_GR_CMN,             0x30
        .equ    PIO_GR_CMN_DISP_PG,     0x01    ; display page of memory
        .equ    PIO_GR_CMN_WR_PG,       0x02    ; write page of memory
        .equ    PIO_GR_CMN_WR_MODE,     0x04    ; write mode (xor/nor)
        ;; bits 3-4 are resolution
        .equ    PIO_GR_CMN_1024x512,    0x18    ; hires        
        .equ    PIO_GR_CMN_EF9367I,     0x20    ; RO:EF9367 interrupt 
        .equ    PIO_GR_CMN_AVDINT,      0x40    ; RO:avd interrupt
        .equ    PIO_GR_CMN_SCRL_MDE,    0x80    ; scroll mode

        ;; --- 0x32 - RW:PIO common text attributes ---------------------------
        .equ    PIO_GR_AT,              0x32
        .equ    PIO_GR_AT_DOTS,         0x01    ; dot stretch
        .equ    PIO_GR_AT_CURS_MODE,    0x02    ; cursor mode
        .equ    PIO_GR_AT_COLOR,        0x04    ; mono / color
        .equ    PIO_GR_AT_BLUE,         0x08    ; force background (blue fore)
        .equ    PIO_GR_AT_REVERSE,      0x10    ; reverse screen (green fore)
        .equ    PIO_GR_AT_DOTS_C0,      0x20    ; dots / char 0
        .equ    PIO_GR_AT_DOTS_C1,      0x40    ; dots / char 1
        .equ    PIO_GR_AT_CLOCK,        0x80    ; 24/16 Mhz text video clock
