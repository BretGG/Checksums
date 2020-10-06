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

    /*
    // Reading all data from input
    int sum = 0; // used to store result from XOR addition
    while (getBlock(block, 1) == SUCCESS && block->byteCount > 0) {
        int bit = 0; // is this where current 8-bit block will be stored?
        for (int i = 0; i < 8; i++){
            bit ^= getBit(block->data, i);
        }

        // Addition using XOR
        // src: https://www.geeksforgeeks.org/add-two-numbers-without-using-arithmetic-operators/
        while (bit != 0) {

            // Getting carry by getting common bits
            // from prev and current block
            int carry = sum & bit;

            // Sum of prev and current block
            sum ^= bit;

            bit = carry << 1;
        }

        // TODO: Uncomment if this is indeed the parity check logic
        // if (strcmp(argv[1], "--odd") == 0)
        //     bit ^= 1;

        // writeBlock(block);
        // block->data = bit;
        // writeBlock(block);
    }

    // Generate checksum by performing one's complement on sum
    int checksum = ~sum;

    // Append checksum to message
    // TODO: Might need to have a createBlock function in IO lib?
    Block checksumBlock = {checksum, 1};
    writeBlock(checksumBlock);
    */

    closeBlock(block);
    (void) argc;
}
