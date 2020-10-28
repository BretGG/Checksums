/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: decode-checksum.c 
--
-- PROGRAM: decode-checksum
--
-- FUNCTIONS: none
--
-- NOTES:
-- This program decodes the original message based on the given even/odd parity checksum.
----------------------------------------------------------------------------------------------------------------------*/
#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    int bitCount;
    int parityBit;

    int decodedCharIndex = 0;
    int16_t data;
    uint8_t decodedChar;
    uint8_t decodedChars[512] = {0};
    FILE* file;

    int blocks[512][9] = {0};
    int blocksIndex = 0;

    int blockData[9] = {0};

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
        bitCount++;
        data = data - '0';

        if (bitCount == 9) {
            // Add checksum
            blockData[bitCount - 1] = data;
            for (int i = 0; i < 9; i++) 
                blocks[blocksIndex][i] = blockData[i];
            blocksIndex++;

            decodedChars[decodedCharIndex++] = decodedChar;

            decodedChar = 0;
            bitCount = 0;
            continue;
        }

        blockData[bitCount - 1] = data;
        decodedChar = (decodedChar << 1) | data;
    }

    for (int i = 0; i < blocksIndex - 1; i++) {
        parityBit = 0;

        for (int x = 0; x < 8; x++){
            parityBit ^= blocks[i][x];
        }

        if (strcmp(argv[1], "--odd") == 0)
            parityBit ^= 1;

        if (blocks[i][8] != parityBit){
            printf("Parity failed");
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < 9; i++) {
        parityBit = 0;

        for (int x = 0; x < blocksIndex - 1; x++){
            parityBit ^= blocks[x][i];
        }

        if (strcmp(argv[1], "--odd") == 0)
            parityBit ^= 1;

        if (blocks[blocksIndex - 1][i] != parityBit){
            printf("Checksum failed");
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < decodedCharIndex - 1; i++)
        fputc(decodedChars[i], stdout);

    fclose(file);
    (void) argc;
}

