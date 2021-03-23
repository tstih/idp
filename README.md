# idp
Iskra Delta Partner code samples.

# Building code samples on Windows

Prerequisites: z88dk installed

`nmake /f makefile.nmake` all

# Creating disks

Use cpmtools. 

Binaries are available here.

http://www.moria.de/~michael/cpmtools/

Disk definitions for floppy and hard drives are in
the scripts folder (diskdefs):
 * idpfdd for floppy drive
 * idphdd for the hard disk

## Hard drive should be created like this

Note: -f is disk format and can be idphdd or idpfdd.
Formats are defined in the scripts\diskdefs folder

mkfs.cpm.exe -f idphdd -t hdda.img

## And floppy like this

mkfs.cpm.exe -f idpfdd -t fddb.img

## Files can be added to disks like this

Following command adds file test.com to 
area 0: of floppy drive fddb.img.

cpmcp -f idpfdd fddb.img test.com 0:test.com

## And removed like this

cpmrm -f idpfdd fddb.img 0:test.com

 ## Iskra Delta Partner emulator

 You can find the emulator here.

 http://matejhorvat.si/sl/slorac/delta/partner/index.htm