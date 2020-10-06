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


    /*

    // Reading all data from input
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
        block->data = data + '0';
        writeBlock(block);
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

        // getBlock(block, 1);
        // if (block->data != bit)
        //     writeErrorBlock(block, "parity checksum failed");
    }

    // Check if result is all 1's
    // src: https://www.geeksforgeeks.org/count-set-bits-in-an-integer/
    // Using Brian Kernighan's algorithm
    int setBits = 0;
    while (sum) {
        sum &= (sum - 1);
        setBits++;
    }

    // If the sum's bits are not all 1's (or set)
    if (setBits != 8) {
        writeErrorBlock(block, "parity checksum failed");
    }
    */

    closeBlock(block);
    (void) argc;
}
