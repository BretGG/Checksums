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
    for (int i = 0; i < size - 2; i++)
        array[i] = array[i+1];
}

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

    char data[1024] = {0};
    int dataIndex = 0;

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
        for (int i = 7; i >= 0; i--)
            data[dataIndex++] = getBit(block->data, i);
    }

    for (int i = 0; i < 32; i++)
        data[dataIndex++] = 0;

    printf("data: ");
    for (int i = 0; i < dataIndex; i++)
        printf("%d", data[i]);

    printf("\n\n");

    char crc[32] = {0};
    copyRange(crc, data, 32);

    printf("divisor: ");
    for (int y = 0; y < 32; y++)
        printf("%d", divisor[y]);

    printf("\n\n");
    char crcFirstBit;

    for (int i = 31; i < dataIndex; i++) {

        crcFirstBit = crc[0];
        for (int x = 0; x < 32; x++) {
            if (crcFirstBit == 1) {
               // printf("%d - %d\n", crc[x], divisor[x]);
                crc[x] = crc[x] ^ divisor[x];
            }
        }
        for (int y = 0; y < 32; y++)
            printf("%d", crc[y]);
        leftShift(crc, 32);
        crc[31] = data[i];

        printf("\n");

    }

    closeBlock(block);
    (void) argc;
}

