#include "sbfs.h"
#include "disk_op.h"
#include "spblk.h"
#include "inode.h"
//#include "bitmap.h"
#include <stdlib.h> 
#include <stdio.h>
#include "inode.h"


/**

Superblock | block bitmap*2 | inode bitmap*2 | inodes 


*/

void write_super_block(){
	struct sbfs_sp_blk *sp_blk = init_sp_blk();
	if(sp_blk == NULL){
		printf("SPBLK IS NULL");
	}
	printf("size of sbfs_super_block: %d\n", sizeof(sp_blk));
	write_block(sp_blk, 1);
	free(sp_blk);
}

int init_inode_list(){
	int j = 0;
	int inode_size = sizeof(struct sbfs_inode);

	while(j < SBFS_NUMBER_OF_INODES){
		
		int i;
		void *inodeBlock = malloc(SBFS_BLOCK_SIZE);
		check_mem(inodeBlock);
		for(i = 0; i < SBFS_BLOCK_SIZE ; i+=inode_size, j++){
			new_inode->mode = 0770;
			new_inode->size = 0;
			new_inode->a_time = 0;
			new_inode->m_time = 0;
			new_inode->c_time = 0;
			new_inode->grp_id = 65534;
			new_inode->user_id = 65534;
			new_inode->link_count = 0;
			new_inode->res_blocks_nmbr = 0;
			new_inode->flags = 0;
			log_info("initialized inode with number: %d", new_inode->number);
			int ret = write_block();
		}

		free(inodeBlock);
	}
	return 0;


error:
	return -1;
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