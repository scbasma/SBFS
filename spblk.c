#include <stdlib.h>
#include <stdio.h>
#include "spblk.h" 
#include "sbfs.h"
#include "dbg.h"
#include "bitmap.h"
#include "disk_op.h"

	// uint16_t version;
	// uint32_t magic_number;
	// uint32_t block_count;
	// uint16_t block_size;
	// uint32_t first_free_inode;
	// uint32_t number_of_inodes;
	// uint16_t next_free_inode;
	// uint32_t next_free_data_block;
	// uint32_t first_free_data_block; //the block spblk is written to
	// uint32_t m_time; //last mounted
	// uint32_t w_time; //last write
	// uint16_t fs_state

//sbfs_sp_blk *sp_blk;

// int get_free_dblock(){
// 	int free_block = sp_blk->next_free_data_block;
// 	sp_blk->next_free_data_block += 1;
// 	int block_nmbr = free_block /NUMBER_OF_DATA_BITMAP_BLOCKS;
// 	int *bitmap_block = malloc(SBFS_BLOCK_SIZE);
// 	int bitPos = free_block - (4096*8*block_nmbr);
// 	read_block(bitmap_block, FIRST_BITMAP_BLOCK_POS+NUMBER_OF_INODE_BITMAP_BLOCKS+block_nmbr);
// 	setBit(bitmap_block, bitPos);
// 	return free_block;
// }

int get_super(){
	sp_blk = calloc(1, sizeof(sbfs_sp_blk));
	void *block = calloc(1, SBFS_BLOCK_SIZE);
	int ret = read_block(block, 0);
	memcpy(sp_blk, block, sizeof(sbfs_sp_blk));
	return ret;
}
