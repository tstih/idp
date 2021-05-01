/*
 * hello.c
 * hello partner program  
 *
 * tomaz stih tue mar 23 2021
 */
void hello_world() __naked { 
    __asm
        ld de, #msg
        ld c, #0x09
        call 5
        ret
msg::
        .ascii "Hello Partner.$"
    __endasm;
}

int main() {
    hello_world();
    return 0;
}