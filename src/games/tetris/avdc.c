#include "avdc.h"

typedef union {
   uint16_t u16;
   uint8_t u8[2];
} addr_t;

uint16_t row_addr[26];

void avdc_init() {
    for (uint8_t i = 0; i < 26; i++) {
        row_addr[i] = avdc_get_pointer(i, 0);
        if (row_addr[i] == 0xFFFF) { // we are in Matej's emulator, so we do something that works until you scroll the screen
            for (uint8_t j = 0; j < 26; j++) {
                row_addr[j] = j * 132 + 450;
            }
            break;
        }
    }
    avdc_cursor_off();
    avdc_fill_screen(' ');
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

void avdc_fill_screen(uint8_t ch) {
    for (uint8_t row = 0; row < 26; row++) {
        avdc_fill_row(row, ch);
    }
}

void avdc_fill_row(uint8_t row, uint8_t ch) {
    avdc_wait_access();
    avdc_wait_ready();
    addr_t addr;
    addr.u16 = avdc_get_pointer_cached(row, 0); 
    // set cursor
    AVDC_CUR_LWR = addr.u8[0];
    AVDC_CUR_UPR = addr.u8[1];
    // set pointer
    addr.u16 += 132; 
    AVDC_CMD = AVDC_CMD_SET_PTR_REG;
    AVDC_INIT = addr.u8[0];
    AVDC_INIT = addr.u8[1];
    // write cursor to pointer
    AVDC_CHR = ch; 
    AVDC_ATTR = 0;
    AVDC_CMD = AVDC_CMD_WRITE_C2P;
    EI;
}

void avdc_set_mode(avdc_mode mode) { // THIS DOES NOT WORK. TEST ON REAL MACHINE!
    avdc_wait_access();
    avdc_wait_ready();
    AVDC_CMD = AVDC_CMD_SET_MODE_REG;
    AVDC_INIT = mode - 1;
    EI;
}

void avdc_restore_mode() {
    // ...
}

uint16_t avdc_get_pointer(uint8_t row, uint8_t col) {
    addr_t row_addr;
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
    addr_t val;
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
    addr_t val;
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
    addr_t addr;
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

void avdc_done() {
    avdc_fill_screen(' ');
    avdc_set_cursor(0, 0); // THIS DOES NOT WORK. TEST ON REAL MACHINE!
    //avdc_cursor_on();
}