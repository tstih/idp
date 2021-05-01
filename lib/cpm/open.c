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

int open(const char *pathname, int flags)
{
    /* errno should be set appropriately */
    char _filename[9];
    char _filetype[4];
    char *ptr = NULL;
    int rval = 0;
    int fd = 0;
    FCB *fcb_ptr = NULL;
    int i = 0;
    char *filename = NULL;
    char *filetype = NULL;
    int length = 0;

    if (!_fds_init_done) {
        _fds_init();
    }

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

//    printf("open: filename=[%s], filetype=[%s]\n",
//               _filename, _filetype);


    fd = (int) _find_free_fd();

    if (fd < -1 || fd >= FILES_MAX) {
        return -1;
    }

    if (fd != -1) {
        fcb_ptr = (FCB*) &CFD[fd].fcb;
        // printf("[writing FCB at fd slot %d:0x%04x:0x%04x (%d bytes)]\n", fd, &fds[fd], fcb_ptr, sizeof(FCB));
        cpm_setFCBname(_filename, _filetype, (FCB*) fcb_ptr);
        rval = cpm_performFileOp(fop_open, fcb_ptr);
        //printf(" ret.val %02X\n", rval);

        if (rval != 0xFF) {
            //printf("File found, reading the FCB!\n");
            //fcb_ptr->resv = 0x8080;
            errno = 0;
            goto return_valid_fd;
        } else {
            if (((flags & O_RDWR) || (flags & O_WRONLY)) && (flags & O_TRUNC)) {
                //printf("fcntl: didn't exist but write was requested and O_TRUNC was set\n");
                /* create new file here */
                rval = cpm_performFileOp(fop_makeFile, fcb_ptr);
                //printf("create rval = %d\n", rval);
                if (rval == 0xFF) {
                    errno = EIO;
                    return -1;
                }

                rval = cpm_performFileOp(fop_open, fcb_ptr);
                /* fail if we can't open the file after creating it */

                if (rval == 0xFF) {
                    errno = EIO;
                    return -1;
                }

                goto return_valid_fd;
            }
            errno = ENOENT;
            return -1;
        }
    } else {
        errno = ENFILE;
        return -1;
    }

return_valid_fd:
    CFD[fd].id = fd;
    CFD[fd].oflags = flags;
    CFD[fd].offset = 0x0000;
//    fcb_ptr->seqreq = 0x00;
//    fcb_ptr->rrec = 0x0000;
//    fcb_ptr->rrecob = 0x00;
    errno = 0;
    return fd;

}


