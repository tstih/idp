/*
 * fcntl.c
 *
 * std C file support functions
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

uint8_t dma_buffer[SSIZE_MAX];
FILE filehandles[FILES_MAX];
cfd_t cfd[FILES_MAX];
bool _fds_init_done = false;

void _fds_init()
{
    int i = 0;
    for (i = 0; i < FILES_MAX; i++) {
        memset(&cfd[i], 0, sizeof(cfd_t));
        cfd[i].id = -1;
        /* setup filehandles */
        filehandles[i]._file = -1;
    }
    _fds_init_done = true;
    return;
}

int  _find_free_fd()
{
    int i = 0;    
    for (i = FILES_BASE; i < FILES_MAX; i++) 
        if (cfd[i].id == -1) 
            return i;
    return -1;
}

int  _find_free_filehandle()
{
    int i = 0;
    for (i = 0; i < FILES_MAX; i++) {
        if (filehandles[i]._file == -1) {
            return i;
        }
    }
    return -1;
}