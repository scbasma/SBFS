#include "sbfs.h"
#include "disk_op.h"
#include "dbg.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

void  *disk;
int load_disk(){
	disk = malloc(SBFS_DISK_SIZE);
	memset(disk, 0, SBFS_DISK_SIZE);
	// check_mem(disk);
	// if(disk = open(disk))
	return 0;

error:
	log_info("ERROR LOADING DISK");
	return -1;

}


//implement address testing (too large etc)
int read_block(void *buf, int block_number){
	
	memcpy(buf, disk + block_number*SBFS_BLOCK_SIZE, SBFS_BLOCK_SIZE);
	
	// int holder = disk;
	// if(pread(disk, buf, block_number*SBFS_BLOCK_SIZE, SBFS_BLOCK_SIZE) < 0){
	// 	disk = holder;
	// 	return -1;
	// }
	// disk = holder;
	return 0;
}


//implement adress testing, dividing buf into block sizes etc 
int write_block(void *buf, int block_number){
	check_mem(buf);
	memcpy(disk + block_number*SBFS_BLOCK_SIZE, buf, SBFS_BLOCK_SIZE);
	// int holder = disk;
	// if(pwrite(disk, buf, block_number*SBFS_BLOCK_SIZE, SBFS_BLOCK_SIZE) < 0){
	// 	disk = holder;
	// 	return -1;
	// }
	// disk = holder;
	return 0;
error:
	return -1;
}
