#include "sbfs.h"
#include "disk_op.h"
#include "spblk.h"
#include "inode.h"
#include "dbg.h"
#include "bitmap.h"
#include "datablock.h"
#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "time.h"
#include "sys.h"
#include "dir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>



/**

Superblock | block bitmap*2 | inode bitmap*2 | inodes 


*/
// int disk;
sbfs_sp_blk *sp_blk;

int load_disk(const char *path){
	if((disk = open(path, O_RDWR)) == -1){
		log_err("disk failed to open with path: %s", path);
		return -1;
	}
	log_info("DISK LOADED");
	return 0;
}

void write_super_block(){
	sp_blk = calloc(1,sizeof(sbfs_sp_blk));
	check_mem(sp_blk);
	sp_blk->size = lseek(disk, 0, SEEK_END);
	log_info("SEEK_end: %d", SEEK_END);
	lseek(disk, 0, SEEK_SET);
	sp_blk->magic_number = 10012;
	sp_blk->block_count = SBFS_NUMBER_OF_BLOCKS;
	sp_blk->block_size = SBFS_BLOCK_SIZE;
	sp_blk->first_free_inode = ROOT_INODE_NUMBER+1;
	sp_blk->number_of_inodes = SBFS_NUMBER_OF_INODES;
	sp_blk->next_free_data_block = FIRST_DATA_BLOCK;
	sp_blk->next_free_inode = ROOT_INODE_NUMBER+1;
	sp_blk->m_time = time(NULL);
	sp_blk->w_time = time(NULL);
	void *blk_sp_blk = calloc(1,SBFS_BLOCK_SIZE);
	memcpy(blk_sp_blk, sp_blk, sizeof(sbfs_sp_blk));
	write_block(blk_sp_blk, 0);
	free(blk_sp_blk);
	check_mem(sp_blk);
error:
	;

}

// int init_inode_list(){
// 	int i;
// 	for(i = 0; i < NUMBER_OF_INODE_BLOCKS; i++){
// 		sbfs_disk_inode *inodes = calloc(1, SBFS_BLOCK_SIZE);
// 		log_info("Writing inode_block: %d", i);
// 		write_block(inodes, FIRST_INODE_BLOCK_NMBR+i);
// 	}
// 	return 0;
// }


int init_inode_list(){
	int j = 0;
	int inode_size = sizeof(sbfs_disk_inode);
	int blk_nmbr = 0;
	// log_info("First block number: %d", FIRST_INODE_BLOCK_NMBR);
	while(j < SBFS_NUMBER_OF_INODES){
		
		int i;
		sbfs_disk_inode *inodeBlock = calloc(1, SBFS_BLOCK_SIZE);
		sbfs_disk_inode *inode_block_trav = inodeBlock;
		check_mem(inodeBlock);
		for(i = 0; i < INODE_NMBR_PER_BLOCK ; i++, j++){
			sbfs_disk_inode *new_inode = malloc(inode_size);
			new_inode->i_nmbr = j;
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
			new_inode->perm = 0777;
			uint32_t init_data_block_list[15] = {0}; 
			memcpy(new_inode->dt_blocks, init_data_block_list, sizeof(new_inode->dt_blocks));
			memcpy(inode_block_trav, new_inode, inode_size);
			inode_block_trav++;
			free(new_inode);
		}

		int ret = write_block((int *) inodeBlock, FIRST_INODE_BLOCK_NMBR+blk_nmbr);
		if(ret == -1){
			log_err("failed to initialize inodes, %d", ret);
			return -1;
		}
		free(inodeBlock);
		blk_nmbr++;
	}
	log_info("First block number: %d", FIRST_INODE_BLOCK_NMBR);
	log_info("Last block number: %d", FIRST_INODE_BLOCK_NMBR+blk_nmbr);
	// log_info("Total size of all inodes: %d", SBFS_NUMBER_OF_INODES*inode_size);
	// log_info("Inodes per block: %d", SBFS_BLOCK_SIZE/sizeof(sbfs_disk_inode));
	// log_info("Number of blocks used: %d", blk_nmbr);



	return 0;


error:
	return -1;
}



int write_bitmaps(){
	//start writing bitmaps after superblock, at block location 1
	int start_block = 1;
	int i;
	for(i = 0; i < NUMBER_OF_BITMAP_BLOCKS; i++){
		int *bitmap = calloc(1024, sizeof(int));
		int ret = write_block(bitmap, start_block+i);
		if(ret == -1){
			return -1;
		}

		log_info("Bitmap written to block: %d",start_block+i);
		free(bitmap);
	}
	//initalize bitmaps correctly, with the first few ones set to 1
	int data_block_nmbr = 1;
	int *d_bitmap = (int*) calloc(1024, sizeof(int)); 
	read_block(d_bitmap, data_block_nmbr);
	for(i = 0; i < FIRST_DATA_BLOCK; i++){
			setBit(d_bitmap, i);
	}
	write_block(d_bitmap, data_block_nmbr);
	free(d_bitmap);

	return 0;
}

