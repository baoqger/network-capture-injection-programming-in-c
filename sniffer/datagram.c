#include <stdio.h>
#include "datagram.h"


void print_datagram(const struct datagram *d) {
    int LEN = 16; // number of bytes to display er line
    char outstr[8],     // for output formatting purposes
         ascii[LEN];    // holds textual bytes for a line

    // display all bytes in hexadecimal and textual forms
    for (unsigned int i = 0; i < d->p_len; i++) {
        if (i%LEN == 0) {
            // before moving to the next line, display accumulated bytes in character form
            if (i > 0) {
                printf("    ");
                for (int j = 0; j < LEN; j++) {
                    printf("%c", ascii[j]);
                }
            }
            // change line and display memory position of next byte in datagram
            printf("\n%.4d", i);
        }

        // display byte in hexadecimal
        printf("%.2x ", (unsigned char)d->p_data[i]);

        // format byte for textual form
        ascii[i%LEN] = ((d->p_data[i] >= 32 && d->p_data[i] <= 126) ? d->p_data[i] : '.');
    }

    // Display last line of bytes in textual form
    for (int i = LEN - d->p_len % LEN; i > 0; i--) {
        printf("   "); // print three spaces
    }
    printf("    "); // print a tab
    for (unsigned int j = 0; j < d->p_len % LEN; j++) {
        printf("%c", ascii[j]);
    }
    printf("\n");
}