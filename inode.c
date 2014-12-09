#include <stdlib.h>
#include <stdio.h> 
#include <libgen.h>
#include "inode.h"
#include "sbfs.h"
#include "dbg.h"
#include "bitmap.h"
#include "disk_op.h"
#include "spblk.h"
#include "dir.h"
#include "datablock.h"



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

void ifree(uint32_t inode_number){
	sp_blk->number_of_inodes++;

}

sbfs_core_inode *ialloc(){
	sbfs_core_inode *c_inode = malloc(sizeof(sbfs_core_inode));

	int free_inode = sp_blk->next_free_inode;
	sp_blk->next_free_inode += 1;
	sp_blk->number_of_inodes -=1;
	//int block_nmbr = free_inode / NUMBER_OF_INODE_BITMAP_BLOCKS;
	int block_nmbr = free_inode / (4096*8);
	
	int *bitmap_block = (int*) calloc(1024, sizeof(int));
	read_block(bitmap_block, FIRST_BITMAP_BLOCK_POS+block_nmbr);
	int bitPos = free_inode - (4096*8*block_nmbr);
	check_mem(bitmap_block);
	setBit(bitmap_block, bitPos);
	write_block(bitmap_block, FIRST_BITMAP_BLOCK_POS+block_nmbr);
	c_inode = iget(free_inode);
	free(bitmap_block);
	return c_inode;

error:
	return NULL;
}

sbfs_core_inode *iget(uint32_t i_nmbr){
	
	int blk_nmbr = (FIRST_INODE_BLOCK_NMBR) + (i_nmbr / INODE_NMBR_PER_BLOCK);
	int inode_in_block_nmbr = i_nmbr % INODE_NMBR_PER_BLOCK;
	
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
		
		}

		write_inode(c_inode);		

		
	}

	//release lock;
}

int bmap(sbfs_core_inode *c_inode, off_t offset, uint8_t *file_offset){
	int blk_nmbr = 0;
	int block_pos_in_file = offset/SBFS_BLOCK_SIZE;
	int pos_in_block = offset%SBFS_BLOCK_SIZE;
	if(file_offset){
		*file_offset = pos_in_block;
	}

	if(block_pos_in_file < 12 ){ //12:number of direct blocks
		blk_nmbr = c_inode->d_inode.dt_blocks[block_pos_in_file];
		if (blk_nmbr == 0){
			blk_nmbr = balloc();
			if(blk_nmbr == 0) return -1;
			c_inode->d_inode.dt_blocks[block_pos_in_file] = blk_nmbr;
		}
	}
	else if(block_pos_in_file < (12+1024)){ //first indirect block, with 1024 block numbers together with the first 12 direct
		uint32_t indirect_block_nmbr = c_inode->d_inode.dt_blocks[12];

		if(indirect_block_nmbr == 0){
			indirect_block_nmbr = balloc();
			c_inode->d_inode.dt_blocks[12] = indirect_block_nmbr;
			uint32_t blk_nmbr = balloc();

			uint32_t *buf = malloc(SBFS_BLOCK_SIZE);
			//initialize every value in new indirect block to zero
			int i;
			for(i = 0; i < SBFS_BLOCK_SIZE/sizeof(uint32_t); i++){
				*(buf++) = 0;
			};
			*(buf+(block_pos_in_file-12)) = blk_nmbr;
			write_block(buf, indirect_block_nmbr);
			free(buf);

			return blk_nmbr;

		}

		uint32_t *buf = malloc(SBFS_BLOCK_SIZE);
		read_block(buf, indirect_block_nmbr);
		uint32_t blk_nmbr = *(buf+(block_pos_in_file-12));

		if(blk_nmbr == NULL || blk_nmbr == 0){
			blk_nmbr = balloc();
			*(buf+(block_pos_in_file-12)) = blk_nmbr;
			write_block(buf, indirect_block_nmbr);
		}

		free(buf);
		return blk_nmbr;

	}
	else if(block_pos_in_file < (12+1024*1024)){ //double indirect block
		uint32_t first_block = block_pos_in_file-(12+1024);
		uint32_t sec_block = first_block/(1024*1024);
		uint32_t third_block = first_block - sec_block*1024;

		uint32_t double_indirect_block_nmbr = c_inode->d_inode.dt_blocks[13];
		uint32_t *buf = malloc(SBFS_BLOCK_SIZE);
		
		if(double_indirect_block_nmbr == 0){

			double_indirect_block_nmbr = balloc();
			c_inode->d_inode.dt_blocks[13] = double_indirect_block_nmbr;
			//initialize values in new double indirect block to 0
			int i;
			for(i = 0; i < SBFS_BLOCK_SIZE/sizeof(uint32_t); i++){
				*(buf++) = 0;
			};
			uint32_t indirect_block_nmbr = balloc();
			*(buf+sec_block) = indirect_block_nmbr;
			write_block(buf, double_indirect_block_nmbr);
			read_block(buf, indirect_block_nmbr);
			//initialize values in new indirect block to 0
			for(i = 0; i < SBFS_BLOCK_SIZE/sizeof(uint32_t); i++){
				*(buf++) = 0;
			};
			uint32_t blk_nmbr = balloc();
			*(buf+third_block) = blk_nmbr;
			free(buf);
			return blk_nmbr;

		}
		read_block(buf, double_indirect_block_nmbr);
		uint32_t indirect_block_nmbr = *(buf+sec_block);
		if(indirect_block_nmbr == NULL || indirect_block_nmbr == 0){
			indirect_block_nmbr = balloc();
			*(buf+sec_block) = indirect_block_nmbr;
		}
		read_block(buf, indirect_block_nmbr);
		uint32_t blk_nmbr = *(buf+third_block);

		if(blk_nmbr == NULL || blk_nmbr == 0){
			blk_nmbr = balloc();
			*(buf+third_block) = blk_nmbr;
		}
		free(buf);
		return blk_nmbr;

	}
	//add functionality for level of indirection = 3
	
	return blk_nmbr;
}



