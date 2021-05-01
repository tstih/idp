/*
 * close.c
 *
 * standard C file lseek function
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 01.05.2021   tstih
 *
 */
#include <errno.h>
#include <fcntl.h>
#include <cpm_sysfunc.h>

off_t lseek(int fd, off_t offset, int whence)
{
    if (!_fds_init_done) {
        _fds_init();
    }
    /* here we just verify offsets and tweak the pointer around. the real 
       magic happens in read/write */
    if (CFD[fd].id == -1) {
        errno = EBADF;
        return -1;
    }
    if (whence == SEEK_SET) {
        CFD[fd].offset = (uint32_t) offset;
        errno = 0;
        return 0;
    }
    errno = EINVAL;
    return -1;
}



