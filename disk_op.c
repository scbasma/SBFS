#include "sbfs.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

char *disk;
void load_disk(){
	disk = malloc(SBFS_DISK_SIZE);
}


//implement address testing (too large etc)
void read_block(void *buf, int block_number){
	printf("inside read!\n");
	memcpy(buf, disk + block_number*SBFS_BLOCK_SIZE, SBFS_BLOCK_SIZE);
	printf("after read!\n");
}


//implement adress testing, dividing buf into block sizes etc 
int write_block(void *buf, int block_number){
	printf("address of disk, %d\n", disk);
	printf("block_number: %d\n", block_number);
	printf("write address: %d\n", disk + block_number*SBFS_BLOCK_SIZE);
	printf("write to block address in relation to disk: %d\n", block_number*SBFS_BLOCK_SIZE);
	printf("write address: %d\n", disk + block_number*SBFS_BLOCK_SIZE);
	printf("size of buffer to write to disk: %d\n", sizeof(buf));
	memcpy(disk + block_number*SBFS_BLOCK_SIZE, buf, SBFS_BLOCK_SIZE);
	return 1;
}