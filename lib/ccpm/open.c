/*
 * read.c
 *
 * standard C file open function
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
#include <stdio.h>
#undef FILE /* Make sure you don't use stdio.h version */
#include <fio.h>
#include <string.h>
#include <io.h>

int open(const char *pathname, int flags)
{
    char _filename[9];
    char _filetype[4];
    char *ptr = NULL;
    int rval = 0;
    int fd = 0;
    fcb_t *fcb_ptr = NULL;
    int i = 0;
    char *filename = NULL;
    char *filetype = NULL;
    int length = 0;

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

    fd = (int) _find_free_fd();

    if (fd < -1 || fd >= FILES_MAX) {
        errno = ENFILE; /* File table overflow */
        return -1;
    }

    if (fd != -1) {
        fcb_ptr = (fcb_t*) &cfd[fd].fcb;
        cpm_set_fcb_name(_filename, _filetype, (fcb_t*) fcb_ptr);
        rval = cpm_perform_file_op(fop_open, fcb_ptr);

        if (rval != 0xFF) {
            errno = 0;
            goto return_valid_fd;
        } else {
            if (((flags & O_RDWR) || (flags & O_WRONLY)) && (flags & O_TRUNC)) {
                 /* create new file here */
                rval = cpm_perform_file_op(fop_make_file, fcb_ptr);
                if (rval == 0xFF) {
                    errno = EIO;
                    return -1;
                }

                rval = cpm_perform_file_op(fop_open, fcb_ptr);

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
    cfd[fd].id = fd;
    cfd[fd].oflags = flags;
    cfd[fd].offset = 0x0000;
    errno = 0;
    return fd;

}