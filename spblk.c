#include <stdlib.h>
#include <stdio.h>
#include "spblk.h" 
#include "sbfs.h"
#include "dbg.h"

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




struct sbfs_sp_blk *init_sp_blk(){
	
	struct sbfs_sp_blk *sp_blk = malloc(SBFS_BLOCK_SIZE);
	check_mem(sp_blk);
	sp_blk->magic_number = 10012;
	sp_blk->block_count = SBFS_NUMBER_OF_BLOCKS;
	sp_blk->block_size = SBFS_BLOCK_SIZE;
	sp_blk->first_free_inode = ROOT_INODE_NUMBER;
	sp_blk->number_of_inodes = 2*1024*32;
	sp_blk->next_free_data_block = 2;
	sp_blk->next_free_inode = ROOT_INODE_NUMBER+1;
	sp_blk->m_time = 0;
	sp_blk->w_time = 0;
	return sp_blk;

error:
	return NULL;
}

// int get_next_free_data_block(struct sbfs_sp_blk *sp_blk){
// 	int next = (*sp_blk)->next_free_data_block;
// 	//extremely naive, unefficient way to handle next block
// 	//implement bitmap on todo list
// 	(*sp_blk)->next_free_data_block = next + 1;
// 	if(next)
// 		return next;
// 	return -1;
// };

// int get_next_free_inode(struct sbfs_sp_blk *sp_blk){
//  int next = (*sp_blk)->next_free_inode;
//  //extremely naive, unefficient way to handle next inode
// 	//implement bitmap on todo list
//  (*sp_blk)->next_free_inode = next + 1;
//  if(next)
//  	return next;
//  return -1;
// };

size_t get_spblk_size(){
	return sizeof(struct sbfs_sp_blk);
}