/*
 * io.h
 *
 * std C file support (internal) header
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 03.05.2021   tstih
 *
 */
#ifndef __IO_H__
#define __IO_H__

#define FILES_BASE                  3
#define FILES_MAX                   16 
#define MODULE_SIZE                 524288
#define EXTENTS_PER_MODULE          32
#define KILOBYTE                    1024
#define EXTENT_SIZE                 (16 * KILOBYTE)

typedef struct cfd_s {
    int id;
    int oflags;                         /* FIXME: should be off_t ? */
    uint32_t offset;
    fcb_t fcb;
} cfd_t;

extern cfd_t cfd[FILES_MAX];
extern uint8_t dma_buffer[SSIZE_MAX];

extern int _find_free_fd();
extern int _find_free_filehandle();

#endif /* __IO_H__ */