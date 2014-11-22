#include <stdlib.h>
#include <stdio.h> 
#include "inode.h"
#include "sbfs.h"
#include "dbg.h"
#include "spblk.h"

//TODO: make so that several inodes in one block; number of inodes = SIZE_OF_BLOCK/SIZE_OF_INODE

// struct sbfs_inode {
// 	uint16_t mode;
// 	uint16_t user_id;
// 	uint32_t a_time;
// 	uint32_t m_time;
// 	uint32_t c_time;
// 	uint16_t grp_id;
// 	uint16_t link_count;
// 	uint32_t res_blocks_nmbr;
// 	uint32_t flags;
// 	uint32_t *dt_blocks; //15 blocks, 12 direct, 13th indirect, 14th double indirect, 15th triple indirect


// 	int *block_pointers;
// };



int write_root_inode(){
	struct sbfs_inode *root_inode = malloc(SBFS_BLOCK_SIZE);
	check_mem(root_inode);
	root_inode->mode=600;
	root_inode->size = 0;
	root_inode->type=2;
	root_inode->block_address = sp_blk;
	return 0;

error:
	return -1;
}


sbfs_inode allocate_inode(int inode_number){
		
}


// sbfs_inode get_next_free_inode(){

// };

// int free_inode(int inode_number){

// };


// sbfs_inode namei(char *path_name){
// 	//traverse dir entries
// };
