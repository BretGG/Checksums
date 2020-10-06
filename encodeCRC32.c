#include "ioutil.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int crc32b(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   printf("%c\n", message[0]);
   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0x04C11DB7 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}
unsigned char getCRC(unsigned char message[], unsigned char length)
{
  unsigned char i, j, crc = 0;
 
  for (i = 0; i < length; i++)
  {
    crc ^= message[i];
    for (j = 0; j < 8; j++)
    {
      if (crc & 1)
        crc ^= 0x04C11DB7;
      crc >>= 1;
    }
  }
  return crc;
}


int main(int argc, char* argv[]) {
    Block block = NULL;
    int status;

    int data[1024] = {0};
    int dataIndex = 0;

    if (argc < 2) {
        perror("Requires 2 args");
        exit(EXIT_FAILURE);
    }

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

    for (int i = 0; i < dataIndex; i++)
        printf("%d", data[i]);


    closeBlock(block);
    (void) argc;
}
