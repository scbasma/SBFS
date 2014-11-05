#include <stdlib.h>
#include <stdio.h>
#include "spblk.h" 
#include "sbfs.h"



struct sbfs_sp_blk *init_sp_blk(){
	
	struct sbfs_sp_blk *sp_blk = malloc(SBFS_BLOCK_SIZE);
	if(sp_blk == NULL){
		printf("sp_blk is NULL");
	}
	sp_blk->version = 1;
	sp_blk->magic_number = 10012;
	sp_blk->block_count = SBFS_NUMBER_OF_BLOCKS;
	sp_blk->block_size = SBFS_BLOCK_SIZE;
	sp_blk->next_free_data_block = FIRST_DATA_BLOCK;
	sp_blk->next_free_inode = ROOT_INODE_NUMBER+1;

	return sp_blk;
};

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