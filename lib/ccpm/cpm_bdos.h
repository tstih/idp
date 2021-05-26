/*
 * cpm_bdos.h
 *
 * bdos calls for cp/m
 * 
 * NOTES:
 *  From http://www.tassos-oak.com/NB2/toolbook.html
 *  The BDOS is located at some address in high storage; that is, it begins at an address higher than the end of our program.
 *  We don't know what that address is. It varies from system to system, and it can vary from one command to the next in a single
 *  system, although it normally doesn't. However, there is always a jump instruction at location 5 (address 0005h) which is
 *  aimed at the lowest instruction in the BDOS.
 *  To call upon the BDOS, we need only place the correct arguments in registers and call location 5.
 *
 *  One BDOS argument is a service number that specifies what we want done. It is always passed in register C.
 *  The other argument depends on the service being requested, but if it's a byte, it is passed in register E, and if a word, in DE.
 *  The second argument is the byte to be typed; it's passed in register E.
 *
 * Copyright (C) Douglas W. Goodall
 * For Non-Commercial use by N8VEM
 * 5/10/2011 dwg - initial version
 * hkzlab - modified version
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.04.2021   tstih
 *
 */
#ifndef __CPM_BDOS_H__
#define __CPM_BDOS_H__

#include <stdint.h>

/* exit codes */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// BDOS function codes
// See here: http://www.seasip.demon.co.uk/Cpm/bdos.html
#define S_RESET		 0
#define C_READ       1
#define C_WRITE      2
#define A_READ       3
#define A_WRITE      4
#define L_WRITE      5
#define C_RAWIO      6
#define GETIOBYTE    7
#define SETIOBYTE    8
#define C_WRITESTR   9
#define C_READSTR    10
#define DRV_ALLRESET 13
#define DRV_SET      14
#define F_OPEN       15
#define F_CLOSE      16
#define F_SMATCH     17
#define F_NMATCH     18
#define F_DELETE     19
#define F_READ       20
#define F_WRITE      21
#define F_MAKE       22
#define F_RENAME     23
#define DRV_LOGINVEC 24
#define DRV_GET      25
#define F_DMAOFF     26
#define DRV_ALLOCVEC 27
#define DRV_SETRO    28
#define DRV_ROVEC    29
#define F_ATTRIB     30
#define DRV_DPB      31
#define F_USERNUM    32
#define F_READRAND   33
#define F_WRITERAND  34
#define F_SIZE       35
#define F_RANDREC    36
#define DRV_RESET    37
#define F_WRITEZF    40


/* BDOS call parameters */
typedef struct bdos_call_s {
	uint8_t func8;
	uint16_t  parm16;
} bdos_call_t;

/* call bdos, ignore result */
extern uint8_t cpmbdos(bdos_call_t *p);

/* call bods, return results to ret_ba and ret_hl */
extern uint8_t cpmbdos_extn(bdos_call_t *p, uint16_t* ret_ba, uint16_t *ret_hl);

#endif /* __CPM_BDOS_H__ */