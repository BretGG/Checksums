/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: encode-crc32.c 
--
-- PROGRAM: decode-crc32
--
-- FUNCTIONS: main, leftShift, copyRange
--
-- NOTES:
-- This program creates a CRC using the supplied message.
----------------------------------------------------------------------------------------------------------------------*/
#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CRC_POLY 0x04C11DB7

void copyRange(char target[], char from[], size_t size) {
    for (size_t i = 0; i < size; i++)
        target[i] = from[i];
}

void leftShift(char array[], size_t size) {
    for (size_t i = 0; i < size - 1; i++)
        array[i] = array[i+1];
}


int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

    char data[65536] = {0};
    int dataIndex = 0;
    int dataIndexWithoutPadding = 0;
    char divisor[] = {1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,1,1};

    if (argc > 1) {
        status = initialize(argv[1], &block);
    }
    else {
        status = initialize("", &block);
    }


    if (status == -1) {
        perror("initialize");
        exit(EXIT_FAILURE);
    }

    while (getBlock(block, 1) == SUCCESS && block->byteCount > 0) {
        for (int i = 7; i >= 0; i--)
            data[dataIndex++] = getBit(block->data, i);
    }

    if (block->fdInput != STDIN_FILENO)
        dataIndex -= 8;
    
    dataIndexWithoutPadding = dataIndex;

    for (int i = 0; i < 33; i++)
        data[dataIndex++] = 0;

    char crc[33] = {0};
    copyRange(crc, data, 33);

    char crcFirstBit;
    for (int i = 32; i < dataIndex - 1; i++) {
        crcFirstBit = crc[0];
        if (crcFirstBit == 1) {
            for (int x = 0; x < 33; x++) {
                crc[x] = crc[x] ^ divisor[x];
            }
        }

        if (i < dataIndex - 2)
            leftShift(crc, 33);

        if (i < dataIndex - 2)
            crc[32] = data[i + 1];

    }

    leftShift(crc, 33);

    // Write original data
    block->byteCount = 1;
    for (int i = 0; i < dataIndexWithoutPadding; i++) {
        block->data = data[i] + '0';
        writeBlock(block);
    }

    // Write CRC
    for (int i = 0; i < 32; i++) {
        block->data = crc[i] + '0';
        writeBlock(block);
    }

    closeBlock(block);
    (void) argc;
}

