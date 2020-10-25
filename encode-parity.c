/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: encode-parity.c 
--
-- PROGRAM: encode-parity
--
-- FUNCTIONS: none
--
-- NOTES:
-- This program encodes 8-bit values and appends a parity bit to the data block using even or odd parity.
----------------------------------------------------------------------------------------------------------------------*/
#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getBit(char byte, int bitIndex) {
    if (bitIndex > 7) 
        return -1;

    return (byte >> bitIndex) & 0x01;
}
int main(int argc, char* argv[]) {
    char data;
    FILE* file;

    if (argc < 2) {
        perror("Requires 2 or 3 args");
        exit(EXIT_FAILURE);
    }
    
    if (argc > 2) {
        file = fopen(argv[2], "r");
    }
    else {
        file = stdin;
    }

    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Reading all data from input*/
    while ((data = fgetc(file)) != EOF){
        int parityBit = 0;

        for (int i = 0; i < 8; i++){
            parityBit ^= getBit(data, i);
        }

        // Checking parity flag
        if (strcmp(argv[1], "--odd") == 0)
            parityBit ^= 1;

        for (int i = 7; i >= 0; i--){
            printf("%c", getBit(data, i) + '0');
        }

        // Write parity bit
        printf("%c", parityBit + '0');
    }

    fclose(file);
    (void) argc;
}
