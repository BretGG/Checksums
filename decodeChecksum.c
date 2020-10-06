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
    Block block = NULL;
    int status;
    int data;
    int bitCount;
    int parityBit;

    int decodedCharIndex;
    char decodedChar;
    char decodedChars[512] = {0};

    int blocks[512][9] = {0};
    int blocksIndex = 0;

    int blockData[9] = {0};

    int checksumGood = 0;
    int checksum[9] = {0};
    int finishedChecksum[9] = {0};


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
        bitCount++;
        char data;

        if (bitCount == 9) {
            if (strcmp(argv[1], "--odd") == 0)
                parityBit ^= 1;

            if (block->data - '0' != parityBit)
                writeErrorBlock(block, "parity failed");

            blockData[bitCount - 1] = parityBit;

            for (int i = 0; i < 9; i++)
                blocks[blocksIndex][i] = blockData[i];
            blocksIndex++;

            decodedChars[decodedCharIndex++] = decodedChar;

            bitCount = 0;
            parityBit = 0;
            continue;
        }

        data = block->data - '0';

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
            writeErrorBlock(block, "checksum failed");
            break;
        }
    }

    for (int i = 0; i < decodedCharIndex - 1; i++){
        block->byteCount = 1;
        block->data = decodedChars[i];
        writeBlock(block);
    }

    closeBlock(block);
    (void) argc;
}
