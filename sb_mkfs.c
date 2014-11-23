#include "sbfs.h"
#include "disk_op.h"

#include "inode.h"
#include "dbg.h"
//#include "bitmap.h"
#include <stdlib.h> 
#include <stdio.h>
#include "stdint.h"


/**

Superblock | block bitmap*2 | inode bitmap*2 | inodes 


*/

sbfs_sp_blk *sp_blk;


void write_super_block(){
	sp_blk = malloc(sizeof(sbfs_sp_blk));
	check_mem(sp_blk);
	sp_blk->magic_number = 10012;
	sp_blk->block_count = SBFS_NUMBER_OF_BLOCKS;
	sp_blk->block_size = SBFS_BLOCK_SIZE;
	sp_blk->first_free_inode = ROOT_INODE_NUMBER+1;
	sp_blk->number_of_inodes = SBFS_NUMBER_OF_INODES;
	sp_blk->next_free_data_block = FIRST_DATA_BLOCK;
	sp_blk->next_free_inode = ROOT_INODE_NUMBER+1;
	sp_blk->m_time = 0;
	sp_blk->w_time = 0;
	void *blk_sp_blk = malloc(SBFS_BLOCK_SIZE);
	memcpy(blk_sp_blk, sp_blk, sizeof(sbfs_sp_blk));
	write_block(blk_sp_blk, 0);
	free(blk_sp_blk);
error:
	;

}

int init_inode_list(){
	int j = 0;
	int inode_size = sizeof(struct sbfs_inode);
	int blk_nmbr = 0;
	log_info("First block number: %d", FIRST_INODE_BLOCK_NMBR);
	while(j < SBFS_NUMBER_OF_INODES){
		
		int i;
		struct sbfs_inode *inodeBlock = malloc(SBFS_BLOCK_SIZE);
		struct sbfs_inode *inode_block_trav = inodeBlock;
		check_mem(inodeBlock);
		for(i = 0; i < INODE_NMBR_PER_BLOCK ; i++, j++){
			struct sbfs_inode *new_inode = malloc(inode_size);
			new_inode->mode = 770;
			new_inode->size = 0;
			new_inode->a_time = 0;
			new_inode->m_time = 0;
			new_inode->c_time = 0;
			new_inode->grp_id = 65534;
			new_inode->user_id = 65534;
			new_inode->link_count = 0;
			new_inode->res_blocks_nmbr = 0;
			new_inode->flags = 0;
			memcpy(inode_block_trav, new_inode, inode_size);
			inode_block_trav++;
			free(new_inode);
		}

		int ret = write_block(inodeBlock, FIRST_INODE_BLOCK_NMBR+blk_nmbr);

		free(inodeBlock);
		blk_nmbr++;
	}
	log_info("Last block number: %d", FIRST_INODE_BLOCK_NMBR+blk_nmbr);
	log_info("Total size of all inodes: %d", SBFS_NUMBER_OF_INODES*inode_size);
	log_info("Inodes per block: %d", SBFS_BLOCK_SIZE/sizeof(struct sbfs_inode));
	log_info("Number of blocks used: %d", blk_nmbr);
	return 0;


error:
	return -1;
}



void write_bitmaps(){
	//start writing bitmaps after superblock, at block location 1
	int start_block = 1;
	int i;
	for(i = 0; i < NUMBER_OF_BITMAP_BLOCKS; i++){
		int *bitmap = calloc(1024, sizeof(int));
		write_block(bitmap, start_block+i);
		log_info("Bitmap written to block: %d",start_block+i);
		free(bitmap);
	}
}


void make_root(){
	struct sbfs_inode root_inode = get_inode(ROOT_INODE_NUMBER);

}

void fill_disk(){
	log_info("Writing superblock.");
	write_super_block();
	log_info("Writing bitmaps");
	write_bitmaps();
	log_info("Writing inode_list");
	init_inode_list();
	make_root();
}


int main(int argc, int *argv[]){
	load_disk();
	printf("sizeof disk: %d\n", SBFS_DISK_SIZE);
	fill_disk();
	return 0;
}