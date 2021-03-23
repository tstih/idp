/* remove overhead */
//#pragma output nostreams            /* stdio */
//#pragma output nofileio             /* file io */
#pragma output noprotectmsdos       /* protective dos header for file */
#pragma output noredir              /* no file redirection when parsing cmd line */
#pragma output nogfxglobals         /* no graphics */

#include <stdio.h>

main() {
    printf("Hello Partner!\n");
}