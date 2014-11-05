#include <stdlib.h>
#include <stdio.h> 
#include "inode.h"
#include "sbfs.h"
//#include "spblk.h"

//TODO: make so that several inodes in one block; number of inodes = SIZE_OF_BLOCK/SIZE_OF_INODE


//only called at mkfs
int init_inode_list(){
	int i;
	for(i = 0; i < SBFS_NUMBER_OF_INODES; i++){
		struct sbfs_inode *new_inode = malloc(SBFS_BLOCK_SIZE);
		if(new_inode == NULL){
			printf("INODE IS NULL");
		}
		printf("initializes inode pointer");
		new_inode->mode = 'r';
		new_inode->size = 0;
		new_inode->block_address = 0;
		printf("initialized inode values");
		int ret = write_block(new_inode, ROOT_INODE_NUMBER + i);
		free(new_inode);
	}
	return 0;
}

int write_root_inode(){
	struct sbfs_inode root_inode = malloc(SBFS_BLOCK_SIZE);
	root_inode->='r';
	root_inode->size = 0;
	new_inode->block_address = FIRST_DATA_BLOCK;
	
	return 0;
}


// sbfs_inode get_inode(int inode_number){
	
// }


// sbfs_inode get_next_free_inode(){

// };

// int free_inode(int inode_number){

// };


// sbfs_inode namei(char *path_name){
// 	//traverse dir entries
// };
