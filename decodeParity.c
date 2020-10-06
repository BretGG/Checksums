#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status = 0;
    int bitCount = 0;
    int parityBit = 0;
    char decodedChar = 0;

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
                writeErrorBlock(block, "parity checksum failed");

            block->data = decodedChar;
            writeBlock(block);
            bitCount = 0;
            parityBit = 0;
            continue;
        }

        data = block->data - '0';
        parityBit ^= data;
        decodedChar = (decodedChar << 1) | data;
    }

    closeBlock(block);
    (void) argc;
}
