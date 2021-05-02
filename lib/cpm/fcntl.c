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
#include <string.h>

uint8_t dma_buffer[SSIZE_MAX];
FILE filehandles[FILES_MAX];
cfd_t cfd[FILES_MAX];
bool _fds_init_done = false;
int  _find_free_fd();
int  _find_free_filehandle();
void _fds_init();

void _fds_init()
{
    int i = 0;
//    printf("_fds_init()\n");
    for (i = 0; i < FILES_MAX; i++) {
        //fds[i] = -1;
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
    if (!_fds_init_done) {
        _fds_init();
    }
    for (i = FILES_BASE; i < FILES_MAX; i++) {
        if (cfd[i].id == -1) {
            return i;
        }
    }
    return -1;
}

int  _find_free_filehandle()
{
    int i = 0;
    if (!_fds_init_done) {
        _fds_init();
    }

    for (i = 0; i < FILES_MAX; i++) {
        if (filehandles[i]._file == -1) {
            return i;
        }
    }
    return -1;
}