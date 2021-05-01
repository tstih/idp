#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl_private.h>
#include "cpmbdos.h"
#include "cpm_sysfunc.h"
#include "ansi_term.h"
#include "tty.h"

/*
		If you are writing an emulator at BDOS level, you need to be aware of how CP/M uses the bytes EX, S2, and CR. Some programs (such as the Digital Research linker,
		LINK.COM) manipulate these bytes to perform "seek" operations in files without using the random-access calls.

			CR = current record,   ie (file pointer % 16384)  / 128
			EX = current extent,   ie (file pointer % 524288) / 16384
			S2 = extent high byte, ie (file pointer / 524288). The CP/M Plus source code refers to this use of the S2 byte as 'module number'.
*/

/* errno should be set appropriately */

int stat(char *pathname, struct stat *buf)
{
    static FCB statfcb;
    FCB *fcb_ptr = NULL;
    char _filename[9];
    char _filetype[4];
    int length = 0;
    int rval = 0;
    char *ptr = NULL;
    uint16_t ret_ba, ret_hl;
    uint8_t current_extent = 0; /* 16K block index */
    uint8_t module_number = 0; /* 512K block index */
    uint16_t num_records = 0; /* number of 128 byte blocks */

    if (!_fds_init_done) {
        _fds_init();
    }

    memset(&statfcb, 0, sizeof(FCB));
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

    fcb_ptr = (FCB*) &statfcb;

    /* TODO: I believe this will only work with CPM2.2 or later, so we may need a better method
             that supports CPM1.x */

    cpm_setFCBname(_filename, _filetype, (FCB*) fcb_ptr);

    rval = cpm_performFileOp(fop_calcFileSize, fcb_ptr);
    ret_ba = get_ret_ba();
    ret_hl = get_ret_hl();

//    printf("+++ ret_ba = 0x%04x\n", ret_ba);

    /* according to: https://www.seasip.info/Cpm/bdos.html
     *
     *  "A" register should contain 0xFF if not found, and 0x00 if file found
     *  It doesn't seem to work that way with Z80Pack, so we'll just do our best.
     *  This probably means we will return st_size=0 for a missing file, as well
     *  as an existing, zero-byte file
     *
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



