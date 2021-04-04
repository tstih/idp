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

## Shared memory area

From 0xC000-0xffff (49152-65535=16384) upwards. This is also standard CP/M bank configuration.
(see: https://obsolescence.wixsite.com/obsolescence/cpm-internals)

## Graphics

	; (predpostavljam, da sta GDP X in Y registra že nastavljena)
.wait1:
	in a, (2Fh)	; GDP status port
	and 4		; Ready flag
	jr z, .wait1
	ld a, 3		; Pen up
	out (20h), a
	ld c, 0		; Sem bodo šli prebrani piksli
	ld b, 8
.wait2:
	in a, (2Fh)
	and 4
	jr z, .wait2
	ld a, 0Fh	; Ukaz za branje piksla
	out (20h), a
.wait3:
	in a, (2Fh)
	and 4
	jr z, .wait3
	in a, (36h)
	rla		; CF := piksel (0, če je nastavljen!)
	rl c
	ld a, 0A0h	; "Nariši" črto, delta X = 1, delta Y = 0
	out (20h), a
	djnz .wait2

Glede na to, da ukaz 3 (pen up) samo spremeni en bit v registru, morda zanka .wait2 ni potrebna; to je treba poskusiti.

Takole bi pa najbrž lahko povrnil 8 vodoravno zaporednih pikslov naenkrat:

	; (predpostavljam, da C vsebuje prej prebrane piksle,
	; da sta GDP X in Y registra že nastavljena, in da je
	; bil ukaz "pen up" že izstavljen)
	ld b, 8
.loop:
	rl c		; CF := piksel (0, če je nastavljen)
	jr c, .wait4	; Če ni, preskočimo pen down in risanje piksla
.wait1:
	in a, (2Fh)
	and 4		; Ready flag
	jr z, .wait1
	rrca		; A := 2 (pen down)
	out (20h), a
.wait2:
	in a, (2Fh)
	and 4
	jr z, .wait2
	ld a, 080h	; Nariši črto, delta X = 0, delta Y = 0
	out (20h), a
.wait3:
	in a, (2Fh)
	and 4
	jr z, .wait3
	ld a, 3		; Pen up
	out (20h), a
.wait4:
	in a, (2Fh)
	and 4
	jr z, .wait4
	ld a, 0A0h	; Nariši črto, delta X = 1, delta Y = 0
	out (20h), a
	djnz .loop

Najbrž bo delovalo hitreje, če to delaš na neaktivni strani. In seveda ti ni treba brati/povračati pikslov, ki jih kazalec ne prekriva.

> b) Kako skriti nadležnega tekstovnega utripača (kurzor).

Ne vem, ali obstaja escape sekvenca, lahko pa tako, da zapišeš 30h na port 39h. In 31h, da ga nazaj vklopiš. Pred izstavljanjem ukaza moraš počakati, da AVDC ni zaseden:

.waitAccess:
	in a, (36h)
	and 10h		; AVDC access flag
	jr nz, .waitAccess
.waitReady:
	in a, (39h)
	and 20h		; Ready flag
	jr z, .waitReady





fix


	; (predpostavljam, da C vsebuje prej prebrane piksle,
	; da sta GDP X in Y registra že nastavljena, in da HL
	; vsebuje isto vrednost kot GDP X register)
	ld b, 8
.loop:
	xor a
	rl c
	rla		; A := "eraser", če piksel ni nastavljen, drugače "pen"
.wait1:
	in d, (2Fh)
	bit 2, d	; Ready flag
	jr z, .wait1
	out (20h), a
.wait2:
	in a, (2Fh)
	and 4
	jr z, .wait2
	ld a, 080h	; Nariši/pobriši črto, delta X = 0, delta Y = 0
	out (20h), a
	inc hl
.wait3:
	in a, (2Fh)
	and 4
	jr z, .wait3
	out (28h), h
	out (29h), l
	djnz .loop


*** IN 2F (GDP status (no interrupt reset)) = F
*** OUT 28 (GDP Xpos MSBs) = 3
*** OUT 29 (GDP Xpos LSBs) = 0
*** OUT 2A (GDP Ypos MSBs) = 3
*** OUT 2B (GDP Ypos LSBs) = 0
*** OUT 20 (GDP status/command) = F
*** IN 2F (GDP status (no interrupt reset)) = D
*** IN 36 (Common input/Graphic scroll) = DE
*** OUT 28 (GDP Xpos MSBs) = 3
*** OUT 29 (GDP Xpos LSBs) = 0
*** OUT 2A (GDP Ypos MSBs) = 2
*** OUT 2B (GDP Ypos LSBs) = FF
*** OUT 20 (GDP status/command) = F
*** IN 2F (GDP status (no interrupt reset)) = D
*** IN 36 (Common input/Graphic scroll) = CE
*** OUT 28 (GDP Xpos MSBs) = 3
*** OUT 29 (GDP Xpos LSBs) = 0
*** OUT 2A (GDP Ypos MSBs) = 2
*** OUT 2B (GDP Ypos LSBs) = FE
*** OUT 20 (GDP status/command) = F
*** IN 2F (GDP status (no interrupt reset)) = D
*** IN 36 (Common input/Graphic scroll) = DF

