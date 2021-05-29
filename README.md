![status.badge] [![language.badge]][language.url] [![standard.badge]][standard.url] [![license.badge]][license.url]

# idp-dev

Welcome to the **idp-dev**, a repository, dedicated to the 
*Iskra Delta Partner* development.

 ![Iskra Delta Partner](doc/img/partner.jpg)

We are a group of volunteers, providing tools and tutorials to developers
writing software for the *Iskra Delta Partner* computer.

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
  * [PONG](#pong)
- [Building](#building)
- * [Prerequisites](#prerequisites)
  * [Make](#make)
  * [Other make targets](#other-make-targets)
- [Internals](#internals)
- [Creating disks manually](#creating-disks-manually)
  * [Create hard drive](#create-hard-drive)
  * [Create floppy drive](#create-floppy-drive)
  * [Add local files to disk](#add-local-files-to-disk)
  * [Remove files from disk](#remove-files-from-disk)
- [The Emulator](#the-emulator)
- [Thank you](#thank-you)
- [The team](#the-team)

# Projects

## The C runtime

The goal of this project is to create a *C11* compiler and a standard C library 
for the *Iskra Delta Partner*. We have adjusted the *SDCC* compiler suite to
generate `.com` files for *Iskra Delta Partner.* 

The adjustment consists of three modules:

| Module                     | Status (complete) |
|----------------------------|-------------------|
| The startup code           | ![100%](https://progress-bar.dev/100/)|
| The SDCC auxiliary library | ![100%](https://progress-bar.dev/100/)|
| The Standard C library     | ![100%](https://progress-bar.dev/100/)|  

To compile your program code, you must strip it of all *SDCC* defaults, and
use *Partner* defaults instead. Following switches should be used.

 * `--no-std-crt0`. This switch tells the compiler not to include the *SDCC*
   C startup file `crt0.s`. You replace it by adding the `scripts/crt0cpm.s`
   as the first file to your project.
 * `--nostdinc`. This will prevent the *SDCC* to include standard headers from
   the SDCC version of the standard library. You can use the `-I` to redirect 
   to *Partner's* standard library headers `-I /include/clib` instead.
 * `--nostdlib`. This will tell the *SDCC* linker no to link the *SDCC* standard
   library. You can replace it by instructing the linker to search the `build/`
   folder for `libsdcc`, and `libccpm`, like this `-Lbuild -llibsdcc -llibcpm`  

Consult the `hello` chapter for complete command line to use to compile your
C source code to *Iskra Delta Partner* binary.

### Startup Code

The startup code is file `scripts/crt0cpm.s`. This code should compile 
to the `0x100` address. It initializes the stack and the heap, reads and
stores the command line arguments (if any!), calls your `main()` function 
and, after it completes, calls the *CP/M* (*BDOS*) `exit()`. 

### SDCC Auxiliary Library

The `libsdcc` auxiliary library is located in the `lib/sdcc/` folder.

This library is the glue betweeen the *SDCC* C compiler and the
*Z80* processor. *Z80* lacks instructions for integer multiplication 
and division, and for handling long integers and floating point 
numbers.

To mitigate it, the *SDCC* C compiler replaces these non-existing
instructions with calls to special functions (such as: `__mulint`).
Invisible to you, the linker then links these special functions 
with your code.

This works in the *SDCC* realm, but if you prevent the compiler to link
default *SDCC* libraries (by using directives `--nostdlib`, `--nostdinc`, 
and `--no-std-crt0`) then you need to provide these special functions,
and the `libsdcc` does that.

### Standard C Library

The *Standard C Library* for *Iskra Delta Partner* is called `libccpm`.
In addition to standard functions it also implements some non-standard 
extensions.  

 > You can check the implementation scope by expanding the 
 > standard header files bellow.

<details><summary>conio.h/</summary>

~~~cpp
/* Terminal type */
#define T_PARTNER           0x00
#define T_VT52              0x01
#define T_ANSI              0x02

/* Text attributes - these are loosely compatible with
VT52 SGR attributes */
#define AT_NONE             0x00
#define AT_BOLD             0x01
#define AT_HIGHLIGHT        0x02
#define AT_BOLD_HIGHLIGHT   0x03
#define AT_UNDERLINE        0x04
#define AT_BLINK            0x05
#define AT_INVERSE          0x07

/* Basic info about the terminal */
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

/* Move cursor */
extern void gotoxy(int x, int y);

/* Put char. */
extern int putch(int c);

/* Put string */
extern int cputs(char *s);

/* Hide cursor. */
extern void hidecursor();

/* Show cursor. */
extern void showcursor();

/* Non blocking keyboard read. */
extern int kbhit();

/* Delete until end of line. */
extern void clreol();

/* Delete current line/ */
extern void delline();

/* Set text attributes. */
extern void textattr(unsigned char attr);

/* DEC Duble-Width, Double-Height Line top */
extern void decdhl_top();

/* DEC Duble-Width, Double-Height Line bottom */
extern void decdhl_bottom();

/* DEC Duble-Width, Single-Height Line */
extern void decdwl();

/* DEC Single-Width, Single-Height Line top */
extern void decswl();
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

~~~cpp
#define ENOENT      2       /* no such file or directory */
#define	EIO         5       /* I/O error */
#define	E2BIG       7       /* argument list too long */
#define EBADF       9       /* bad file descriptor */
#define	EAGAIN      11      /* try again */
#define EWOULDBLOCK EAGAIN  /* -"- */
#define	ENOMEM      12      /* out of memory */
#define EINVAL      22      /* negative offset or offset beyond end of file? Invalid address */
#define ENFILE      23      /* too many open files (file table overflow) */
#define	ENOTTY      25      /* not a typewriter */
#define	EPIPE       32      /* broken pipe */

/* global error code */
extern int errno;
~~~
</details>  

<details><summary>fcntl.h/</summary>

~~~cpp
#define O_RDONLY    0
#define O_WRONLY    1
#define O_RDWR      2
#define O_TRUNC     4

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
~~~
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

~~~cpp
typedef unsigned int    size_t;
typedef long            ssize_t;
typedef long            off_t;
~~~
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

~~~cpp
#define EOF         0x1A	
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define FILE        void
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* Open file. */
extern FILE *fopen(const char *path, const char *mode);

/* Move to fpos. */
extern int fseek(FILE *stream, long offset, int whence);

/* Read a record. */
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

/* EOF reached? */
extern int feof(FILE *stream);

/* Close a file. */
extern int fclose(FILE *stream);

/* Get file position. */
extern long ftell(FILE *stream);

/* Write a record. */
extern size_t fwrite(
    const void *ptr, 
    size_t size, 
    size_t nmemb, 
    FILE *stream);

/* Prints a string. */
extern int puts(const char *s);

/* Print formatted string to stdout. */
extern int printf(char *fmt, ...);

/* Prints formated string to a string. */
extern int sprintf(char *buf, char *fmt, ...);

/* Prints a char. */
extern void putchar(int c);

/* Reads a char (blocks. */
extern int getchar(void);
~~~
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

/* Covert ascii to integer. */
extern int atoi(const char *str);

/* Strin to unsigned long using base, */
extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

/* Absolute value. */
extern int abs (int i);

/* Return random number */
extern int rand(void);

/* Set random seed. */
extern void srand(unsigned int seed);

/* Memory allocation. */
extern void *malloc(size_t size);

/* Allocate zero initialized block. */
extern void *calloc (size_t num, size_t size);

/* Free allocated memory block. */
extern void free(void *ptr);
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

<details><summary>sys/stat.h/</summary>

~~~cpp
struct stat
{
    short   st_mode;                    /* flags */
    long    st_atime;                   /* access time */
    long    st_mtime;                   /* modification time */
    long    st_size;                    /* file size in bytes */
};

/* Flag bits in st_mode */
#define S_IFMT          0x600           /* type bits */
#define S_IFDIR         0x400           /* is a directory */
#define S_IFREG         0x200           /* is a regular file */
#define S_IREAD         0400            /* file can be read */
#define S_IWRITE        0200            /* file can be written */
#define S_IEXEC         0100            /* file can be executed */
#define S_HIDDEN        0x1000          /* file is hidden */
#define S_SYSTEM        0x2000          /* file is marked system */
#define S_ARCHIVE       0x4000          /* file has been written to */

/* Tests */
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

/* Set file mode. */
extern int chmod(const char *path, mode_t mode);

/* Get file info. */
extern int stat(char *path, struct stat *buf);
~~~
</details>  

<details><summary>sys/types.h/</summary>

~~~cpp
typedef uint32_t mode_t;
~~~
</details>  

<br/>

### Hello Partner Project

The *Iskra Delta Partner* **Hello World** program is located in the 
`src/hello` folder.

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
 * `stdlib/std-test.c` The *Standard C Library* auto unit tests.
 * `stdlib/file-xp.c` Creating a test text file: `ZDRAVA.TXT`
 * `stdlib/mem-xp.c` Allocating memory with `malloc`, freeing with `free` test.
 * `stdlib/rnd-xp.c` Random numbers tests.
 * `stdlib/tme-test` Testing the `time.h` functions. **Warning:** sets system time to 1.1.1980.
 * `hw/setup-xp.c` Displays real time clock and battery powered RAM bytes.
 * `hw/conio-xp.c` Console output tests.
 * `gpx/gpx-xp.c` Drawing experiments (in graphics mode).

## GPX

The goal of this project is to create fast graphical library for 
*Iskra Delta Partner*.

![Under construction.](doc/img/under-construction.jpg)

## xyz

The goal of this ambitious project is to create a graphical, 
multitasking operating system to replace the *CP/M* on *Iskra Delta Partner*.

![Under construction.](doc/img/under-construction.jpg)

## PONG

Terminal version of the *Atari PONG*. 

 > This was a viability test and the conclusion is that terminal emulation
   on *Iskra Delta Partner* is too slow for games.

## TETRIS

A version of the game of *Tetris*.

![Under construction.](doc/img/under-construction.jpg)

# Building

## Prerequisites

At present the build environment is *Linux* with following tools installed: 
`sdcc`, `cpmtools`, `gcc`, and `sed`. 

 > The *Standard C Library* requires latest version of *SDCC* (**4.1.6**). 
 > You are going to have to [build it from the sources]
 > (http://sdcc.sourceforge.net/). To make things more complicated, 
 > the build process also requires an exact version of the `automake` tools 
 > (**1.16.2**) and will not compile with other versions.

## Make

Commpile everything with 

`make`

After you are done compiling, create disk image for the emulator with

`make install`

All output will go to the `build` folder. Disk image that you 
can import into the Partner emulator (using **Alt+O**) is called `fddb.img`.

## Other make targets

For comfortable work following targets can also be used. Each
of them creates a `bin` folder, copies the `.com`, `.lib` and `crt0cpm.rel` 
files into it. And then creates a image of the floppy disk called 
`fddb.img` with all the `.com` files to the floppy image, 
   

 * `make install` Create the `bin` folder and standard floppy (with `.com` files).
 * `make ccp` Add `ccp.com` to the floppy. CP/M allows programs to overwrite its'
   command shell called the CCP. When the program ends the CP/M reloads the shell
   and if not present on the disk displays an error (prompts for disk change). 
 * `make boot` Uses a bootable floppy for *std. partner* as a base for creating
   the disk image. 
 * `make bootg` Uses a bootable floppy for *graphical partner* as a base for
   creating the disk image
 * `make dex` Calls `make install` and copies the floppy image to a user
   folder (`~/Dex/`). Use this if you work in *Linux* and need to exchange 
   the image with enother environment (i.e. a *Windows* where the emulator
   is running). *In case you wonder, dex stands for Data EXchange.*

# Internals

While reverse engineering the Partner and writing code, we are keeping 
notes on its internal functioning. While these are not well structured,
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

# Creating disks manually

Use the `cpmtools` package to create your own disks for the emulator.

http://www.moria.de/~michael/cpmtools/

Disk definitions for Partner floppy and hard drives are in
the the `scripts\diskdefs` file.
 * `idpfdd` for floppy drive
 * `idphdd` for the hard disk

## Create hard drive

Note: `-f` is disk format and can be `idphdd` or `idpfdd`.

`mkfs.cpm.exe -f idphdd -t hdda.img`

## Create floppy drive

`mkfs.cpm.exe -f idpfdd -t fddb.img`

## Add local files to disk

Following command adds file `hello.com` to area 0: of floppy drive `fddb.img`.

`cpmcp -f idpfdd fddb.img test.com 0:test.com`

## Remove files from disk

`cpmrm -f idpfdd fddb.img 0:test.com`

# The Emulator

You can download the emulator from here.

http://matejhorvat.si/sl/slorac/delta/partner/index.htm

Once you are in the emulator, press Alt+O to select the `fddb.img` file. 
This will create a `B:` drive. Finally, type...

~~~
B:
DIR
HELLO
~~~

And, voila...

![Hello World](doc/img/hello.jpg)

# Thank you

*Matej Horvat* for sharing technical details about his emulator and code samples.

# The team

*Tomaz Stih* (lead) 
*Miha Grcar*


[language.url]:   https://en.wikipedia.org/wiki/ANSI_C
[language.badge]: https://img.shields.io/badge/language-C-blue.svg

[standard.url]:   https://en.wikipedia.org/wiki/C89/
[standard.badge]: https://img.shields.io/badge/standard-C89-blue.svg

[license.url]:    https://github.com/tstih/nice/blob/master/LICENSE
[license.badge]:  https://img.shields.io/badge/license-MIT-blue.svg

[status.badge]:  https://img.shields.io/badge/status-unstable-red.svg