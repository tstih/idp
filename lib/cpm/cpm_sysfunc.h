/*
 * cpm_sysfunc.h
 *
 * cp/m system functions header
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
#ifndef __CPM_SYSFUNC_H__
#define __CPM_SYSFUNC_H__

#include <stdbool.h>
#include <stdint.h>

#define MAX_ARGS 8 

typedef struct rs_buffer_s {
    uint8_t size;
    uint8_t len;
    char bytes[80];
} rs_buffer_t;

typedef struct fcb_s {
	uint8_t drive;              /* 0 -> Searches in default disk drive */
	char filename[8];           /* file name ('?' means any char) */
	char filetype[3];           /* file type */
	uint8_t ex;                 /* extent */
   	uint16_t resv;              /* reserved for CP/M */
	uint8_t rc;                 /* records used in extent */
	uint8_t alb[16];            /* allocation blocks used */
	uint8_t seqreq;             /* sequential records to read/write */
	uint16_t rrec;              /* random record to read/write */ 
	uint8_t rrecob;             /* random record overflow byte (MS) */
} fcb_t; /* File Control Block */

typedef struct cpm_dir_s {
	uint8_t status;
	char filename[8];
	char filetype[3];
	uint8_t xl;
	uint8_t bc;
	uint8_t xh;
	uint8_t rc;
	uint8_t alb[16];
} com_dir_t;

typedef enum {
	fop_open,
	fop_close,
	fop_first_name_match,
	fop_next_match,
	fop_make_file,
	fop_del_file,
	fop_set_file_attr,
	fop_read_seq_record,
	fop_write_seq_record,
	fop_read_rand_record,
	fop_write_rand_record,
	fop_upd_rand_rec_ptr,
	fop_calc_file_size
} file_operation;

/* required externs */
extern int rtargc;
extern uint16_t heapaddr;

extern void cpm_sysfunc_init();
extern void cpm_reset(void);
extern char *cpm_gets(char *p);
extern char cpm_getchar(void);
extern void cpm_putchar(char c);
extern char cpm_getchar_nonblock(void);
extern uint8_t cpm_perform_file_op(file_operation fop, fcb_t *cb);
extern uint8_t cpm_delete_file(fcb_t *cb);
extern uint8_t cpm_set_file_attribs(fcb_t *cb);
extern void cpm_set_dma_addr(uint16_t addr);
extern uint8_t cpm_get_cur_drive(void);
extern uint8_t cpm_set_cur_drive(uint8_t drive);
extern uint8_t cpm_reset_drives(void);
extern void cpm_set_fcb_name(char *fname, char *ftype, fcb_t *cb);

/* return saved values of BA or HL */
extern uint16_t get_ret_ba();
extern uint16_t get_ret_hl();
extern uint16_t get_stktop();

extern bool continue_or_exit ();

#endif /* __CPM_SYSFUNC_H__ */