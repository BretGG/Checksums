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

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;
    char decodedChar;

    int dataIndex = 0;
    char data[1024] = {0};
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

    // Reading all data from input*/
    while (getBlock(block, 1) == SUCCESS && block->byteCount > 0) {
        data[dataIndex++] = block->data - '0';
    }
    char crc[32] = {0};
    copyRange(crc, data, 32);

    char crcFirstBit;
    for (int i = 31; i < dataIndex; i++) {
        crcFirstBit = crc[0];
        if (crcFirstBit == 1) {
            for (int x = 0; x < 32; x++) {
                crc[x] = crc[x] ^ divisor[x];
            }
        }


        if (i != dataIndex - 1)
            leftShift(crc, 32);

        crc[31] = data[i + 1];
    }

    // Check if CRC is correct (0)
    for (int i = 0; i < 32; i++) {
        if (crc[i] != 0)
            writeErrorBlock(block, "CRC invalid");
            break;
    }


    int bitsRead = 0;
    for (int i = 0; i < dataIndex - 33; i++) {
        decodedChar = (decodedChar << 1) | data[i];
        bitsRead++;

        if (bitsRead == 8) {
            block->byteCount = 1;
            block->data = decodedChar;
            writeBlock(block);
            decodedChar = 0;
            bitsRead = 0;
        }
    }


    closeBlock(block);
    (void) argc;
}

