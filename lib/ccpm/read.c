/*
 * read.c
 *
 * standard C file read function
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
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <io.h>
#undef FILE /* Make sure you don't use stdio.h version */
#include <fio.h>

/* fwd definition */
extern ssize_t seq_read(int fd, void *buf, size_t count);

ssize_t read(int fd, void *buf, size_t count)
{
    return seq_read(fd, buf, count);
}

ssize_t seq_read(int fd, void *buf, size_t count)
{
    fcb_t *fcb_ptr = NULL;
    uint16_t ret_ba, ret_hl;
    int rval = 0, rval2 = 0;
    int cpm_err = 0;
    int rd = 0;
    int start_offset = 0;
    int limit = SSIZE_MAX;
    uint16_t required_module = 0;
    uint16_t required_extent = 0;
    uint16_t required_block = 0;
    uint16_t required_resv = 0;
    bool flag_reopen = false;
    uint8_t i = 0, j = 0;

    if ((fd < 0 || fd >= FILES_MAX) || cfd[fd].id == -1) {
        errno = EBADF;
        return -1;
    }

    if (count > SSIZE_MAX) {
        /* apparently behaviour is supposed to be undefined here, but we will return I/O error */
        errno = EIO;
        return -1;
    }
    fcb_ptr = (fcb_t*) &cfd[fd].fcb;

    if (cfd[fd].offset % SSIZE_MAX) {
        start_offset = cfd[fd].offset % SSIZE_MAX;
        limit -= start_offset;
    }

    /* TODO: check hardcoded values */
    required_module = (cfd[fd].offset / 524288);
    required_resv = (uint16_t) ((uint8_t) ((0x80 + required_module) & 0x00ff) << 8);
    required_extent = ((cfd[fd].offset / 16384) % 0x20);
    required_block = ((cfd[fd].offset % 524288) % (EXTENT_SIZE)) / 128;

    if (fcb_ptr->ex != required_extent) {
        fcb_ptr->resv = required_resv;
        fcb_ptr->ex = required_extent;
    }

    if (fcb_ptr->resv != required_resv) {
        fcb_ptr->resv = required_resv;
        fcb_ptr->ex = required_extent;
    }

    fcb_ptr->seqreq = (required_block) % 0x80;

    /* Magic undocumented folding algorithm for crossing extent boundaries */
    if (fcb_ptr->ex && fcb_ptr->seqreq == 0x0) {
        fcb_ptr->ex --;
        fcb_ptr->seqreq = 0x80;
    }

    memset(&dma_buffer, 0xFF, 128);
    cpm_set_dma_addr((uint16_t)dma_buffer);

    rval = cpm_perform_file_op(fop_read_seq_record, fcb_ptr);
    ret_ba = get_ret_ba();
    ret_hl = get_ret_hl();

    if (rval == 0) {
        rval2 = cpm_perform_file_op(fop_upd_rand_rec_ptr, fcb_ptr);
    }

    if (rval != 0) {
        cpm_err = (ret_ba & 0xff00) >> 8;
        switch(cpm_err) {
        case 0x01:
            /* end of file - return 0 to caller, clear errno */
            errno = 0;
            return 0;
            break;
        default:
            /* TODO: error processing */
            exit(1);
            break;
        }

        /* something bad happened? */
        errno = EIO;
        return -1;
    }

    /* if we requested more bytes than are available, 
       just copy those and return the value */
    if (count < limit) {
        limit = count;
    }

    /* copy to target buffer - TODO - see if we can work zero copy in here */
    memcpy(buf, &dma_buffer + start_offset, limit);
    
    /* TODO: return the number of bytes actually read */
    cfd[fd].offset += limit;
    errno = 0;
    return limit;
}


