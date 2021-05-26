![status.badge] [![language.badge]][language.url] [![standard.badge]][standard.url] [![license.badge]][license.url]

# idp-dev

Welcome to the **idp-dev**, a repository, dedicated to the 
Iskra Delta Partner development.

 ![Iskra Delta Partner](doc/img/partner.jpg)

We are a group of volunteers, providing tools and tutorials to developers
writing software for the Iskra Delta Partner computer.

> The repository is currently under construction. Things change. 

# Table of Contents

- [Projects](#projects)
  * [The C Runtime](#the-c-runtime)
    + [Startup Code](#startup-code)
    + [SDCC Auxiliary Library](#sdcc-auxiliary-library)
    + [Standard C Library](#standard-c-library)
    + [Hello Partner Project](#hello-partner-project)
  * [The CP/M Test Framework](#tests)
  * [GPX](#gpx)
  * [xyz](#xyz)
  * [TETRIS](#tetris)
- [Building](#building)
  * [On Linux](#on-linux)
- [Internals](#internals)
- [Creating disks](#creating-disks)
  * [Create hard drive](#create-hard-drive)
  * [Create floppy drive](#create-floppy-drive)
  * [Add local files to disk](#add-local-files-to-disk)
  * [Remove files from disk](#remove-files-from-disk)
- [The Emulator](#the-emulator)
- [Thank you](#thank-you)

# Projects

## The C Runtime

The goal of this project is to create a C11 compiler and a standard C library 
for the Iskra Delta Partner. We are adjusting the SDCC compiler suite to
generate (.COM) files for Iskra Delta Partner. 

It consists of three main modules:

| Module                     | Status (complete) |
|----------------------------|-------------------|
| The startup code           | ![100%](https://progress-bar.dev/100/)|
| The SDCC auxiliary library | ![100%](https://progress-bar.dev/100/)|
| The Standard C library     | ![80%](https://progress-bar.dev/80/)|  

To compile your source code, you must strip it of all SDCC defaults, and
use Partner defaults instead. Following switches should be used.

 * `--no-std-crt0`. This switch tells the compiler not to include the SDCC
   C startup file `crt0.s`. You replace it by adding the `scripts/crt0cpm.s`
   as the first file to your project.
 * `--nostdinc`. This will prevent the SDCC to include standard headers from
   the SDCC standard library. You can use the `-I` to redirect to Partner's
   standard library headers `-I /include/clib` instead.
 * `--nostdlib`. This will tell the SDCC linker no to link the SDCC standard
   library. You can replace it by instructing the linker to search the `build/`
   folder for `libsdcc`, and `libccpm`, like this `-Lbuild -llibsdcc -llibcpm`  

Consult the `hello` chapter for complete command line to use to compile your
C source code to Iskra Delta Partner binary.

### Startup Code

The startup code is file `scripts/crt0cpm.s`. This code should compile 
to the `0x100` address. It initializes the stack and the heap, reads and
stores the command line arguments (if any!), calls your `main()` function 
and, after it completes, calls the CP/M (BDOS) `exit()`. 

### SDCC Auxiliary Library

The `libsdcc` auxiliary library is located in the `lib/sdcc/` folder.

This library is the glue betweeen the SDCC C compiler and the
Z80 processor. Z80 lacks instructions for integer multiplication 
and division, and for handling long integers and floating point 
numbers.

To mitigate it, the SDCC C compiler replaces these non-existing
instructions with calls to special functions (such as: `__mulint`).
Invisible to you, the linker then links these special functions 
with your code.

This works in the SDCC realm, but if you prevent the compiler to link
default SDCC libraries (by using directives `--nostdlib`, `--nostdinc`, 
and `--no-std-crt0`) then you need to provide these special functions,
and the `libsdcc` does that.

### Standard C Library

The Standard C Library for Iskra Delta Partner is called `libccpm`.
In addition to standard functions it also implements some non-standard 
extensions.  

*You can check the contents of standard header files by expanding them.*

<details><summary>conio.h/</summary>

~~~cpp
/* Terminal type. */
#define T_PARTNER    0x00
#define T_VT52       0x01
#define T_ANSI       0x02

/* Text attributes. */
#define AT_NONE         0x00
#define AT_BOLD         0x01
#define AT_UNDERLINE    0x04
#define AT_BLINK        0x05
#define AT_INVERSE      0x07

/* Basic info about the terminal. */
struct text_info {
    unsigned char screenwidth;
    unsigned char screenheight;
    unsigned char terminal;
};

/* Basic information about the screen. */
extern void gettextinfo(struct text_info *ti);

/* Clears the screen. Moves cursor to 0,0. */
extern void clrscr(void);

/* A single character from the predefined 
standard input handle is read and returned. */
extern int getch(void);

/* Puts char back so that getch returns it again. */
extern int ungetch(int);

/* Move cursor. */
extern void gotoxy(int x, int y);

/* Put char. */
extern int putch(int c);

/* Put string. */
extern int cputs(char *s);

/* Hide cursor. */
extern void hidecursor();

/* Show cursor. */
extern void showcursor();

/* Non blocking keyboard read. */
extern int kbhit();

/* Delete until end of line. */
void clreol();

/* Delete current line. */
void delline();

/* Set text attributes. */
void textattr(unsigned char attr);
~~~

</details>  

<details><summary>ctype.h/</summary>

~~~cpp
/* True if char is a letter. */
extern bool isalpha(int c);

/* True if char is white space. */
extern bool isspace(int c);

/* True if char is punctuation. */
extern bool ispunct(int c);

/* Returns char, converted to lowercase. */
extern int tolower(int c);
~~~
</details>  

<details><summary>errno.h/</summary>

</details>  

<details><summary>fcntl.h/</summary>

</details>  

<details><summary>limits.h/</summary>

~~~cpp
#define CHAR_BIT    8                /* number of bits in byte */
#define SCHAR_MIN   -128
#define SCHAR_MAX   +127
#define UCHAR_MAX   255
#define CHAR_MIN    -128
#define CHAR_MAX    +127
#define SHRT_MIN    -128
#define SHRT_MAX    +127
#define USHRT_MAX   255
#define INT_MIN     -32768
#define INT_MAX     +32767
#define UINT_MAX    65535
#define LONG_MIN    -2147483648
#define LONG_MAX    +2147483647
#define ULONG_MAX   4294967295
~~~
</details>  

<details><summary>stdarg.h/</summary>

~~~cpp
/* Standard C var arg macros */
#define va_list                 unsigned char *
#define va_start(marker, last)  { marker = (va_list)&last + sizeof(last); }
#define va_arg(marker, type)    *((type *)((marker += sizeof(type)) - sizeof(type)))
#define va_end(marker)          marker = (va_list) 0;
~~~
</details>  

<details><summary>stdbool.h/</summary>

~~~cpp
#define bool int

#define false 0
#define FALSE 0
#define true 1
#define TRUE 1
~~~
</details>  

<details><summary>stddef.h/</summary>

</details>  

<details><summary>stdint.h/</summary>

~~~cpp
typedef char            int8_t;
typedef unsigned char   uint8_t;
typedef int             int16_t;
typedef unsigned int    uint16_t;
typedef long            int32_t;
typedef unsigned long   uint32_t;
typedef unsigned int*   uintptr_t;
~~~
</details>  

<details><summary>stdio.h/</summary>

</details>  

<details><summary>stdlib.h/</summary>

~~~cpp
/* Standard requires it here. */
#ifndef NULL
#define NULL 0
#endif /* NULL */

/* Exit application. */
extern void exit(int status);

/* String to long using base. */
extern long strtol(char *nptr, char **endptr, int base);

/* Strin to unsigned long using base, */
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

/* Absolute value. */
extern int abs (int i);

/* Return random number */
extern int rand(void);

/* Set random seed. */
extern void srand(unsigned int seed);
~~~
</details>  

<details><summary>string.h/</summary>

~~~cpp
#ifndef NULL
#define NULL ( (void *) 0)
#endif /* NULL */

/* Set n bytes in memory block to the value c, */
extern void *memset(void *s, int c, size_t n);

/* Copy memory block, */
extern void *memcpy(const void *dest, const void *src, size_t n);

/* Searches for the first occurrence of the character c 
(an unsigned char) in the first n bytes. */
extern void *memchr(const void *s, int c, size_t n);

/* Compare memory blocks. 0=equal. */
extern int memcmp(const void *s1, const void *s2, size_t n);

/* Zero terminated string length. */
extern size_t strlen(const char *s);

/* Copy string to another string */
extern char *strcpy(char *dest, const char *src);

/* Copy max num chars to another string. */
extern char* strncpy(char* dst, const char* src, size_t num);

/* Compare strings, 0=match. */
extern int strcmp(const char *s1, const char *s2);

/* Compare first n chars of string. */
extern int strncmp(const char *s1, const char *s2, size_t n);

/* Find first occurence of c in s. */
extern char *strchr(const char *s, int c);

/* Find first occurence of c in s frin the right (=last occurence). */
extern char *strrchr(const char *s, int c);

/* Non standard (unix) tokenizer. */
extern char *strsep(char **stringp, const char *delim);

/* The length of the number of characters before the 1st occurrence 
of character present in both the string. */
extern size_t strcspn(const char *s1, const char *s2); 

/* Tokenize string s using delimiters delim. */
extern char *strtok(char *s, const char *delim);

/* Return text of the errnum system error. */
extern char *strerror(int errnum);
~~~
</details>  

<details><summary>time.h/</summary>

~~~cpp
/* idp clock has a resolution of 1/1000 sec */
#define CLOCKS_PER_SEC  1000
typedef long clock_t;

/* Std C defines this as number of seconds since  00:00, Jan 1 1970 UTC */
typedef long time_t;

/* Std C tm struct */
struct tm {
    /* seconds,  range 0 to 59 */
    int tm_sec;
    /* minutes, range 0 to 59 */
    int tm_min;
    /* hours, range 0 to 23 */
    int tm_hour;
    /* day of the month, range 1 to 31 */
    int tm_mday;
    /* month, range 0 to 11 */
    int tm_mon;
    /* The number of years since 1900 */
    int tm_year;
    /* day of the week, range 0 to 6 */
    int tm_wday;
    /* day in the year, range 0 to 365 */
    int tm_yday;
    /* daylight saving time */
    int tm_isdst;
};

/* Converts given calendar time tm to a textual representation of 
the following fixed 25-character form: Www Mmm dd hh:mm:ss yyyy. */
extern char* asctime(const struct tm* time_ptr);

/* Return current clock in 1/1000 seconds */
extern clock_t clock(void);

/* Convert current time to textual representation using the following
format Www Mmm dd hh:mm:ss yyyy (uses asctime...).*/
extern char* ctime(const time_t* ptt);

/* Returns difference between two time points in seconds! */
extern long difftime(time_t time_end,time_t time_beg);

/* Get Greenwich mean time (politically correct: UTC), make localtime
equal to UTC. */
#define localtime gmtime
extern struct tm *gmtime(const time_t *timer);

/* Create time_t given tm structure. */
extern time_t mktime(struct tm *tme);
	
/* Get current time. */
extern time_t time(time_t *arg);

/* Non standard function to set system time. */
extern void setdatetime(struct tm *tme);
~~~
</details>  

<details><summary>unistd.h/</summary>

~~~cpp
/* Max CPM path len is NNX:AAAAAAAA.EEE plus zero terminator */
#define PATH_MAX 17

/* Open file. */
extern int open(const char *pathname, int flags);

/* Close file. */
extern int close(int fd);

/* Read count bytes from file. */
extern ssize_t read(int fd, void *buf, size_t count);

/* Write count bytes to file. */
extern ssize_t write(int fd, const void *buf, size_t count);

/* Move to position */
extern off_t lseek(int fd, off_t offset, int whence);
~~~
</details>  


<details><summary>sys/ioctl.h/</summary>

</details>  

<details><summary>sys/stat.h/</summary>

</details>  

<details><summary>sys/types.h/</summary>

</details>  

<br/>

### Hello Partner Project

The Iskra Delta Partner Hello World program is located in the `src/hello` folder.

~~~cpp
#include <stdio.h>

int main(int argc, char * argv[]) {
    printf("Hello world!\n");
    return 0;
}
~~~

and does what every **Hello World** program in the world should do. 

![Hello World](doc/img/hello.jpg)

## Tests

There are two types of tests in the `test` folder: *automated unit tests* that
automatically verify test results against expected results. And *experiments*,
that test some feature of the operating system, and require the user to check
the results. 

Automated unit tests use the [tiny test framework of Eric Radman](https://eradman.com/posts/tdd-in-c.html), based on [the original MinUnit by John Brewer](http://www.jera.com/techinfo/jtns/jtn002.html). The name of automated unit tests ends in `-test` (i.e. `std-test.com`).

Experiments may create some files, display some data, draw some graphics, etc.
Experiments end with -xp i.e. `setup-xp.com`.

At time of writing, forllowing tests are available: 
 * `stdlib/std-test.c` The Standard C Library auto unit tests.
 * `hw/setup-xp.c` Displays real time clock and battery powered RAM bytes.
 * `gpx/gpx-xp.c` Drawing experiments (in graphics mode).

## GPX

The goal of this project is to create fast graphical library for Iskra Delta
Partner.

![Under construction.](doc/img/under-construction.jpg)

## xyz

The goal of this project is to create a graphical, multitasking operating system
to replace the CP/M on Iskra Delta Partner.

![Under construction.](doc/img/under-construction.jpg)

## TETRIS

Porting terminal version of the game of Tetris.

![Under construction.](doc/img/under-construction.jpg)

# Building

## On Linux

Make sure you have gcc, sdcc, and cpmtools installed.

Compile with 

`make`

Create disk image for the emulator with

`make install`

All output will go to the build folder. Disk image that you 
can import into Partner emulator (using Alt+O) is called `fddb.img`.

# Internals

While reverse engineering the Partner and writing code, we keep notes 
on its internal functioning. While these are not well structured,
they are a rich source of information about inner functioning of
Iskra Delta Partner and the softwares.

 * [Computer and OS start up process](doc/notes/startup-notes.md)
 * [Text (&terminal)](doc/notes/avdc-notes.md)
 * [Graphics](doc/notes/gdp-notes.md)
 * [Hard disk](doc/notes/hdd-notes.md)
 * [Floppy disk](doc/notes/fdd-notes.md)
 * [Serial communication (Z80 SIO)](doc/notes/z80sio-notes.md)
 * [Real time clock](doc/notes/rtclock-notes.md)
 * [Porting software plans](doc/notes/porting-notes.md)

# Creating disks

Use cpmtools to create your own disks for the emulator.

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

# The Emulator

You can download the emulator from here.

http://matejhorvat.si/sl/slorac/delta/partner/index.htm

Once you are in the emulator, press Alt+O and select the `fddb.img` file. This will create `B:` drive. Finally, type...

~~~
B:
DIR
HELLO
~~~

And, voila...

![Hello World](doc/img/hello.jpg)

# Thank you

**Miha Grcar** for keeping a Partner Revival Slack, testing, and sharing his findings.

**Matej Horvat** for sharing technical details about his emulator and code samples.

**Tomaz Stih**, 22.05.2021


[language.url]:   https://en.wikipedia.org/wiki/ANSI_C
[language.badge]: https://img.shields.io/badge/language-C-blue.svg

[standard.url]:   https://en.wikipedia.org/wiki/C89/
[standard.badge]: https://img.shields.io/badge/standard-C89-blue.svg

[license.url]:    https://github.com/tstih/nice/blob/master/LICENSE
[license.badge]:  https://img.shields.io/badge/license-MIT-blue.svg

[status.badge]:  https://img.shields.io/badge/status-unstable-red.svg