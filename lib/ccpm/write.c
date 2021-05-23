/*
 * write.c
 *
 * standard C file write function
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

ssize_t write(int fd, void *buf, size_t count)
{
    fcb_t *fcb_ptr = NULL;
    uint16_t ret_ba, ret_hl;
    int rval = 0;
    int cpm_err = 0;
    int wr =0;
    int start_offset = 0;
    int limit = SSIZE_MAX;
    uint16_t required_module = 0;
    uint16_t required_extent = 0;
    uint16_t required_block = 0;
    /* assume success */
    errno = 0;
    if ((fd < 0 || fd >= FILES_MAX) || cfd[fd].id == -1) {
        errno = EBADF;
        return -1;
    }

    /* fd is validated */
    if (count > SSIZE_MAX) {
        /* apparently behaviour is supposed to be undefined here, 
           but we will return I/O error */
        errno = EIO;
        return -1;
    }

    fcb_ptr = (fcb_t*) &cfd[fd].fcb;
    if (cfd[fd].offset % SSIZE_MAX) {
        start_offset = cfd[fd].offset % SSIZE_MAX;
        limit -= start_offset;
    }

    /* TODO: check this hardcoded values */
    required_module = (cfd[fd].offset / 524288);
    required_extent = (cfd[fd].offset / 16384);
    required_block = (cfd[fd].offset / 128);
    required_block -= (required_extent * 0x80);

    if (fcb_ptr->ex != required_extent) {
        fcb_ptr->ex = required_extent;
        rval = cpm_perform_file_op(fop_open, fcb_ptr);
    }

    if (fcb_ptr->seqreq != required_block) {
        fcb_ptr->seqreq = required_block;
    }

    fcb_ptr->rrec = 0x0000;
    fcb_ptr->rrecob = 0x00;

    /* careful, we are setting the DMA address straight 
       into the buffer here */
    cpm_set_dma_addr((uint16_t) buf);
    rval = cpm_perform_file_op(fop_write_seq_record, fcb_ptr);
    ret_ba = get_ret_ba();
    ret_hl = get_ret_hl();

    if (rval != 0) {
        cpm_err = (ret_ba & 0xff00) >> 8;
        if (fcb_ptr->rrec > 0x00ff) {} /* TODO: error */

        switch(cpm_err) {
        case 0x01:
            /* end of file - return 0 to caller, clear errno */
            errno = 0;
            return 0;
            break;
        }
        /* something bad happened? */
        errno = EIO;
        return -1;
    }

    rval = cpm_perform_file_op(fop_upd_rand_rec_ptr, fcb_ptr);

    /* if we requested more bytes than are available, just copy those and return the value */
    if (count < limit) {
        limit = count;
    }

    /* close the file */
    rval = cpm_perform_file_op(fop_close, fcb_ptr);

    /* TODO: return the number of bytes actually read */
    errno = 0;
    return limit;
}




