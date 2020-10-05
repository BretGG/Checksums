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
        int bit = 0;
        for (int i = 0; i < 8; i++){
            bit ^= getBit(block->data, i);
        }

        if (strcmp(argv[1], "--odd") == 0)
            bit ^= 1;

        writeBlock(block);

        getBlock(block, 1);
        if (block->data != bit)
            writeErrorBlock(block, "parity checksum failed");

    }

    closeBlock(block);
    (void) argc;
}
