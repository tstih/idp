/*
 * fio.h
 *
 * std C file support (internal) header for stdio implementation
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 28.05.2021   tstih
 *
 */
#ifndef __FIO_H__
#define __FIO_H__

struct _iobuf {
  char*     _ptr;
  int       _cnt;
  char*     _base;
  char      _flags[4];
  int       _file;
  bool 	    _eof;
  ssize_t   _limit;
}; 

typedef struct _iobuf FILE;

#endif /* __FIO_H__ */