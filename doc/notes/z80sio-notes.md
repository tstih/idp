# Notes from 18. apr 2021

## SIO initialization in ROM

Here's the SIO initialization code from IDP ROM.

~~~
;; keyboard SIO initialization
        ld	c,#0xd9     ; 0xd9 is SIO port 1 control register 
        ld	hl,init_sio
        ld	b,#7
        otir

init_sio:	
        .db	
        ;; 0b00011000 channel reset command
        0x18            
        ;; 0b00000100    ... next byte goes to WR4
        ,0x04
        ;; configure for N-1 and x16 clock mode 
        ;; bits 7-6 ... 01 = x16 clock mode
        ;; bits 5-4 ... 00 = 8 bits sync char, this is not 8bits/char!
        ;; bit 3-2 ... 01 = 1 stop bit
        ;; bit 1 ... 0 = even parity
        ;; bit 0 ... 0 = no parity
        ,0x44           ; 0b01000100 
        ;; we are at WR0 again, next write goes to WR3 
        ,0x03           ; 0b00000011
        ;; bits 7-6 = 11 = rx 8 bits/character
        ;; bits 0 = rx enable
        ,0xC1           ; 0b11000001
        ;; write next byte into WR5
        ,0x05           ; 0b00000101
        ;; bit 7 = 0 ... dtr off
        ;; bits 6-5 ... 11 = tx 8 bits/character
        ;; bit 3 ... 11 = tx enable
        ,0x68           ; 0b01101000
~~~

Setting RTS on and off

~~~
rts_off:
        ;; next write will go to WR5
        ld a,#0x05                      
        out (#SIO_A_CTRL),a             ; write to control reg. of port A
        ;; 0b11101000
        ;; bit 7 ... dtr=on
        ;; bits 6-5 ... 11=8 bits/char
        ;; bit 4 ... break off
        ;; bit 3 ... tx on
        ;; bit 2 ... ???
        ;; bit 1 ... rts off
        ;; bit 0 ... tx crc disabled
        ld a,#0xe8
        out (#SIO_A_CTRL),a             
        ret

rts_on:
        ;; next write will go to WR5
        ld a,#0x05
        out (#SIO_A_CTRL),a             ; write to control reg. of port A
        ;; same as rts_off, but 0b11101010
        ;; bit 1 ... rts on
        ld a,#0xea                      ; dtr, 8bit, break off, tx on, rts on
        out (#SIO_A_CTRL),a             
        ret
~~~

And reading and writing to sio port.

~~~
        ;; read sio with polling
read_sio:
        in a,(#0xd9)                    ; read register 0
        ;; bit 0 ... receive char avail?
        bit	#0,a
        jr z, read_sio                  ; loop until char ready
        in a,(#0xd8)                    ; read the char data
        res #0x07, a                    ; make it ascii?
        cp #0x20                        ; lower then space (first ascii)?
        ret c                           ; finish if control sign (<32)

        ;; a is char to send
write_sio:
        out (#SIO_A_D), a                ; output char
wait4sent:
        ld 1,#0x01                      ; select read register 1
        out (#SIO_A_C),a 
        in a,(#SIO_A_C)                 ; read it
        ;; bit 0 of RR1 is "all sent"
        bit 0,a
        jp z,wait4sent                  ; must be 1
        ret
~~~

## Other SIO libraries

Analysis of this and comparison with the ROM code showed similar traits.
https://github.com/dattaway/Zilog-Z80-Projects/blob/master/SIO.ASM

https://github.com/anchorz/z80-uart/tree/main/src

https://github.com/durgadas311/MmsCpm3/blob/72e3ae10ada3d85a874fe2614ee0e7ea14c6d768/sys/kaypro/src/sio3kp.asm

https://github.com/bienata/CA80/blob/5565ca7b8a0b7a48b9a37d0f5f72f0577ab1effc/calo4000.asm

https://github.com/Soomiru/mame/blob/d1b98e6e1013fccbd80161f94edab198d0bffb57/src/devices/machine/z80dart.h

# Notes from 19. apr 2021

# Serial comms basics

A device can act as a DTE or a DCE. The DTE is usually a PC, and the DCE a peripheral device. Following are pinouts on DB-25 for both. According to the standard the DTE usually has a male connector and the DTE a female.

| name | purpose                                                                                                    | DTE | DCE | DB25 pin |
|------|------------------------------------------------------------------------------------------------------------|-----|-----|----------|
| DTR  | DTE is ready (Data Terminal Ready)                                                                         | out | in  | 20       |
| DCE  | DCE is receiving a carrier (Data Carrier Detect)                                                           | in  | out | 8        |
| DSR  | DCE is ready to receive and send (Data Set Ready)                                                          | in  | out | 6        |
| RI   | DCE has detected incoming phone ring (Ring Indicator)                                                      | in  | out | 22       |
| RTS  | DTE requests the DCE prepare to transmit data (Reaquest To Send)                                           | out | in  | 4        |
| RTR  | DTE is ready to receive data from DCE. This is by convention i.e. uses same pin as RTS. (Ready to Receive) | out | in  | 4        |
| CTS  | DCE is ready to accept data from DTE (Clear to Send)                                                       | in  | out | 5        |
| TxD  | Transmit data                                                                                              | out | in  | 2        |
| RxD  | Receive data                                                                                               | in  | out | 3        |
| GND  | Zero voltage reference for pins above (Ground)                                                             | -   | -   | 7        |
| PG   |  Protective ground, connected to the chassis ground                                                        | -   | -   | 1        |


## Z80 SIO galore

https://github.com/dcollens/ZED-80/blob/523859c66f22f01e962289b1a9ae0e3ca3ac6167/emulator/src/io_sio.mm

https://github.com/BirchJD/advancemame-0.106.1-RPi/blob/43064d73e5e7a28dc6a76922969adfd0ca5b6c86/src/machine/z80sio.h

https://github.com/SHARPENTIERS/EmuZ-700-1500/blob/19ede7e09588ab368312abb08559b2beb84f473a/source/src/vm/z80sio.cpp
https://github.com/KailoKyra/Clay/tree/faa3142fb87a76f54cedc509b8e02e306cea1c39/rom

https://github.com/pdzion/fc-100/blob/a29f1a048c1cee65c18519bbf5290c45cd86d88b/source/src/vm/mz2500/mouse.cpp

https://github.com/DSkywalk/gmame-custom-0.178/blob/bdeba7e98124d5ba19b8e119419bbd6f203986d0/src/devices/machine/z80sio.cpp

https://github.com/anchorz/z80-uart/tree/main/src

https://github.com/anchorz/jkcemu/tree/master/src/z80emu

