/*
 * stat.c
 *
 * standard C file stat function
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 02.05.2021   tstih
 *
 */
#include <errno.h>
#include <cpm_sysfunc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#undef FILE /* Make sure you don't use stdio.h version */
#include <fio.h>

int stat(char *pathname, struct stat *buf)
{
    static fcb_t statfcb;
    fcb_t *fcb_ptr = NULL;
    char _filename[9];
    char _filetype[4];
    int length = 0;
    int rval = 0;
    char *ptr = NULL;
    uint16_t ret_ba, ret_hl;
    uint8_t current_extent = 0; /* 16K block index */
    uint8_t module_number = 0;  /* 512K block index */
    uint16_t num_records = 0;   /* number of 128 byte blocks */

    memset(&statfcb, 0, sizeof(fcb_t));
    memset(&_filename, 0, 9);
    memset(&_filetype, 0, 4);
    ptr = strchr(pathname, '.');

    length = ptr - pathname;
    if (length > 8) {
        errno = ENOENT;
        return -1;
    }
    strncpy(_filename, pathname, length);
    length = strlen(ptr+1);
    if (length > 3) {
        errno = ENOENT;
        return -1;
    }
    strncpy(_filetype, ptr+1, ( length < 4 ? length : 3));

    fcb_ptr = (fcb_t*) &statfcb;

    /* TODO: will this work with CPM1? */
    cpm_set_fcb_name(_filename, _filetype, (fcb_t*) fcb_ptr);

    rval = cpm_perform_file_op(fop_calc_file_size, fcb_ptr);
    ret_ba = get_ret_ba();
    ret_hl = get_ret_hl();

    /* according to: https://www.seasip.info/Cpm/bdos.html
     *  "A" register should contain 0xFF if not found, and 0x00 if file found
     *  It doesn't seem to work that way with Z80Pack, so we'll just do our best.
     *  This probably means we will return st_size=0 for a missing file, as well
     *  as an existing, zero-byte file
     */
    switch (ret_ba & 0x00ff) {
    case 0xff:
        /* file not found */
        errno = ENOENT;
        return -1;
        break;
    case 0x0:
        /* file found, set statbuf fields, including st_size, and return with 0, errno = SUCCESS */
        buf->st_mode = 0;
        buf->st_mode |= S_IFREG;
        buf->st_mode |= S_IREAD;
        buf->st_mode |= S_IWRITE;
        buf->st_atime = 0;
        buf->st_mtime = 0;
        buf->st_size = (uint32_t) (fcb_ptr->rrec) * 128;
        errno = 0x0;
        return 0;
        break;
    }

    /* some kind of weird unknown/hardware error */
    errno = EIO;
    return -1;
}