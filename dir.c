#include <stdlib.h>
#include "sbfs.h"
#include "dir.h"
#include "dbg.h"


//directory is just a file consisting of directory entries


//not finished function, call sys write when finished, just get up root inode
// int add_dir(struct sbfs_inode *inode, char *name){
// 	struct dir_entry dentry = malloc(sizeof(struct sbfs_dir_entry)):
	
// 	if(sizeof(name) >= FILENAME_MAX_LENGTH){
// 		strcpy(dentry->name, name);
// 	} else {
// 		return -1;
// 	}
	
// 	dentry->inode_number = inode->number;
// 	dentry->file_t = 2;
// 	dentry->offset = sizeof(struct sbfs_dir_entry);
	

// error:
// 	return -1;

// }


// void rem_dir(struct sbfs_inode *inode, char *path){

// }


struct dir_entry *get_dir(sbfs_core_inode *c_inode, uint32_t offset){
	
	if(offset % sizeof(struct dir_entry) != 0){
		log_err("offset at get_dir: not compatible with size of dir_entry");
		return NULL; //not able to find directory
	}
	if(offset >= c_inode->d_inode.size){
		log_err("offset larger than inode_size with offset: %d and inode_size: %d, this is inode %d", offset, c_inode->d_inode.size, c_inode->i_nmbr);
		return NULL;
	}

	struct dir_entry *entry = malloc(sizeof(struct dir_entry));
	if(sys_read(c_inode->i_nmbr, entry, sizeof(struct dir_entry), offset) > 0){
 				return entry;
 	}else{

 		return NULL;
 	}

 	return NULL;



}

