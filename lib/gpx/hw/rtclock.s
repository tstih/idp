		;; rtclock.s
        ;; 
        ;; a library of primitives for id partners real time clock
        ;;
        ;; MIT License (see: LICENSE)
        ;; copyright (c) 2021 tomaz stih
        ;;
		;; 03.04.2021   tstih
        .module rtclock

		.globl 	_rtc_read
        .globl	_rtc_write


        ;; all ports are R/W and in packed BCD format
        .equ    RTC_SEC,            0xa2
        .equ    RTC_MIN,            0xa3
        .equ    RTC_HR24,           0xa4
        .equ    RTC_DAY,            0xa6
        .equ    RTC_MONTH,          0xa7
        .equ    RTC_YEAR99,         0xa9

        .area	_CODE


        ;; ----------------------------------
		;; datetime_t* rtc_read(datetime_t*);
        ;; ----------------------------------
        ;; read the real time clock
		;; input:	-
		;; output:	-
        ;; affect:  hl (ret value)
_rtc_read::
        push af
        push bc
        push de

        ;; first get to the parameters
        ld hl,#0x08                     ; skip af,bc.de and ret. addr
        add hl,sp
        ld e,(hl)                       ; get the pointer into de
        inc hl
        ld d,(hl)
        ex de,hl                        ; and into hl
        push hl                         ; store this pointer (ret value)

        ;; read seconds
        in a,(#RTC_SEC)                 ; seconds into a
        call bcd2bin
        ;; set second of datetime_t
        ld (hl),a               
        inc hl

        ;; read minutes
        in a,(#RTC_MIN)                 ; minutes into a
        call bcd2bin
        ;; set minute of datetime_t
        ld (hl),a               
        inc hl

        ;; read hours
        in a,(#RTC_HR24)                ; hours into a
        call bcd2bin
        ;; set hour of datetime_t
        ld (hl),a               
        inc hl

        ;; read day
        in a,(#RTC_DAY)                 ; day of month into a
        call bcd2bin
        ;; set day of datetime_t
        ld (hl),a               
        inc hl

        ;; read month
        in a,(#RTC_MONTH)               ; month of year into a
        call bcd2bin
        ;; set month of datetime_t
        ld (hl),a               
        inc hl

        ;; read year
        in a,(#RTC_YEAR99)              ; 2 digit year into a
        call bcd2bin
        ;; add 2000
        ex de,hl                        ; store hl to de
        ld hl,#2000                     ; year is 2k (decimal!)
        ld b,#0                         ; bc=a
        ld c,#a
        add hl,bc                       ; hl has correct year
        ex de,hl                        ; de=year, hl=address
        ;; set year of datetime_t
        ld a,e
        ld (hl),a 
        inc hl
        ld a,d
        ld (hl),a
        inc hl

        pop hl                          ; correct return value!
        pop de
        pop bc
        pop af
        ret

        ;; ---------------------
		;; rtc_write(datetime_t*)
        ;; ---------------------
        ;; write the real time clock
		;; input:	-
		;; output:	-
        ;; affect:  -
_rtc_write::
        push af
        push bc
        push de
        push hl

        ;; first get to the parameters
        ld hl,#0x0a                     ; skip af,bc,de,hl and ret. addr
        add hl,sp
        ld e,(hl)                       ; get the pointer into de
        inc hl
        ld d,(hl)
        ex de,hl                        ; and into hl

        ;; write seconds to rtc
        ld a,(hl)                       ; a=sec
        call bin2bcd
        out (#RTC_SEC),a
        inc hl

        ; write minutes to rtc
        ld a,(hl)                       ; a=min
        call bin2bcd
        out (#RTC_MIN),a
        inc hl

        ; write hour to rtc
        ld a,(hl)                       ; a=hour
        call bin2bcd
        out (#RTC_HR24),a
        inc hl

        ; write day to rtc
        ld a,(hl)                       ; a=day
        call bin2bcd
        out (#RTC_DAY),a
        inc hl

        ; write month to rtc
        ld a,(hl)                       ; a=month
        call bin2bcd
        out (#RTC_MONTH),a
        inc hl

        ; write year to rtc
        ld e,(hl)                       ; de=year
        inc hl
        ld d,(hl)
        ld hl,#2000                     ; hl=2000
        ex de,hl                        ; hl=year,de=2000
        sbc hl,de                       ; hl=hl-2000
        ld a,l                          ; a has the year
        call bin2bcd
        out (#RTC_YEAR99),a

        pop hl                     
        pop de
        pop bc
        pop af
        ret


        ;; convert a number in acc to bcd
		;; input:	a=number
		;; output:	a=bcd number
        ;; affect:  af, bc
bin2bcd:
        ld	c, a
        ld	b, #0x08
        xor	a
b2bloop:
        sla	c
        adc	a, a
        daa
        djnz	.loop
        ret

        ;; convert a bcd number in acc to bin
		;; input:	a=bcd number
		;; output:	a=number
        ;; affect:  af, bc
bcd2bin:
        ld	c,a
        and	#0xf0
        srl	a
        ld	b,a
        srl	a
        srl	a
        add	a,b
        ld	b,a
        ld	a,c
        and	#0x0f
        add	a,b
        ret