// struct sbfs_inode {
// 	uint32_t number;
// 	uint16_t mode;
// 	uint16_t user_id;
// 	uint32_t size;
// 	uint32_t a_time;
// 	uint32_t m_time;
// 	uint32_t c_time;
// 	uint16_t grp_id;
// 	uint16_t link_count;
// 	uint32_t res_blocks_nmbr;
// 	uint32_t flags;
// 	uint32_t *dt_blocks; //15 blocks, 12 direct, 13th indirect, 14th double indirect, 15th triple indirect

// };




void make_root(){
	sbfs_core_inode *root_inode = iget(ROOT_INODE_NUMBER);
	root_inode->d_inode.mode = 0777;
	root_inode->d_inode.user_id=getuid();
	root_inode->d_inode.grp_id=getgid();
	root_inode->d_inode.a_time = time(NULL);
	root_inode->d_inode.m_time = time(NULL);
	root_inode->d_inode.link_count =2;
	root_inode->d_inode.res_blocks_nmbr = 1;
	root_inode->d_inode.flags = 0;
	root_inode->d_inode.dt_blocks[0] = balloc();
	root_inode->d_inode.type = 2;
	root_inode->d_inode.perm = 0777;
	iput(root_inode);

	// log_info("BLOCK GIVEN TO ROOT: %d", root_inode->d_inode.dt_blocks[0]);
	//struct dir_entry root_entries[2];

	// log_info("Size of dir entry: %d", sizeof(struct dir_entry));
	struct dir_entry *dot_entry = malloc(sizeof(struct dir_entry));
	strcpy(dot_entry->name , ".");
	dot_entry->inode_number = 2;
	dot_entry->offset = sizeof(struct dir_entry);
	dot_entry->file_t = 2;

	struct dir_entry *dot_dot_entry = malloc(sizeof(struct dir_entry));
	strcpy(dot_dot_entry->name, "..");
	dot_dot_entry->inode_number = 2;
	dot_dot_entry->offset = sizeof(struct dir_entry);
	dot_dot_entry->file_t = 2;
	// root_entries[0] = dot_entry;
	// root_entries[1] = dot_dot_entry;


	//sys_write(root_inode->i_nmbr, (char * ) root_entries, sizeof(root_entries), 0);

	add_dir(root_inode->i_nmbr, dot_dot_entry);
	//root_inode = iget(2);
	add_dir(root_inode->i_nmbr, dot_entry);
	// free(dot_dot_entry);
	// free(dot_entry);
	free(root_inode);
	
	// struct dir_entry *test = malloc(sizeof(struct dir_entry));
	// sys_read(root_inode->i_nmbr, test, sizeof(struct dir_entry), sizeof(struct dir_entry));
	// // log_info("test of dir entry: %s", test->name);
	// free(test);
	
	// free(root_inode);
	// root_inode = iget(ROOT_INODE_NUMBER);
	// log_info("BLOCK GIVEN TO ROOT AFTER IGET: %d", root_inode->d_inode.dt_blocks[0]);
	//sbfs_core_inode *nameitest = namei("/..");



}

void fill_disk(){
	log_info("Writing superblock.");
	write_super_block();
	log_info("Writing bitmaps");
	write_bitmaps();
	log_info("initializing data blocks");
	init_dt_blks();
	log_info("Writing inode_list");
	init_inode_list();
	make_root();
}

void mkfs(){
	fill_disk();
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("1 arg required: path");
		return -1;
	}
	const char* path = realpath(argv[1], NULL);
	
	load_disk(path);

	mkfs();
	sbfs_core_inode *inode = namei("/");
	struct dir_entry *entry = get_dir(inode, 0);
	log_info("entry name: %s", entry->name);
	entry = get_dir(inode, sizeof(struct dir_entry));
	log_info("inode size: %d", (int)inode->d_inode.size);
	log_info("entry 2 name: %s", entry->name);
	log_info("inode grp id: %d", inode->d_inode.grp_id);
	close(disk);
	load_disk(path);
	inode = namei("/");
	entry = get_dir(inode, 0);
	log_info("entry name: %s", entry->name);
	entry = get_dir(inode, sizeof(struct dir_entry));
	log_info("entry 2 name: %s", entry->name);
	log_info("inode grp id: %d", inode->d_inode.grp_id);
	log_info("inode size: %d", (int)inode->d_inode.size);
	get_super();
	log_info("superblock magic number: %d", sp_blk->magic_number);
	log_info("Size of struct dir_Entry: %d", (int)sizeof(struct dir_entry));
	close(disk);
	return 0;
};