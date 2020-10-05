#include "ioutil.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


int initialize(char* path, Block* newBlock) {
    int fdInput;

    if (strlen(path)) {
        fdInput = open(path, O_RDONLY);
        if (fdInput == -1) return ERR_FAILEDFILEOPEN;
    } else {
        fdInput = STDIN_FILENO;
    }

    *newBlock = (Block) malloc(sizeof(struct block));
    (*newBlock)->fdInput = fdInput;
    (*newBlock)->data = 0;
    (*newBlock)->byteCount = 0;

    return SUCCESS;
}

int getBlock(Block block, size_t byteCount) {
    block->data = 0;

    ssize_t amntRead = read(block->fdInput, &block->data, byteCount);

    if (amntRead == -1)
        return ERR_FILEREAD;

    block->byteCount = amntRead;

    return SUCCESS;
}

int writeBlock(Block block) {
    if (write(STDOUT_FILENO, &block->data, block->byteCount) == -1) return ERR_FILEWRITE;

    return SUCCESS;
}

int writeErrorBlock(Block block, char errMessage[]){ 
    off_t fileLocation = lseek(block->fdInput, 0, SEEK_CUR);
    char message[MAX_ERROR_MESSAGE] = "";

    if (fileLocation == -1) // stdin from keyboard file location is not applicable
        sprintf(message, "ERROR - BYTES: %02x MESSAGE: %s\n", block->data, errMessage);
    else
        sprintf(message, "ERROR - BYTES: %02x FILE_OFFSET: %ld MESSAGE: %s\n", block->data, fileLocation, errMessage);

    if (block->errCount >= MAX_ERROR_COUNT - 1) {
        strcpy(block->errMessages[MAX_ERROR_COUNT - 1], "Errors all the way down\n");
    }
    else {
        int index = block->errCount++;
        strcpy(block->errMessages[index], message);
    }

    return SUCCESS;
}

int closeBlock(Block block) {
    if (block->fdInput != -1 && block->fdInput != STDIN_FILENO) 
        if (close(block->fdInput) == -1) return ERR_FILECLOSE;

    // Write error messages
    write(STDOUT_FILENO, "\n", sizeof(char));
    for (size_t i = 0; i < block->errCount; i++) {
        write(STDOUT_FILENO, block->errMessages[i], strlen(block->errMessages[i]));
    }

    free(block);

    return SUCCESS;
 }


