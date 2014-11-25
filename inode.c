#include <stdlib.h>
#include <stdio.h> 
#include "inode.h"
#include "sbfs.h"
#include "dbg.h"
#include "bitmap.h"
#include "disk_op.h"
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
	return 0;

error:
	return -1;
}


sbfs_core_inode *iget(uint32_t i_nmbr){
	log_info("Inode core size: %d", sizeof(sbfs_core_inode));
	int blk_nmbr = (FIRST_INODE_BLOCK_NMBR) + (i_nmbr / INODE_NMBR_PER_BLOCK);
	int inode_in_block_nmbr = i_nmbr % INODE_NMBR_PER_BLOCK;
	
	log_info("Block number of inode: %d", blk_nmbr);
	log_info("Inode number in block: %d", inode_in_block_nmbr);
	
	sbfs_disk_inode *inode_block = malloc(SBFS_BLOCK_SIZE);
	read_block(inode_block, blk_nmbr);
	
	sbfs_disk_inode *d_inode = malloc(sizeof(sbfs_disk_inode));
	memcpy(d_inode, inode_block + inode_in_block_nmbr, sizeof(sbfs_disk_inode));

	sbfs_core_inode *c_inode = malloc(sizeof(sbfs_core_inode));
	memcpy(&(c_inode->d_inode), d_inode, sizeof(sbfs_disk_inode) );

	c_inode->i_nmbr = i_nmbr;
	c_inode->status = 0;
	c_inode->ref_count = 1;
	
	return c_inode;
}

void iput(sbfs_core_inode *c_inode){
	//lock here
	c_inode->ref_count--;
	if(c_inode->ref_count == 0){

		if(c_inode->d_inode.link_count == 0){
			//free inode, blocks 
		}
		if(c_inode->status){ //if changed write to disk, do it anyways right now

		}

		//write inode to disk
	}

	//release lock;
}

int bmap(sbfs_core_inode *c_inode, off_t offset,off_t *file_offset){
	int blk_nmbr;
	int block_pos_in_file = offset/SBFS_BLOCK_SIZE;
	int pos_in_block = offset%SBFS_BLOCK_SIZE;

	*file_offset = pos_in_block;

	if(block_pos_in_file > 12 ){ //12:number of direct blocks
		blk_nmbr = c_inode->d_inode.dt_blocks[block_pos_in_file];
	}
	//add functionality for level of indirection = 3
	return blk_nmbr;
}

int set_free_inode(int inode_number){

};


sbfs_core_inode namei(char *path_name){
 	//traverse dir entries
};
