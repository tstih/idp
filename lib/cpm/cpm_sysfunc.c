/*
 * cpm_sysfunc.c
 *
 * cp/m system functions implementation
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.04.2021   tstih
 *
 */
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "cpm_sysfunc.h"
#include "cpmbdos.h"

#define READLINE_BUF_SIZE 80

static bdos_call_t bdos_readstr;
static rs_buffer_t rs_buf;

static uint16_t ret_ba, ret_hl;
extern int rtargc;
extern uint16_t heapaddr;

uint16_t get_ret_ba()
{
    return ret_ba;
}

uint16_t get_ret_hl()
{
    return ret_hl;
}

uint16_t get_stktop()
{
    return heapaddr;
}

void _cpm_sysfunc_init() {
    /* standard CP/M command buffer at 0x0080 */
    /* this function returns the correct value for argc */
    /* FIXME: this is such a hack right now - we save the argv pointers in the "UNUSED" section at 0x0050*/
    char *argptr = (char *) 0x0080;
    unsigned char _argbytes;
    int i = 0;
    bool within_arg = false;
    int _argc = 1;
    /* first word is argv0, which we don't have,
       but we'll fill it with NULL anyway */
    uint16_t *argv_ptr = (uint16_t*) 0x0052;    
    /* this is apparently unused in CP/M 2.2 so we will use it 
       as pointers for argv */
    memset((void *) 0x50, 0, 0x10);
    _argbytes=argptr[0];
    for (i = 1; i < (_argbytes+2 ); i++) {
        switch(argptr[i]) {
        case '\0':
            if (within_arg) 
                _argc++;
            /* end of argument string */
            within_arg = false;
            if (_argc >= MAX_ARGS) {
                goto args_done;
            }
            break;
        case ' ':
            argptr[i] = '\0';
            if (within_arg)
                _argc++;
            if (_argc >= MAX_ARGS) {
                goto args_done;
            }
            within_arg = false;
            break;
        default:
            if (!within_arg) {
                /* very hacky */
                argv_ptr[0] = 0x80+i;
                argv_ptr[1] = 0x00;
                argv_ptr++;
                within_arg = true;
            } 
            break;
        }
    }

    // Initialize READSTR BDOS call
    bdos_readstr.func8 = C_READSTR;
    bdos_readstr.parm16 = (uint16_t)&rs_buf;

args_done:
    rtargc = (int )_argc;
}

char *cpm_gets(char *p) {
    memset(rs_buf.bytes, 0, sizeof(rs_buf.bytes));
    rs_buf.size = sizeof(rs_buf.bytes);
    rs_buf.len = 0;
    cpmbdos_extn(&bdos_readstr, &ret_ba, &ret_hl);
    strcpy(p, rs_buf.bytes);
    return p;
}


char cpm_getchar(void) {
    char f = 0;
    bdos_call_t cread = { C_READ, { (uint16_t)0 } };
    f = cpmbdos_extn(&cread, &ret_ba, &ret_hl);
    return f;
}

char cpm_getchar_nonblock(void) {
    char f = 0;
    bdos_call_t cread = { C_RAWIO, { (uint16_t) 0x00ff } };
    f = cpmbdos_extn(&cread, &ret_ba, &ret_hl);
    return f;
}


void cpm_putchar(char c) {
    bdos_call_t cwrite = { C_WRITE, { (uint16_t)c } };
    cpmbdos_extn(&cwrite, &ret_ba, &ret_hl);
}

void cpm_set_dma_addr(uint16_t addr) {
    bdos_call_t fdma = { F_DMAOFF, {addr} };

    cpmbdos_extn(&fdma, &ret_ba, &ret_hl);
}

uint8_t cpm_get_cur_drive(void) {
    bdos_call_t drv = { DRV_GET, { 0 } };

    return cpmbdos_extn(&drv, &ret_ba, &ret_hl);
}

uint8_t cpm_reset_drives(void) {
    bdos_call_t drv = { DRV_ALLRESET, { 0 } };

    return cpmbdos_extn(&drv, &ret_ba, &ret_hl);
}

uint8_t cpm_set_cur_drive(uint8_t drive) {
    bdos_call_t drv = { DRV_SET, { drive } };

    return cpmbdos_extn(&drv, &ret_ba, &ret_hl);
}

void cpm_set_fcb_name(char *fname, char *ftype, fcb_t *cb) {
    int idx;
    char c;
    for (idx = 0; (idx < 8) && (fname[idx] != '\0'); idx++) {
        c = fname[idx] & 0x7F;
        if (c >= 0x61 && c <= 0x7a)
            c -= 0x20;
        cb->filename[idx] = c;
    }

    while (idx < 8) {
        cb->filename[idx] = ' '; // Pad the filename
        idx++;
    }

    for (idx = 0; (idx < 3) && (ftype[idx] != '\0'); idx++) {
        c = ftype[idx] & 0x7F;
        if (c >= 0x61 && c <= 0x7a)
            c -= 0x20;

        cb->filetype[idx] = c;
    }

    while (idx < 3) {
        cb->filetype[idx] = ' '; // Pad the filetype
        idx++;
    }
}

uint8_t cpm_performFileOp(file_operation fop, fcb_t *cb) {
    bdos_call_t call = { 0, {(uint16_t)cb} };

    switch (fop) {
    case fop_open:
        call.func8 = F_OPEN;
        break;
    case fop_close:
        call.func8 = F_CLOSE;
        break;
    case fop_first_name_match:
        call.func8 = F_SMATCH;
        break;
    case fop_next_match:
        call.func8 = F_NMATCH;
        break;
    case fop_make_file:
        call.func8 = F_MAKE;
        break;
    case fop_del_file:
        call.func8 = F_DELETE;
        break;
    case fop_set_file_attr:
        call.func8 = F_ATTRIB;
        break;
    case fop_read_seq_record:
        call.func8 = F_READ;
        break;
    case fop_write_seq_record:
        call.func8 = F_WRITE;
        break;
    case fop_read_rand_record:
        call.func8 = F_READRAND;
        break;
    case fop_write_rand_record:
        call.func8 = F_WRITERAND;
        break;
    case fop_upd_rand_rec_ptr:
        call.func8 = F_RANDREC;
        break;
    case fop_calc_file_size:
    default:
        call.func8 = F_SIZE;
        break;
    }

    return cpmbdos_extn(&call, &ret_ba, &ret_hl);
}

void cpm_reset(void) {
    bdos_call_t sreset = { S_RESET, { (uint16_t)0 } };
    cpmbdos_extn(&sreset, &ret_ba, &ret_hl);
}

bool continue_or_exit ()
{
    unsigned char chr[READLINE_BUF_SIZE];
    puts("  More? [Y] continue [N] exit: ");
    memset (&chr, 0, READLINE_BUF_SIZE);
    cpm_gets (chr);
    return ((chr[0] == 'Y' || chr[0] == 'y') ? true : false);
}