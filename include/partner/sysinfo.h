/*
 * sysinfo.h
 *
 * System info.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 09.08.2021   tstih
 *
 */
#ifndef __SYSINFO_H__
#define __SYSINFO_H__

#include <stdint.h>
#include <stdbool.h>

/* get partner model, the values are also 
   bitmasks 
   bit meaning
    0  1=2xFDD, 0=1xFDD
    1  HDD
    2  Graphics */
#define M_1F    0x00                    /* 1 FDD */
#define M_2F    0x01                    /* 2 FDD */
#define M_WF    0x02                    /* HDD + FDD */
#define M_1FG   0x04                    /* 1 FDD + G */
#define M_2FG   0x05                    /* 2 FDD + G */
#define M_WFG   0x06                    /* HDD + FDD + G */
extern int model();

/* is it the emulator? */
extern bool isemu();

#endif /* __SYSINFO_H__ */