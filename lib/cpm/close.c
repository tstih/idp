/*
 * close.c
 *
 * standard C file close function
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
#include <cpm_sysfunc.h>
#include <sys/stat.h>
#include <stdlib.h>

int close(int fd)
{
    fcb_t *fcb_ptr = NULL;
    int rval = 0 ;
    if (!_fds_init_done) {
        _fds_init();
    }

    if (fd < 0) {
        errno = EBADF;
        return -1;
    }
    /* errno should be set appropriately */
    if (CFD[fd].id != -1) {
        fcb_ptr = &CFD[fd].fcb;
        rval = cpm_performFileOp(fop_close, fcb_ptr);
        CFD[fd].id = -1;
        errno = 0;
        return 0;
    }
    errno = EBADF;
    return -1;
}