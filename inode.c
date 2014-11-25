#include <stdlib.h>
#include <stdio.h> 
#include "inode.h"
#include "sbfs.h"
#include "dbg.h"
#include "bitmap.h"
#include "disk_op.h"
#include "spblk.h"
#include "dir.h"



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


sbfs_core_inode c_inode_list[100];
sbfs_core_inode *c_inode_free;

int write_root_inode(){
	struct sbfs_inode *root_inode = malloc(SBFS_BLOCK_SIZE);
	return 0;

error:
	return -1;
}

void init_incore_list(){
	c_inode_free = malloc(100*sizeof(sbfs_core_inode));

	int i;
	for(i = 0; i < 100; i++){

	}
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
	free(inode_block);
	sbfs_core_inode *c_inode = malloc(sizeof(sbfs_core_inode));
	memcpy(&(c_inode->d_inode), d_inode, sizeof(sbfs_disk_inode) );
	free(d_inode);
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
			log_info("BLOCK GIVEN TO ROOT BEFORE WRITE: %d", c_inode->d_inode.dt_blocks[0]);
			write_inode(c_inode);
		}

		
	}

	//release lock;
}

int bmap(sbfs_core_inode *c_inode, off_t offset, uint8_t *file_offset){
	log_info("Inside bmap");
	int blk_nmbr = 0;
	int block_pos_in_file = offset/SBFS_BLOCK_SIZE;
	int pos_in_block = offset%SBFS_BLOCK_SIZE;
	if(file_offset){
		*file_offset = pos_in_block;
	}
	log_info("file offset: %d", *file_offset );

	if(block_pos_in_file < 12 ){ //12:number of direct blocks
		blk_nmbr = c_inode->d_inode.dt_blocks[block_pos_in_file];
	}
	//add functionality for level of indirection = 3
	log_info("BLOCK NUMBER GIVEN BY BMAP: %d", blk_nmbr);
	return blk_nmbr;
}

int set_free_inode(int inode_number){

};

int write_inode(sbfs_core_inode *inode){
	sbfs_disk_inode *buf = malloc(SBFS_BLOCK_SIZE);
	int blk_nmbr = (FIRST_INODE_BLOCK_NMBR) + ((inode->i_nmbr) / INODE_NMBR_PER_BLOCK);
	int inode_in_block_nmbr = (inode->i_nmbr) % INODE_NMBR_PER_BLOCK;
	
	read_block(buf, blk_nmbr);
	memcpy(buf+inode_in_block_nmbr, &(inode->d_inode), sizeof(sbfs_disk_inode));
	log_info("BLOCK GIVEN TO ROOT BEFORE WRITE: %d", inode->d_inode.dt_blocks[0]);
	log_info("Block number of inode: %d", blk_nmbr);
	log_info("Inode number in block: %d", inode_in_block_nmbr);
	write_block(buf, blk_nmbr);
	free(buf);

	return 0;
}


sbfs_core_inode *namei(char *path_name){
 	sbfs_core_inode *working_inode = iget(2); //start with root
 	log_info("number of working inode: %d", working_inode->i_nmbr);
 	log_info("status of working inode: %d", working_inode->status);
 	char *path = malloc(strlen(path_name) + 1);
 	strcpy(path, path_name);
 	char *dir_name;
 	dir_name = strtok(path, "/");
 	int inodeNumber = working_inode->i_nmbr;
 	int increment = 0;
 	log_info("Dir name we are looking for: %s", dir_name);
 	while(dir_name != NULL){
 		log_info("DIR NAME: %s", dir_name);
 		log_info("size of dir_entry: %d",sizeof(struct dir_entry));
 		struct dir_entry *entry = malloc(sizeof(struct dir_entry)); 
 		struct dir_entry *entry_ptr = entry;
 		log_info("Value of increment: %d", increment);
 		
 		if(sys_read((uint64_t) working_inode, entry_ptr, sizeof(struct dir_entry), increment*sizeof(struct dir_entry))){
 			if(entry_ptr->name == NULL){ //end of entries for this directory
 				log_info("NO ENTRY MATCHES");
 				return NULL; 
 			}
 			log_info("test of dir entry: %s", entry_ptr->name);
 			log_info("BEFOR WHILE ENTRY PTR NAME");	
 			log_info("Entry ptr name: %s", entry_ptr->name);
 			if(!strcmp(entry_ptr->name,dir_name)){
 				log_info("Entry ptr name FOUND: %s", entry_ptr->name);
 				working_inode = iget(entry_ptr->inode_number);
 				dir_name = strtok(NULL, "/");
 				increment = 0;
 				continue;
 			}
 		
 		}
 		increment++;
 		continue;
 		free(entry);
 	}
 	return working_inode;
};
