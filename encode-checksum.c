/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: encode-checksum.c 
--
-- PROGRAM: encode-checksum
--
-- FUNCTIONS: none
--
-- NOTES:
-- This program calculates the even/odd parity checksum of the given input.
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

    int checksum[9] = {0};

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

    while ((data = fgetc(file)) != EOF){
        int parityBit = 0;

        for (int i = 0; i < 8; i++){
            checksum[i] ^= getBit(data, abs(7 - i));
            parityBit ^= getBit(data, i);
        }

        if (strcmp(argv[1], "--odd") == 0)
            parityBit ^= 1;

        for (int i = 7; i >= 0; i--){
            fputc(getBit(data, i) + '0', stdout);
        }

        // Write parity bit
        checksum[8] ^= parityBit;
        printf("%c", parityBit + '0');
    }

    // Handling checksum write
    for (int i = 0; i < 9; i++) {
        data = checksum[i];
        if (strcmp(argv[1], "--odd") == 0)
            data ^= 1;
        printf("%d", data);
    }

    fclose(file);
    (void) argc;
}
