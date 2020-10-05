#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

    if (argc < 2) {
        perror("Requires 2 or 3 args");
        exit(EXIT_FAILURE);
    }

    if (argc > 2) {
        status = initialize(argv[1], &block);
    }
    else {
        status = initialize("", &block);
    }


    if (status == -1) {
        perror("initialize");
        exit(EXIT_FAILURE);
    }

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

    closeBlock(block);
    (void) argc;
}
