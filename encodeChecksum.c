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

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;
    int data;

    int checksum[9] = {0};

    if (argc < 2) {
        perror("Requires 2 or 3 args");
        exit(EXIT_FAILURE);
    }

    if (argc > 2) {
        status = initialize(argv[2], &block);
    }
    else {
        status = initialize("", &block);
    }


    if (status == -1) {
        perror("initialize");
        exit(EXIT_FAILURE);
    }

    // Reading all data from input*/
    while (getBlock(block, 1) == SUCCESS && block->byteCount > 0) {
        int parityBit = 0;
        data = block->data;

        for (int i = 0; i < 8; i++){
            checksum[i] ^= getBit(data, abs(7 - i));
            parityBit ^= getBit(data, i);
        }

        if (strcmp(argv[1], "--odd") == 0)
            parityBit ^= 1;

        for (int i = 7; i >= 0; i--){
            block->data = getBit(data, i) + '0';
            writeBlock(block);
        }

        // Write parity bit
        checksum[8] ^= parityBit;
        block->data = parityBit + '0';
        writeBlock(block);
    }

    // Handling checksum write
    for (int i = 0; i < 9; i++) {
        data = checksum[i];
        if (strcmp(argv[1], "--odd") == 0)
            data ^= 1;
        block->byteCount = 1;
        block->data = data + '0';
        writeBlock(block);
    }

    closeBlock(block);
    (void) argc;
}
