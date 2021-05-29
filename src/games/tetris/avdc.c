#include "avdc.h"

void avdc_wait() {
    uint8_t ready = 0;
    while ((ready & SCN2674_STS_RDY) == 0) {
        ready = SCN2674_STS;
    }
}

uint16_t avdc_get_pointer(uint8_t row, uint8_t col) {
    U16_U8 row_addr;
    uint8_t dummy;
    avdc_read_at_pointer(row * 2, &row_addr.u8[0], &dummy);
    avdc_read_at_pointer(row * 2 + 1, &row_addr.u8[1], &dummy);
    return row_addr.u16 + col;
}

void avdc_read_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr) { 
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait();
    // CPU writes address into pointer registers.
    U16_U8 val;
    val.u16 = address;
    SCN2674_CMD = 0x1A;
    SCN2674_INIT = val.u8[0];
    SCN2674_INIT = val.u8[1];
    // CPU issues 'read at pointer' command. AVDC generates control signals and outputs specified address to perform requested operation. Data is copied from memory to the interface latch and AVDC sets RDFLG status to indicate that the read is completed.
    SCN2674_CMD = 0xA4;
    // CPU checks RDFLG status to see if operation is completed.
    avdc_wait();
    // CPU reads data from interface latch.
    *chr = SCN2674_CHR;
    *attr = SCN2674_ATTR;
}

void avdc_write_at_pointer(uint16_t address, uint8_t chr, uint8_t attr) {
    // CPU checks RDFLG status bit to assure that any delayed commands have been completed.
    avdc_wait();
    // CPU loads data to be written to display memory into the interface latch.
    SCN2674_CHR = chr;
    SCN2674_ATTR = attr;
    // CPU writes address into pointer registers.
    U16_U8 val;
    val.u16 = address;
    SCN2674_CMD = 0x1A;
    SCN2674_INIT = val.u8[0];
    SCN2674_INIT = val.u8[1];
    // CPU issues 'write at pointer' command. AVDC generates control signals and outputs specified address to perform requested operation. Data is copied from the interface latch into the memory. AVDC sets RDFLG status to indicate that the write is completed.
    SCN2674_CMD = 0xA2;
}

void avdc_write_str_at_pointer(uint16_t address, uint8_t *chr, uint8_t *attr) { 
    if (attr) {
        while (*chr != 0) {
            avdc_write_at_pointer(address, *chr, *attr);
            address++;
            chr++;
            attr++; 
        }
    } else {
        while (*chr != 0) {
            avdc_write_at_pointer(address, *chr, DEFAULT_ATTR);
            address++;
            chr++;
        }
    }
}

void avdc_write_str_at_position(uint8_t row, uint8_t col, uint8_t *chr, uint8_t *attr) { 
    avdc_write_str_at_pointer(avdc_get_pointer(row, col), chr, attr);
}