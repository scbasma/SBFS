#include <stdlib.h>
#include <stdio.h> 
#include "inode.h"
#include "sbfs.h"
#include "dbg.h"
//#include "spblk.h"
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


struct sbfs_inode *get_inode(int i_nmbr){
	log_info("Inode size: %d", sizeof(struct sbfs_inode));
	int blk_nmbr = (FIRST_INODE_BLOCK_NMBR) + (i_nmbr / 85);
	int inode_in_block_nmbr = i_nmbr % 85;
	log_info("Block number of inode: %d", blk_nmbr);
	log_info("Inode number in block: %d", inode_in_block_nmbr);
	struct sbfs_inode *inode_block = malloc(SBFS_BLOCK_SIZE);
	read_block(inode_block, blk_nmbr);
	int i;
	struct sbfs_inode *inode = malloc(sizeof(struct sbfs_inode));
	memcpy(inode, inode_block + inode_in_block_nmbr, sizeof(struct sbfs_inode));
	log_info("inode grp_id: %d ", inode->grp_id);
	return inode;
}

struct sbfs_inode allocate_free_inode(){
		return get_inode(get_free_inode()); //method in spblk

}


int set_free_inode(int inode_number){

};


struct sbfs_inode namei(char *path_name){
 	//traverse dir entries
};
