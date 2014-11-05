#include "sbfs.h"
#include "disk_op.h"
#include "spblk.h"
#include "inode.h"
//#include "bitmap.h"
#include <stdlib.h> 
#include <stdio.h>


void write_super_block(){
	struct sbfs_sp_blk *sp_blk = init_sp_blk();
	if(sp_blk == NULL){
		printf("SPBLK IS NULL");
	}
	printf("size of sbfs_super_block: %d\n", sizeof(sp_blk));
	init_sp_blk(sp_blk);
	write_block(sp_blk, 1);
	free(sp_blk);
}

void write_bitmaps(){
}

void write_inodes(){
	write_root_inode();
	init_inode_list();
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