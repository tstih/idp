/*
 * file-xp.c
 *
 * Test file functions.
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 28.05.2021   tstih
 *
 */
#include <stdio.h>

/* Zdravljica + EOF */
char str[] = "Prijatli obrodile so trte vince nam sladko\n\rKi nam ozivlja zile, srce razjasni in oko\n\r\x1A";

void main() {
    // pointer demo to zdravljica.
    FILE* z;
 
    // Creates a file "demo_file"
    // with file acccess as write-plus mode
    z = fopen("zdrava.txt", "w");
 
    // adds content to the file
    fwrite(str , 1 , sizeof(str) , z );
 
    // closes the file pointed by demo
    fclose(z);

}