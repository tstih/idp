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

ssize_t seq_read(int fd, void *buf, size_t count);

ssize_t read(int fd, void *buf, size_t count)
{
    return seq_read(fd, buf, count);
}


ssize_t seq_read(int fd, void *buf, size_t count)
{
    FCB *fcb_ptr = NULL;
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

    if (!_fds_init_done) {
        _fds_init();
    }

    if ((fd < 0 || fd >= FILES_MAX) || CFD[fd].id == -1) {
        errno = EBADF;
        return -1;
    }

    if (count > SSIZE_MAX) {
        /* apparently behaviour is supposed to be undefined here, but we will return I/O error */
        errno = EIO;
        return -1;
    }
    fcb_ptr = (FCB*) &CFD[fd].fcb;

    if (CFD[fd].offset % SSIZE_MAX) {
        start_offset = CFD[fd].offset % SSIZE_MAX;
        limit -= start_offset;
    }

    required_module = (CFD[fd].offset / 524288);
    required_resv = (uint16_t) ((uint8_t) ((0x80 + required_module) & 0x00ff) << 8);
    required_extent = ((CFD[fd].offset / 16384) % 0x20);
    required_block = ((CFD[fd].offset % 524288) % (EXTENT_SIZE)) / 128;

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
    cpm_setDMAAddr((uint16_t)dma_buffer);

//#define DEBUG_LIBCIO_READ
#ifdef DEBUG_LIBCIO_READ
    printf("%s", TTY_FOREGROUND_RED);
//    printf(" read ret.val=%02X, ret_ba=0x%04x ret_hl=0x%04x\n", rval, ret_ba, ret_hl);
    printf("\n");
    printf("\tmodule ->\t  %02X\n", required_module);
    printf("\textent ->\t  %02X\n", required_extent);
    printf("\tblock  ->\t  %02X\n", required_block);
    printf("\t--\n");
    printf("\ts1s2   ->\t%04X\n", fcb_ptr->resv);
    printf("\tex     ->\t  %02X\n", fcb_ptr->ex);
    printf("\trc     ->\t  %02X\n", fcb_ptr->rc);
    printf("\tsreq   ->\t  %02X\n", fcb_ptr->seqreq);
    printf("%s", TTY_FOREGROUND_WHITE);
#endif /* DEBUG_LIBCIO_READ */

    rval = cpm_performFileOp(fop_readSeqRecord, fcb_ptr);
    ret_ba = get_ret_ba();
    ret_hl = get_ret_hl();

    if (rval == 0) {
        rval2 = cpm_performFileOp(fop_updRandRecPtr, fcb_ptr);
    }

#ifdef DEBUG_LIBCIO_READ
    printf("%s", TTY_FOREGROUND_RED);
    printf("\tread ret.val=%02X, ret_ba=0x%04x ret_hl=0x%04x\n", rval, ret_ba, ret_hl);
    printf("\trrec   ->\t%04X\n", fcb_ptr->rrec);
    printf("\trrecob ->\t  %02X\n", fcb_ptr->rrecob);
    printf("%s", TTY_FOREGROUND_WHITE);
#endif /* DEBUG_LIBCIO_READ */

    if (rval != 0) {
        cpm_err = (ret_ba & 0xff00) >> 8;
        switch(cpm_err) {
        case 0x01:
            /* end of file - return 0 to caller, clear errno */
//            printf("\n# EOF?: ret.val=%02X, ret_ba=0x%04x ret_hl=0x%04x, CPM_ERR=%d\n", rval, ret_ba, ret_hl, cpm_err);
//            printf("/* read() hit EOF */\n");
//						for (i = 0; i < 8; i++) {
//							for (j = 0; j < 16; j++) {
//								printf("%02x ", dma_buffer[(i*16) + j]);
//								}
//								printf("\n");
//						}
            errno = 0;
            return 0;
            break;
        default:
            printf("\n# READ_FAIL: ret.val=%02X, ret_ba=0x%04x ret_hl=0x%04x, CPM_ERR=%d\n", rval, ret_ba, ret_hl, cpm_err);
            exit(1);
            break;
        }

        /* something bad happened? */
        errno = EIO;
        return -1;
    }

    /* update RR record */

    //rval2 = cpm_performFileOp(fop_updRandRecPtr, fcb_ptr);

    /* if we requested more bytes than are available, just copy those and return the value */
    if (count < limit) {
        limit = count;
    }

    /* copy to target buffer - TODO - see if we can work zero copy in here */
    memcpy(buf, &dma_buffer + start_offset, limit);
    /* TODO: return the number of bytes actually read */

    CFD[fd].offset += limit;

    errno = 0;

    return limit;
}


