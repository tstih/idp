# Notes from 21. mar 2021

## Start up procedure

ROM loads CPM loader (from boot sector) to 0xe000 and jumps to 0xf600. The emulator expects first byte to have a value of 31h (opcode for LD SP,). Suspected length of loader is 0x1F00 bytes. Code at 0xf600 copies the loader to 0x100.

* TODO: Find real length of the loader.
* TODO: Find out whay it copies itself to 0x100.

The loader uses virtual 128 byte disk blocks and does not implement write (or is not simulated). Due to the emulator shortcomings you should avoid addresses between 0xb00 and 0xb72.

* TODO: Understand this.

# Notes from 27. may 2021

## Links

Porting CP/M
http://cpuville.com/Code/CPM-on-a-new-computer.html

http://www.s100computers.com/Software%20Folder/CPM3%20BIOS%20Installation/CPM3%20HDISK%20BIOS%20Software.htm
