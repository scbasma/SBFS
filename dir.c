#include <stdlib.h>
#include "sbfs.h"
#include "dir.h"
#include "dbg.h"


//directory is just a file consisting of directory entries


//not finished function, call sys write when finished, just get up root inode
int add_dir(struct sbfs_inode *inode, char *name){
	struct dir_entry dentry = malloc(sizeof(struct sbfs_dir_entry)):
	
	if(sizeof(name) >= FILENAME_MAX_LENGTH){
		strcpy(dentry->name, name);
	} else {
		return -1;
	}
	
	dentry->inode_number = inode->number;
	dentry->file_t = 2;
	dentry->offset = sizeof(struct sbfs_dir_entry);
	

error:
	return -1;

}


void rem_dir(struct sbfs_inode *inode, char *path){

}

