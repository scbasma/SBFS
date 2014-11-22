#include <stdlib.h>
#include "sbfs.h"
#include "dir.h"




void add_dir(struct sbfs_inode *inode, char *path){
	struct sbfs_dir_entry dentry = malloc(sizeof(struct sbfs_dir_entry)):
	strcpy(dentry->name, path);
	dentry->inode_number = inode->number;
	//send dentry to write function
}


void rem_dir(struct sbfs_inode *inode, char *path){

}

