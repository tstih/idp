#include "avdc.h"

uint16_t row_addr[24];

void avdc_wait_access() { // WARNME: disables interrupts
    __asm
    00000$:
        IN  A, (#0x36)
        AND #0x10
        JR  Z, 00000$
        EI
    00001$:
//        EI
        IN A, (#0x36)
//        DI
        AND #0x10
        JR NZ, 00001$
        DI
    __endasm;    
}

void avdc_wait_ready() {
    uint8_t ready = 0;
    while ((ready & SCN2674_STS_RDY) == 0) {
        ready = SCN2674_STS;
    }
}

void avdc_cursor_off() {
    avdc_wait_access();
    SCN2674_CMD = 0x30;
}

void avdc_cursor_on() {
    avdc_wait_access();
    SCN2674_CMD = 0x31;
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

void avdc_read_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr) { 
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU writes address into pointer registers.
    U16_U8 val;
    val.u16 = address;
    SCN2674_CMD = 0x1A;
    SCN2674_INIT = val.u8[0];
    SCN2674_INIT = val.u8[1];
    // CPU issues 'read at pointer' command. AVDC generates control signals and outputs specified address to perform requested operation. Data is copied from memory to the interface latch and AVDC sets RDFLG status to indicate that the read is completed.
    SCN2674_CMD = 0xA4;
    // CPU checks RDFLG status to see if operation is completed.
    avdc_wait_ready();
    // CPU reads data from interface latch.
    *chr = SCN2674_CHR;
    *attr = SCN2674_ATTR;
}

void avdc_write_at_pointer(uint16_t address, uint8_t chr, uint8_t attr) {
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU writes address into pointer registers.
    U16_U8 val;
    val.u16 = address;
    SCN2674_CMD = 0x1A;
    SCN2674_INIT = val.u8[0];
    SCN2674_INIT = val.u8[1];
    // CPU loads data to be written to display memory into the interface latch.
    SCN2674_CHR = chr;
    SCN2674_ATTR = attr;
    // CPU issues 'write at pointer' command. AVDC generates control signals and outputs specified address to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    SCN2674_CMD = 0xA2;
}

void avdc_write_str_at_pointer(uint16_t address, uint8_t *str, uint8_t *attr) { 
    if (attr) {
        while (*str != 0) {
            avdc_write_at_pointer(address, *str, *attr);
            address++;
            str++;
            attr++; 
        }
    } else {
        while (*str != 0) {
            avdc_write_at_pointer(address, *str, DEFAULT_ATTR);
            address++;
            str++;
        }
    }
}

void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr) { 
    avdc_write_str_at_pointer(avdc_get_pointer_cached(row, col), str, attr);
}

void avdc_set_cursor(uint8_t row, uint8_t col) {
    avdc_wait_access();
    //avdc_wait_ready();
    U16_U8 addr;
    addr.u16 = avdc_get_pointer_cached(row, col);
    SCN2674_CUR_LWR = addr.u8[0];
    SCN2674_CUR_UPR = addr.u8[1];
}

void avdc_write_at_cursor(uint8_t chr, uint8_t attr) {
    avdc_wait_access();
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait_ready();
    // CPU loads data to be written to display memory into the interface latch.
    SCN2674_CHR = chr;
    SCN2674_ATTR = attr;
    // CPU issues 'write at cursor' command. AVDC generates control signals and outputs specified address to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    SCN2674_CMD = 0xAB;
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
            avdc_write_at_cursor(*str, DEFAULT_ATTR);
            str++;
        }
    }
}

void avdc_set_cursor_write_str(uint8_t row, uint8_t col, uint8_t *str, uint8_t *attr) {
    avdc_set_cursor(row, col);
    avdc_write_str_at_cursor(str, attr);
}

void avdc_done() { 
    __asm__ ("EI");
}