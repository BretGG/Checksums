#ifndef IOUTIL
#define IOUTIL

#include <stddef.h>

#define SUCCESS 0
#define FILE_OPENED 1
#define ERR_FAILEDFILEOPEN -1
#define ERR_NOFILEOPEN -2
#define ERR_FILEREAD -3
#define ERR_FILEWRITE -4
#define ERR_FILECLOSE -5

#define BLOCK_SIZE 1 // size in bytes
#define MAX_ERROR_COUNT 1024 // size in bytes
#define MAX_ERROR_MESSAGE 1024 // size in bytes

/* Block data structure holding values returned from getBlock */
typedef struct block {
    int data;
    size_t byteCount;
    int fdInput;
    size_t errCount;
    char errMessages[MAX_ERROR_COUNT][MAX_ERROR_MESSAGE];
} *Block;

/* Loads a file to be used by ioutil */
int initialize(char* path, Block* newBlock);

/* Reads the next available block of data assigning it to the Block pointer */
int getBlock(Block, size_t byteCount);

/* Writes byteCount worth of data from Block to file opened using loadFile */
int writeBlock(Block);

/* Formats and writes block to standard out, format - ERROR: file location, file name, error description*/
int writeErrorBlock(Block, char errMessage[]);

/* Closes any file opened by loadFile */
int closeBlock(Block block);

#endif
