		;; scn2674.inc
        ;; 
        ;; signetics SCN2674 advanced video display controller (AVDC)
		;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 04.04.2012   tstih

        
        ;; --- 0x34 - R/W:character register ----------------------------------
        .equ SCN2674_CHR,               0x34

        
        ;; --- 0x35 - R/W:attribute register ----------------------------------
        .equ SCN2674_AT,                0x35
        .equ SCN2674_AT_NONE,           0x00    ; no attributes
        .equ SCN2674_AT_BLINK,          0x01    ; blink
        .equ SCN2674_AT_UNDERLINE,      0x02    ; underline
        .equ SCN2674_AT_SPC_CHR,        0x04    ; special character
        .equ SCN2674_AT_PROTECT,        0x08    ; protect
        .equ SCN2674_AT_HIGHLIGHT,      0x10    ; R:highlight, W:red foreground
        .equ SCN2674_AT_REVERSE,        0x20    ; R:reverse, W:green background
        .equ SCN2674_AT_GP2,            0x40    ; R:GP 2, W:blue background
        .equ SCN2674_AT_GP1,            0x80    ; R:GP 1, W:red background


        ;; --- 0x36 - W:gr.scroll (byte value=scan lines), R:common input -----
        .equ SCN2674_GR_SCROLL,         0x36

        .equ SCN2674_GR_CMNI,           0x36    ; same register, different name
        .equ SCN2674_GR_CMNI_SCN2674A,  0x10    ; SCN2674 access flag
        .equ SCN2674_GR_CMNI_PIX,       0x80    ; graph. pix input


        ;; --- 0x38 - W:init (all 15 init reg. sequentially accessed) --------- 
        ;; R:interrupt (bits 7-6 default 0)
        .equ SCN2674_INIT,              0x38
        .equ SCN2674_IR,                0x38    ; same register, different name
        .equ SCN2674_IR_SS2,            0x01    ; split screen 2 interrupt
        .equ SCN2674_IR_RDY,            0x02    ; ready interrupt
        .equ SCN2674_IR_SS1,            0x04    ; split screen 1 interrupt
        .equ SCN2674_IR_LZ,             0x08    ; line zero interrupt
        .equ SCN2674_IR_VB,             0x10    ; vblank int


        ;; --- 0x39 - W:command (byte is command) -----------------------------
        ;; R:status (bits 7-5 default 0)
        .equ SCN2674_CMD,               0x39    
        .equ SCN2674_CMD_RESET,         0x00    ; master reset
        .equ SCN2674_CMD_SET_IR,        0x10    ; set IR pointer to lower nibble
        .equ SCN2674_CMD_CURS_OFF,      0x30    ; switch off cursor
        .equ SCN2674_CMD_CURS_ON,       0x31    ; switch on cursor
        .equ SCN2674_CMD_WC2P,          0xbb    ; write from cursor to pointer
        .equ SCN2674_CMD_WAC,           0xab    ; write at cursor

        .equ SCN2674_STS,               0x39    ; same register, different name    
        .equ SCN2674_STS_SS2,           0x01    ; split screen 2 interrupt
        .equ SCN2674_STS_RDYI,          0x02    ; ready interrupt
        .equ SCN2674_STS_SS1,           0x04    ; split screen 1 interrupt
        .equ SCN2674_STS_LZ,            0x08    ; line zero interrupt
        .equ SCN2674_STS_VB,            0x10    ; vblank int
        .equ SCN2674_STS_RDY,           0x20    ; ready flag


        ;; --- 0x3a - R/W: screen start 1 lower register ----------------------
        .equ SCN2674_SS1_LO,            0x3a
        ;; --- 0x3b - R/W: screen start 1 upper register ----------------------
        .equ SCN2674_SS1_HI,            0x3b


        ;; --- 0x3c - R/W: cursor address lower register ----------------------
        .equ SCN2674_CUR_LO,            0x3c
        ;; --- 0x3d - R/W: cursor address upper register ----------------------
        .equ SCN2674_CUR_HI,            0x3d


        ;; --- 0x3e - R/W: screen start 2 lower register ----------------------
        .equ SCN2674_SS2_LO,            0x3e
        ;; --- 0x3f - R/W: screen start 2 upper register ----------------------
        .equ SCN2674_SS2_HI,            0x3f