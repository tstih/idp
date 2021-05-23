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
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

int close(int fd)
{
    fcb_t *fcb_ptr = NULL;
    int rval = 0 ;
    if (fd < 0) {
        errno = EBADF;
        return -1;
    }
    /* errno should be set appropriately */
    if (cfd[fd].id != -1) {
        fcb_ptr = &cfd[fd].fcb;
        rval = cpm_perform_file_op(fop_close, fcb_ptr);
        cfd[fd].id = -1;
        errno = 0;
        return 0;
    }
    errno = EBADF;
    return -1;
}