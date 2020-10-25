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

int main(int argc, char* argv[]) {
    int bitCount;
    int parityBit;

    int decodedCharIndex = 0;
    char data;
    char decodedChar;
    char decodedChars[512] = {0};
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

        if (bitCount == 9) {
            if (strcmp(argv[1], "--odd") == 0)
                parityBit ^= 1;

            if (data - '0' != parityBit) {
                printf("Parity Failed");
                exit(EXIT_SUCCESS);
            }

            blockData[bitCount - 1] = parityBit;

            for (int i = 0; i < 9; i++) 
                blocks[blocksIndex][i] = blockData[i];
            blocksIndex++;

            decodedChars[decodedCharIndex++] = decodedChar;

            bitCount = 0;
            parityBit = 0;
            continue;
        }

        data = data - '0';
        blockData[bitCount - 1] = data;
        parityBit ^= data;
        decodedChar = (decodedChar << 1) | data;
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

