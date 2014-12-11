#include "sbfs.h"
#include "dbg.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int disk;
// void  *disk;
// int load_disk(){
// 	disk = malloc(SBFS_DISK_SIZE);
// 	memset(disk, 0, SBFS_DISK_SIZE);
// 	// check_mem(disk);
// 	// if(disk = open(disk))
// 	return 0;

// error:
// 	log_info("ERROR LOADING DISK");
// 	return -1;

// }


//implement address testing (too large etc)
int read_block(void *buf, int block_number){
	
	// memcpy(buf, disk + block_number*SBFS_BLOCK_SIZE, SBFS_BLOCK_SIZE);
	
	int holder = disk;
	if((pread(disk, buf, SBFS_BLOCK_SIZE, block_number*SBFS_BLOCK_SIZE)) < 0){
		disk = holder;
		return -1;
	}
	disk = holder;
	return 0;
}


//implement adress testing, dividing buf into block sizes etc 
int write_block(const void *buf, int block_number){
	// memcpy(disk + block_number*SBFS_BLOCK_SIZE, buf, SBFS_BLOCK_SIZE);
	int holder = disk;
	if((pwrite(disk, buf, SBFS_BLOCK_SIZE , block_number*SBFS_BLOCK_SIZE)) < 0){
		disk = holder;
		return -1;
	}
	disk = holder;

	return 0;

}