int write_inode(sbfs_core_inode *inode){
	sbfs_disk_inode *buf = malloc(SBFS_BLOCK_SIZE);
	int blk_nmbr = (FIRST_INODE_BLOCK_NMBR) + ((inode->i_nmbr) / INODE_NMBR_PER_BLOCK);
	int inode_in_block_nmbr = (inode->i_nmbr) % INODE_NMBR_PER_BLOCK;
	

	read_block(buf, blk_nmbr);
	memcpy(buf+inode_in_block_nmbr, &(inode->d_inode), sizeof(sbfs_disk_inode));
	// log_info("BLOCK GIVEN TO ROOT BEFORE WRITE: %d", inode->d_inode.dt_blocks[0]);
	// log_info("Block number of inode: %d", blk_nmbr);
	// log_info("Inode number in block: %d", inode_in_block_nmbr);
	write_block(buf, blk_nmbr);
	free(buf);

	return 0;
}


sbfs_core_inode *namei(const char *one_path){
	char * path_name = malloc(sizeof(*one_path)); 
	strcpy(path_name, one_path);
 	sbfs_core_inode *working_inode = iget(2);
 	
; 	if(strcmp(path_name, "/") == 0){
 		return working_inode;
 	}	
 	// log_info("number of working inode: %d", working_inode->i_nmbr);
 	// log_info("status of working inode: %d", working_inode->status);
 	char *path = malloc(strlen(path_name) + 1);
 	strcpy(path, path_name);
 	char *filenamec = strdup(path);
 	char *filename; 
 	filename = basename(filenamec);
 	char *dir_name;
 	dir_name = strtok(path, "/");
 	int inodeNumber = working_inode->i_nmbr;
 	int increment = 0;
 	while(dir_name != NULL){
 		
 		struct dir_entry *entry = get_dir(working_inode, increment*(sizeof(struct dir_entry))); 
		
		if(entry == NULL){
			return NULL;
		}
		if(entry != NULL && strcmp(entry->name, filename) == 0){
			working_inode = iget(entry->inode_number);
			//return working_inode;
		}
		// log_info("test of dir entry: %s", entry_ptr->name);
		// log_info("BEFOR WHILE ENTRY PTR NAME");	
		// log_info("Entry ptr name: %s", entry_ptr->name);
		if(strcmp(entry->name,dir_name) == 0){
			working_inode = iget(entry->inode_number);
			dir_name = strtok(NULL, "/");
			increment = 0;
			continue;
		}

 		increment++;
 		continue;
 		free(entry);
 	}
 	return working_inode;
};
