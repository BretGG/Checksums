#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

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
        printf("%02x - %lu\n", block->data, block->byteCount);
        writeErrorBlock(block, "decode");
    }

    // Writing data
    short test[] = {64, 128, 256, 512, 1024};
    for (size_t i = 0; i < 5; i++) {
        block->data = test[i];
        block->byteCount = sizeof(test[i]);
        writeBlock(block);
    }

    closeBlock(block);
    (void) argc;
}
