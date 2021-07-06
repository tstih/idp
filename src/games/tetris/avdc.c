#include "avdc.h"

uint16_t row_addr[24];

void avdc_init() {
    for (uint16_t i = 0; i < 24; i++) {
        row_addr[i] = avdc_get_pointer(i, 0);
    }
    avdc_cursor_off();
}

void avdc_done() {
    avdc_cursor_on();
}

void avdc_wait_access() { // WARNME: disables interrupts
    uint8_t status = 0;
    while ((status & AVDC_ACCESS_FLAG) == 0) {
        status = AVDC_ACCESS;
    }
    EI;
    while ((status & AVDC_ACCESS_FLAG) != 0) {
        status = AVDC_ACCESS;
    }
    DI;
}

void avdc_wait_ready() {
    uint8_t status = 0;
    while ((status & AVDC_STATUS_READY) == 0) {
        status = AVDC_STATUS;
    }
}

void avdc_cursor_off() {
    avdc_wait_access();
    AVDC_CMD = AVDC_CMD_CUR_OFF;
    EI;
}

void avdc_cursor_on() {
    avdc_wait_access();
    AVDC_CMD = AVDC_CMD_CUR_ON;
    EI;
}

uint16_t avdc_get_pointer(uint8_t row, uint8_t col) {
    U16_U8 row_addr;
    uint8_t dummy;
    avdc_read_at_pointer(row * 2, &row_addr.u8[0], &dummy);
    avdc_read_at_pointer(row * 2 + 1, &row_addr.u8[1], &dummy);
    return row_addr.u16 + col;
}

uint16_t avdc_get_pointer_cached(uint8_t row, uint8_t col) {
    return row_addr[row] + col;
}

void avdc_read_at_pointer(uint16_t addr, uint8_t *chr, uint8_t *attr) { 
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU writes addr into pointer registers.
    U16_U8 val;
    val.u16 = addr;
    AVDC_CMD = AVDC_CMD_SET_PTR_REG;
    AVDC_INIT = val.u8[0];
    AVDC_INIT = val.u8[1];
    // CPU issues 'read at pointer' command. AVDC generates control signals and outputs specified addr to perform requested operation. Data is copied from memory to the interface latch and AVDC sets RDFLG status to indicate that the read is completed.
    AVDC_CMD = AVDC_CMD_READ_AT_PTR;
    // CPU checks RDFLG status to see if operation is completed.
    avdc_wait_ready();
    // CPU reads data from interface latch.
    *chr = AVDC_CHR;
    *attr = AVDC_ATTR;
    EI;
}

void avdc_write_at_pointer(uint16_t addr, uint8_t chr, uint8_t attr) {
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU writes addr into pointer registers.
    U16_U8 val;
    val.u16 = addr;
    AVDC_CMD = AVDC_CMD_SET_PTR_REG;
    AVDC_INIT = val.u8[0];
    AVDC_INIT = val.u8[1];
    // CPU loads data to be written to display memory into the interface latch.
    AVDC_CHR = chr;
    AVDC_ATTR = attr;
    // CPU issues 'write at pointer' command. AVDC generates control signals and outputs specified addr to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    AVDC_CMD = AVDC_CMD_WRITE_AT_PTR;
    EI;
}

void avdc_write_str_at_pointer(uint16_t addr, uint8_t *str, uint8_t *attr) { 
    if (attr) {
        while (*str != 0) {
            avdc_write_at_pointer(addr, *str, *attr);
            addr++;
            str++;
            attr++; 
        }
    } else {
        while (*str != 0) {
            avdc_write_at_pointer(addr, *str, AVDC_DEFAULT_ATTR);
            addr++;
            str++;
        }
    }
}

void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr) { 
    avdc_write_str_at_pointer(avdc_get_pointer_cached(row, col), str, attr);
}

void avdc_set_cursor(uint8_t row, uint8_t col) {
    avdc_wait_access();
    U16_U8 addr;
    addr.u16 = avdc_get_pointer_cached(row, col);
    AVDC_CUR_LWR = addr.u8[0];
    AVDC_CUR_UPR = addr.u8[1];
    EI;
}

void avdc_write_at_cursor(uint8_t chr, uint8_t attr) {
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU loads data to be written to display memory into the interface latch.
    AVDC_CHR = chr;
    AVDC_ATTR = attr;
    // CPU issues 'write at cursor' command. AVDC generates control signals and outputs specified addr to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    AVDC_CMD = AVDC_CMD_WRITE_AT_CUR;
    EI;
}

void avdc_write_str_at_cursor(uint8_t *str, uint8_t *attr) {
    if (attr) {
        while (*str != 0) {
            avdc_write_at_cursor(*str, *attr);
            str++;
            attr++; 
        }
    } else {
        while (*str != 0) {
            avdc_write_at_cursor(*str, AVDC_DEFAULT_ATTR);
            str++;
        }
    }
}

void avdc_set_cursor_write_str(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr) {
    avdc_set_cursor(row, col);
    avdc_write_str_at_cursor(str, attr);
}