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
    for (int i = 0; i < size; i++)
        target[i] = from[i];
}

void leftShift(char array[], size_t size) {
    for (int i = 0; i < size - 1; i++)
        array[i] = array[i+1];
}

int getNextByte(Block block, char *data, int* paddingCount) {
    printf("\nPadding: %d\n", *paddingCount);
    if (*paddingCount >= 32) 
        return 0;

    if (*paddingCount > 0) {
        for (int x = 0; x < 8; x++) {
            data[x] = 0;
            *paddingCount += 1;
        }
        return 1;
    }

    if (getBlock(block, 1) != SUCCESS) {
        perror("Failed to read next byte");
        exit(EXIT_FAILURE);
    }

    // printf("\nNext Block: %lu, %d, %d\n", block->byteCount, block->data, *paddingCount);

    if (block->byteCount > 0) {
        for (int x = 7; x >= 0; x--)
            data[7 - x] = getBit(block->data, x);
    } else {
        for (int x = 0; x < 8; x++) {
            *paddingCount += 1;
            data[x] = 0;
        }
    }   

    return 1;
}

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

    char crc[33] = {0};
    char nextByte[8] = {0};
    int dataIndex = 0; int dataIndexWithoutPadding = 0;
    int paddingCount = 0;
    // char divisor[] = {0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,1,1};
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

    //if (block->fdInput != STDIN_FILENO)
     //   dataIndex -= 8;

    // Initial data
    int y = 0;
    for (int i = 0; i < 4; i++) {
        if (getNextByte(block, nextByte, &paddingCount)) {
            printf("\nNext Byte:");
            for (int x = 0; x < 8; x++) {
                    printf("%d", nextByte[x]);
            }
            printf("\n");

            for (int x = 0; x < 8; x++) {
                crc[y++] = nextByte[x];
                
                if (paddingCount <= 0) {
                    // Write encoded characters
                    block->byteCount = 1;
                    block->data = nextByte[x] + '0';
                    writeBlock(block);
                }           
            }       
        }       
    }

    //printf("\n\n\n\n");

    /*printf("\n\n\n");*/
    /*for (int x = 0; x < 32; x++)*/
        /*printf("%d", crc[x]);*/
    /*printf("\n\n\n");*/

    printf("\n\n\n:");
    printf("\nCRC:");
    for (int i = 0; i < 33; i++)
        printf("%d", crc[i]);
    printf("\n\n\n:");
        
    char crcFirstBit;
    int firstRun = 1;
    while(getNextByte(block, nextByte, &paddingCount)) {
        printf("\nNext Byte:");
        for (int x = 0; x < 8; x++) {
                printf("%d", nextByte[x]);
        }
        printf("\n");

        if (firstRun){
            crc[32] = nextByte[0];
        }

        for (int i = 0; i < 8; i++) {
            crcFirstBit = crc[0];
            if (crcFirstBit == 1)
                for (int x = 0; x < 33; x++) 
                    crc[x] = crc[x] ^ divisor[x];

            if (paddingCount < 32) {
                // Write encoded characters
                block->byteCount = 1;
                block->data = nextByte[i] + '0';
                // writeBlock(block);
            }           

            if (paddingCount < 32 || i < 8) {
                leftShift(crc, 33);
                crc[32] = nextByte[i];
            }

            printf("\nCRC:");
            for (int i = 0; i < 33; i++)
                printf("%d", crc[i]);
            printf("\nDIV:");
            for (int i = 0; i < 33; i++)
                printf("%d", divisor[i]);
        }
    }

    leftShift(crc, 33);

    // Write CRC
    for (int i = 0; i < 32; i++) {
        block->byteCount = 1;
        block->data = crc[i] + '0';
        writeBlock(block);
    }

    closeBlock(block);
    (void) argc;
}

