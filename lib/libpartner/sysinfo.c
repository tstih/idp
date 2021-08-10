/*
 * sysinfo.c
 *
 * partner cp/m is version 3.1 (CP/M Plus),
 * this harvests system info
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 09.08.2021   tstih
 *
 */
#include <partner/sysinfo.h>
#include <sys/bdos.h>

/* sectors per track */
#define FDDSPT  36
#define HDDSPT  64
static int _getspt(uint8_t drive) {
    /* set correct drive */
    bdos(DRV_SET,drive);
    /* get sectors per track for current drive */
    bdos_ret_t res;
    bdosret(DRV_DPB,0,&res);
    uint16_t* dpb = (uint16_t*)res.rethl;
    return *dpb;
}

__sfr __at 0x23 _EF9367_CH_SIZE;
int model() {
    /* assume basic model */
    int model = M_1F;
    /* enumerate drives */
    bdos_ret_t res;
    bdosret(DRV_LOGINVEC,0,&res);
    if ((res.rethl&3)==3) { /* two drives */
        /* remember current drive */
        uint8_t currdrv=bdos(DRV_GET,0);
        int spt_a=_getspt(0), spt_b=_getspt(1);
        /* now check drive sizes */
        if (spt_a==FDDSPT && spt_b==FDDSPT) 
            model=M_2F; /* 2xFDD */
        else
            model=M_WF; /* HDD+FDD */
        /* restore current drive */
        bdos(DRV_SET,currdrv);
    } 
    /* now the graphics check, CHSIZE register must be > 0 */
    if (_EF9367_CH_SIZE!=0)
        model|=0x04;

    /* finally, return the model */
    return model;
}

/* emulator will return the ame value as the port number! */
__sfr __at 0x64 _PORT100;
bool isemu() {
    return (_PORT100 == 0x64);
}