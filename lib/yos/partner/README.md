# IDP YOS Configuration

## Boot process
The ROM loads CPM loader (from boot sector) to `0xe000` and jumps to 0xf600. 
We can't change that. This is inside the "operating system area" spanning 
from `0xdc00`.

## Memory map

### Banks
IDP has two 64K memory banks, each `0xf400` bytes long. Upper `0xc00` (3072) bytes
are shared and can be read/write regardless of the selected bank. Operating
system uses this space for shared structures.

### Layout

The yos operating system layouts memory in the following way.

**BANK 1**

| addr   | area        |
|--------|-------------|
| 0x0000 | RST vectors |
| 0x0100 | kernel      |
| 0x4000 | user memory |
| 0xf400 | OS heap     |
| 0xffff | OS stack    |

User memory is managed by the kernel and should be allocated using the `malloc()` function.

**BANK 2**

Same as bank 1 for version 1.0.

In yos 1.0, the kernel resides in both banks, and uses only shared space 
to communicate.

## Sys.calls

A pointer to a system call table for a particular server API is obtained by
loading HL with the API name, and then executing RST 10. 
~~~
        LD HL,#yos
        RST 0x10
        ; HL now has yos api pointer
        RET
yos:    .asciiz "yos"
~~~

## Thread Switching

Threads can reside in different banks. 

# Creating the boot sector

## SDCC compiler suite

Make sure you have the SDCC compiler suite correctly installed. The 
recommended installation is from the source files, but if you are
satisfied with a bit older version of compiler, you can also do 

`sudo apt-get install sdcc`

In case you need to peek at the generated assembly, you can install a
dissasembler

`sudo apt-get install z80dasm`

## Compiling the boot.s

You can compile the assembly into object code with `sdasz80 -losyg boot.s`,
and link it with `sdldz80 -pmiy boot.rel`. Finally, you can convert the
generated `boot.ihx` (that's a binary in Intel Hex format) into the 
raw binary file by using the `objcopy -I ihex -O binary boot.ihx boot.bin` tool.

# Links

https://github.com/breakintoprogram/bsx/blob/master/z80/Monitor/uart.z80
https://shred.zone/cilla/page/426/z80-disassembler.html
https://www.tablesgenerator.com/markdown_tables#