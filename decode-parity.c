/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: decode-parity.c 
--
-- PROGRAM: decode-parity
--
-- FUNCTIONS: none
--
-- NOTES:
-- This program decodes 9-bit values from even or odd parity. It also outputs an error message if an error 
-- is encountered during parity checking.
----------------------------------------------------------------------------------------------------------------------*/
#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int bitCount = 0;
    int parityBit = 0;
    char decodedChar = 0;
    char data;
    FILE* file;


    if (argc < 2) {
        perror("Requires 2 or 3 args");
        exit(EXIT_FAILURE);
    }
    
    if (argc > 2) {
        file = fopen(argv[2], "r");
    }
    else {
        file = stdin;
    }

    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Reading all data from input*/
    while ((data = fgetc(file)) != EOF){
        bitCount++;

        if (bitCount == 9) {
            if (strcmp(argv[1], "--odd") == 0)
                parityBit ^= 1;

            if (data - '0' != parityBit) {
                printf("Parity failed");
                exit(SUCCESS);
            }

            data = decodedChar;
            printf("%c", data);
            bitCount = 0;
            parityBit = 0;
            continue;
        }

        data = data - '0';
        parityBit ^= data;
        decodedChar = (decodedChar << 1) | data;
    }

    fclose(file);
    (void) argc;
}
