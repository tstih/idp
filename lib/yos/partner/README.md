# IDP YOS Configuration

## Boot process
The ROM loads CPM loader (from boot sector) to 0xe000 and jumps to 0xf600. 
We can't change that. This is inside the "operating system area" spanning 
from 0xdc00.

## Memory map

### Banks
IDP has two 64K memory banks, each 0xf400 bytes long. Upper 0xc00 (3072) bytes
are shared and can be read/write regardless of the selected bank. Operating
system uses this space for shared structures.

### Layout
**BANK 1**
0x0000:     RST vectors
0x0100      kernel
0x4000:     user memory
0xf400;     OS heap 
0xffff:     OS stack
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