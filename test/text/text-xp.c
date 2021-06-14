/*
 * text-xp.c
 *
 * Tests of text capabilities. 
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 27.03.2021   tstih
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <conio.h>

__sfr __at 0x39 SCN2674_CMD;
__sfr __at 0x38 SCN2674_INIT;

/* Original registers. */
uint8_t origIR[15]={
    0b11010000,                         /* IR0 */
    0b00101111,                         /* IR1 */
    0b00001101,                         /* IR2 */
    0b00000101,                         /* IR3 */
    0b10011001,                         /* IR4 */
    0b01001111,                         /* IR5 */
    0b00001010,                         /* IR6 */
    0b11101010,                         /* IR7 */
    0b00000000,                         /* IR8 */
    0b00110000,                         /* IR9 */
    0b11111111,                         /* IR10 */
    0b00011111,                         /* IR11 */
    0b00000000,                         /* IR12 - split reg 1 */
    0b00000000,                         /* IR13 - split reg 2*/
    0b00000000                          /* IR14 - scan lines to scroll*/
};

uint8_t testIR[15];

void write_ir(uint8_t *ir) {
    /* Reset IR to 0. */
    SCN2674_CMD=0x10;
    /* and write em all */
    for (uint8_t i=0;i<15;i++) {
        SCN2674_INIT=ir[i];
    }
}

void copy_ir(uint8_t*src, uint8_t*dst) {
    for (uint8_t i=0;i<15;i++) dst[i]=src[i];
}

void wait(char *s) {
    cputs(s);
    while (!kbhit());
}

void main() {

    /* Initialize! */
    write_ir(origIR);

    /* Display warning. */
    decdhl_top();
    cputs("Warning:\n\r");
    decdhl_bottom();
    cputs("Warning:\n\r");
    decswl();
    cputs("The software will remember the current text\n\r");
    cputs("mode. After you press a key it will execute\n\r");
    cputs("the  experiment  and you  will  observe the\n\r");
    cputs("behahiour. After  you're done, press  a key\n\r");
    cputs("again  original settings  will be restored!\n\r\n\r");
    
    
    /* 1. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Press key to set single width char.\n\r\n\r");
    testIR[0]=0b01010000;
    write_ir(testIR);
    
    /* 2. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Press key to set row scan lines to 8\n\r\n\r");
    testIR[0]=0b01000000;
    write_ir(testIR);
    
    /* 3. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Press key to set row scan lines to 4\n\r\n\r");
    testIR[0]=0b00100000;
    write_ir(testIR);

    /* 4. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Press key to set row scan lines to 1\n\r\n\r");
    testIR[0]=0b00001000;
    write_ir(testIR);

    /* 5. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Press key to Switch interlace on\n\r\n\r");
    testIR[1]=0b1010111;
    write_ir(testIR);

    /* 6. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Change chars per screen to 32\n\r\n\r");
    testIR[4]=0b00100000;
    write_ir(testIR);

    /* 7. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Change chars per screen to 64\n\r\n\r");
    testIR[4]=0b01000000;
    write_ir(testIR);

    /* 8. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Set chars per row to 128\n\r\n\r");
    testIR[5]=0b10000000;
    write_ir(testIR);

    /* 9. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Change cursor scan lines\n\r\n\r");
    testIR[6]=0b10101110;
    write_ir(testIR);

    /* 10. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Move underline position to middle of char\n\r\n\r");
    testIR[6]=0b11100010;
    write_ir(testIR);

    /* 11. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Double mode (all!)\n\r\n\r");
    testIR[14]=0b11110000;
    write_ir(testIR);

    /* 12. test. */
    copy_ir(origIR,testIR);
    wait("TEST: Enter graphics mode\n\r\n\r");
    write_ir(testIR);
    SCN2674_CMD=0b00100011;
    for(int j=0;j<160;j++) cputs(0xaa);

    /* Master reset 2x.
    /* Switch on interlaced mode. */
    wait("TEST: Press key for master reset and display on commands.\n\r\n\r");
    SCN2674_CMD=0; /* Master reset. 2x. */
    SCN2674_CMD=0; 
    SCN2674_CMD=0b00101001; /* Display on */
    /* And return original setting. */
    write_ir(origIR);
    
    wait("THE END: Sorry, can't return to CP/M. Reset the machine.\n\r");

}