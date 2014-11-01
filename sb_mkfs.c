#include "sbfs.h"
#include "disk_op.h"
#include <stdlib.h> 
#include <stdio.h>


char *disk;

void load_disk(){
	printf("adress of disk: %d\n", disk);
	disk = (char*) malloc(SBFS_DISK_SIZE);
	printf("adress of disk: %d\n", disk);
}

void write_super_block(){
	struct sbfs_super_block *sp_blk = malloc(sizeof(struct sbfs_super_block));
	sp_blk->version = 1;
	sp_blk->magic_number = 10012;
	sp_blk->block_count = SBFS_NUMBER_OF_BLOCKS;
	sp_blk->block_size = SBFS_BLOCK_SIZE;
	printf("size of sbfs_super_block: %d\n", sizeof(sp_blk));
	write_block(sp_blk, 1, 1);
	struct sbfs_super_block *buf = malloc(sizeof(SBFS_BLOCK_SIZE));
	read_block(buf, 1, 1);
	printf("Number of blocks: %d\n",buf->block_count);


}

void write_inodes(){

}


void fill_disk(){
	write_super_block();
	write_inodes();
}


int main(int argc, int *argv[]){
	load_disk();
	printf("sizeof disk: %d\n", SBFS_DISK_SIZE);
	fill_disk();
	return 0;
}