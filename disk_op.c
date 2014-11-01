#include "sbfs.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>


//implement address testing (too large etc)
void read_block(void *buf, int block_number, int block_address){
	printf("inside read!\n");
	memcpy(buf, disk + block_number*SBFS_BLOCK_SIZE, block_address*SBFS_BLOCK_SIZE);
	printf("after read!\n");
}


//implement adress testing 
void write_block(void *buf, int block_number, int block_address){
	printf("address of disk, %d\n", disk);
	printf("block_number: %d\n", block_number);
	printf("block_address: %d\n", block_address);
	printf("write address: %d\n", disk + block_number*SBFS_BLOCK_SIZE);
	printf("write to block address in relation to disk: %d\n", block_number*SBFS_BLOCK_SIZE);
	printf("write address: %d\n", disk + block_number*SBFS_BLOCK_SIZE);
	printf("size of buffer to write to disk: %d\n", sizeof(buf));
	memcpy(disk + block_number*SBFS_BLOCK_SIZE, buf, block_address*SBFS_BLOCK_SIZE);
}