# idp

Iskra Delta Partner code samples.

# Building code samples 

## On Windows

z88dk, cpmtools, and nmake must be installed.

Compile with 

`nmake /f makefile.nmake all`

Create disk image for the emulator with

`nmake /f makefile.nmake install` 

All output will go to the build file.

# Creating disks

Use cpmtools to create disks for the emulator.

http://www.moria.de/~michael/cpmtools/

Disk definitions for Partner floppy and hard drives are in
the the `scripts\diskdefs` file.
 * idpfdd for floppy drive
 * idphdd for the hard disk

## Create hard drive

*Note: -f is disk format and can be idphdd or idpfdd.*

`mkfs.cpm.exe -f idphdd -t hdda.img`

## Create floppy drive

`mkfs.cpm.exe -f idpfdd -t fddb.img`

## Add local files to disk

*Following command adds file test.com to area 0: of floppy drive fddb.img.*

`cpmcp -f idpfdd fddb.img test.com 0:test.com`

## Remove files from disk

`cpmrm -f idpfdd fddb.img 0:test.com`

 # Iskra Delta Partner emulator

 You can find the emulator here.

 http://matejhorvat.si/sl/slorac/delta/partner/index.htm

 Once you are in the emulator, press Alt+O and select the `build\fddb.img` file.
 This will create `B:` drive. Finally, type...

 ~~~
 B:
 DIR
 TEST
 ~~~

 And, voila...

 ![Test in emulator](doc/test.jpg)

 # Thank you

 Matej Horvat for kindly sharing technical details about his emulator and functioning of a Partner, and complex code samples.

 Miha Grcar for keeping a Partner Revival Slack and sharing his findings.

Cheers,
Tomaz Stih