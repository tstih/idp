/*
 * stdio.c
 *
 * idp standard C library fileio impl: fopen, fread,
 * fwrite, ftell, fseek, feof, and fclose.
 * 
 * NOTES:
 *  the idp standard library is based on the cpmlibc
 *  https://github.com/dmo9000/cpmlibc
 * 
 *  If you are writing an emulator at BDOS level, 
 *  you need to be aware of how CP/M uses the bytes 
 *  EX, S2, and CR. Some programs (such as the Digital 
 *  Research linker, LINK.COM) manipulate these bytes 
 *  to perform "seek" operations in files without using 
 *  the random-access calls.
 *  - CR = current record,   ie (file pointer % 16384)  / 128
 *  - EX = current extent,   ie (file pointer % 524288) / 16384
 *  - S2 = extent high byte, ie (file pointer / 524288). 
 *    The CP/M Plus source code refers to this use 
 *    of the S2 byte as 'module number'.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 28.04.2021   tstih
 *
 */
#include <errno.h>
#include <cpm_sysfunc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

extern int open(const char *pathname, int flags);
extern int close(int fd);
extern ssize_t read(int fd, void *buf, size_t count);
extern ssize_t write(int fd, void *buf, size_t count);
extern off_t lseek(int fd, off_t offset, int whence);

extern int  _find_free_filehandle();
extern FILE filehandles[FILES_MAX];

void putchar(int c) {
    cpm_putchar(c);
}

FILE *fopen(const char *path, const char *mode)
{
    static struct stat statbuf;
    int fd = -1;
    int fh = -1;
    int oflags = 0;
    FILE *myfhptr = NULL;   
    ssize_t initial_size = 0;

    if (strncmp(mode, "r", 1) == 0)
    {
        oflags = O_RDONLY;
    }

    if (strncmp(mode, "w", 1) == 0)
    {
        oflags = O_WRONLY | O_TRUNC;
    }

    if (strncmp(mode, "r+", 2) == 0)
    {
        oflags = O_RDWR;
    }

    if (strncmp(mode, "w+", 2) == 0)
    {
        oflags = O_RDWR | O_TRUNC;
    }
    errno = 0;
    fd = open(path, oflags);

    if (fd < 0)
    {
        /* pass through errno */
        return NULL;
    }
    errno = 0;
    /* get free filehandle */
    fh = _find_free_filehandle();
    if (fh == -1)
    {
        close(fd);
        errno = ENFILE;
        return NULL;
    }
    errno = 0;
    filehandles[fh]._file = fd;
    filehandles[fh]._eof = false;
    filehandles[fh]._limit = initial_size;
    memset(&filehandles[fh]._flags, 0, 4);
    strncpy((const char *)&filehandles[fh]._flags, (const char *)mode, 3);
    myfhptr = &filehandles[fh];
    return (FILE *)myfhptr;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    ssize_t ritems = 0;
    int i = 0;
    size_t bytes = 0;
    int rd = 0;
    char *myptr = (char *)ptr;
    char *eofptr = NULL;
    if (!stream || stream->_file == -1)
    {
        errno = EBADF;
        return -1;
    }
    for (i = 0; i < nmemb; i++)
    {
        rd = read(stream->_file, myptr, size);
        if (rd == 0)
        {
            /* END OF FILE REACHED */
            stream->_eof = true;
        }

        if (rd == -1)
        {
            /* fread() returns 0 on error (or a short item count) 
               and leaves it to the caller to determine what happened */
            return (0);
        }

        if (rd == size)
        {
            myptr += rd;
            ritems++;
        }
        else
        {
            if (rd < size)
            {
                errno = EIO;
                return ritems;
            }
            errno = EIO;
            return ritems;
        }
    }
    /* before returning, check the 'b' flag on the FILE. if it is not set, 
       we need to search for the EOF marker in this block */
    if (memchr(&stream->_flags, 'b', 3) == NULL)
    {
        /* if the file was not opened in binary mode, we should respect 
           that character 0x1A is the EOF marker */
        eofptr = memchr((const char *)ptr, 0x1A, size * nmemb);
        if (eofptr)
        {
            stream->_limit = cfd[stream->_file].offset + (eofptr - ptr);
            stream->_eof = true;
            cfd[stream->_file].offset -= (SSIZE_MAX - (eofptr - ptr));
            stream->_limit = cfd[stream->_file].offset;
        }
        else
        {
        }
    }
    errno = 0;
    return ritems;
}

size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    int i = 0;
    ssize_t wr = 0;
    char *myptr = (char *)ptr;
    if (!stream || stream->_file == -1)
    {
        errno = EBADF;
        return -1;
    }
    for (i = 0; i < nmemb; i++)
    {
        wr = write(stream->_file, myptr, size);
        if (wr == 0)
        {
            /* END OF FILE REACHED */
            stream->_eof = true;
        }
        if (wr == -1)
        {
            /* pass through errno from write() */
            return 0;
        }
    }
    return i;
}

long ftell(FILE *stream)
{
    return cfd[stream->_file].offset;
}

int fseek(FILE *stream, long offset, int whence)
{
    if (!stream || stream->_file == -1)
    {
        errno = EBADF;
        return -1;
    }
    /* success */
    if (lseek(stream->_file, offset, whence) == -1)
    {
        /* pass through errno from lseek() */
        return -1;
    }
    errno = 0;
    return 0;
}

int feof(FILE *stream)
{
    errno = 0;
    if (!stream || stream->_file == -1)
    {
        errno = EBADF;
        return -1;
    }
    if (stream->_eof)
    {
        return 1;
    }
    return 0;
}

int fclose(FILE *stream)
{
    if (!stream || stream->_file == -1)
    {
        errno = EBADF;
        return -1;
    }
    /* FIXME: probably should call close()! */
    stream->_file = -1;
    errno = 0;
    return 0;
}