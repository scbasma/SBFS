#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "string.h"
#include "inode.h"
#include "disk_op.h"
#include "spblk.h"
#include "sbfs.h"
#include "dbg.h"
#include <time.h>
#include "datablock.h"
#include "dir.h"
#include <libgen.h>

size_t sys_write(uint32_t fd, const char *buf, size_t count, off_t offset){
	buf = (char *) buf;
	//sbfs_core_inode *c_inode = (sbfs_core_inode*) fd;
	sbfs_core_inode *c_inode = iget(fd);
	
	uint8_t file_offset;
	log_info("Calling bmap with offset: %d", offset);
	int blk_nmbr = bmap(c_inode, offset, &file_offset);
	
	//naive method, assuming everyting fits
	// if(!blk_nmbr){
	// 	c_inode->d_inode.dt_blocks[blk_nmbr] = balloc();
	// }


	char block_buf[SBFS_BLOCK_SIZE];
	log_info("Reading blocknr in write: %d ", blk_nmbr);
	read_block(block_buf, blk_nmbr);
	
	int number=0;
	while(number < count){
		block_buf[file_offset+number]=buf[number];
		number++;
	}
	//file_offset er offset innenfor blokken
	//offset er totalt offset
	//count er hvor langt du skal skrive fra offset
	//total størrelse er gitt 
	log_info("Size of inode %d in write: %d", c_inode->i_nmbr, c_inode->d_inode.size);
	log_info("offset + count in write: %d", offset+count);
	if(offset+count > (c_inode->d_inode.size)){
		c_inode->d_inode.size = offset+count;
	}
	write_block(block_buf, blk_nmbr);
	c_inode->status = 1;
	c_inode->d_inode.a_time = time(NULL);
	c_inode->d_inode.c_time = time(NULL);
	log_info("Size of inode in write after update: %d",c_inode->d_inode.size);
	iput(c_inode);
	return number;

}

size_t sys_read(uint32_t fd, char *buf, size_t count, off_t offset){
	log_info("INSIDE READ");
	sbfs_core_inode *c_node = iget(fd);
	log_info("read inode number: %d", c_node->i_nmbr);
	int number=0;
	uint8_t file_offset;

	int blk_nmbr = bmap(c_node, offset, &file_offset);

	char block_buf[SBFS_BLOCK_SIZE];

	read_block(block_buf, blk_nmbr);

	while(number < count){
		buf[number] = block_buf[file_offset+number];
		number++;
	}

	return number;

}

uint32_t sys_open(const char *path, int flags, mode_t mode){
	sbfs_core_inode *inode = namei(path);

	if(!inode){
		return  -ENOENT;
	}else {
		return (inode->i_nmbr);
	}
}


uint32_t sys_mknod(const char *path, uint8_t file_t, mode_t mode){
	log_info("INSIDE MKNOD");
	sbfs_core_inode *inode = namei(path);

	if(inode != NULL){
		log_info("INODE NOT NULL IN MKNOD");
		return -1; //file already exists
	}

	char *parentc, *parent, *filenamec, *filename;

	parentc = strdup(path);
	filenamec = strdup(path);
	filename = basename(filenamec);
	parent = dirname(parentc);
	
	if(filename != NULL ){
		inode = namei(parent);
		if(inode == NULL) {
			log_info("dir does not exist: %s", parent);
			return -1;
		} 
	}
	log_info("PARENT INODE IN MKNOD: %d", inode->i_nmbr);

	struct dir_entry root_entries[1];

	struct dir_entry *entry = malloc(sizeof(struct dir_entry));
	sbfs_core_inode *new_inode = ialloc();

	entry->name = filename;
	entry->inode_number = new_inode->i_nmbr;;
	entry->offset = sizeof(struct dir_entry);
	entry->file_t = 2;
	root_entries[0] = *entry;
	inode->status = 1;
	inode->d_inode.m_time = time(NULL);
	sys_write(inode->i_nmbr, (char * )root_entries, sizeof(root_entries), inode->d_inode.size);

	new_inode->d_inode.type = file_t;
	new_inode->d_inode.perm = mode & 0770;
	new_inode->d_inode.dt_blocks[0] = balloc();
	iput(new_inode);

	if(file_t == 2){
			
			struct dir_entry dir_entries[2];

			struct dir_entry *dot_entry = malloc(sizeof(struct dir_entry));
			dot_entry->name = ".";
			dot_entry->inode_number = new_inode->i_nmbr;
			dot_entry->offset = sizeof(struct dir_entry);
			dot_entry->file_t = 2;

			struct dir_entry *dot_dot_entry = malloc(sizeof(struct dir_entry));
			dot_dot_entry->name = "..";
			dot_dot_entry->inode_number = new_inode->i_nmbr;
			dot_dot_entry->offset = sizeof(struct dir_entry);
			dot_dot_entry->file_t = 2;
			dir_entries[0] = *dot_entry;
			dir_entries[1] = *dot_dot_entry;	

			new_inode->status = 1;
			new_inode->d_inode.m_time = time(NULL);
			new_inode->d_inode.c_time = time(NULL);
			new_inode->d_inode.link_count = 2;
			sys_write(new_inode->i_nmbr, (char *) &dir_entries, sizeof(dir_entries), 0);
			free(dot_dot_entry);
			free(dot_entry);

	}


	//free(inode);


	
	//free(inode);
	return 0;


}


int sys_mkdir(const char *path, mode_t mode){
	const char *pathname = path;
	int ret = sys_mknod(pathname, 2, mode);
	return ret;
}


int sys_readdir(){
	// int increment = -1;
	// while(1){
	// 	struct dir_entry *entry = malloc(sizeof(struct dir_entry)); 
	//  	increment++;
	//  	entry = get_dir(p_inode, increment*(sizeof(struct dir_entry))); 
	 			
	// 	if((entry == NULL)){ //end of entries for this directory
	// 		log_info("reached end of entries");
	// 		return -1; 
	// 	}
	
	// }
}


int sys_unlink(char *path){
	char *parentc, *parent;

	parentc = strdup(path);

	parent = dirname(parentc);

	sbfs_core_inode *p_inode = namei(parent);
	sbfs_core_inode *c_inode = namei(path);
	int increment = -1;
	while(1){
		struct dir_entry *entry = malloc(sizeof(struct dir_entry)); 
	 	increment++;
	 	entry = get_dir(p_inode, increment*(sizeof(struct dir_entry))); 
	 			
		if((entry == NULL)){ //end of entries for this directory
			log_info("NO ENTRY MATCHES");
			return -1; 
		}
		if(entry->inode_number == c_inode->i_nmbr){
			entry->inode_number = 0;
			c_inode->d_inode.link_count -= 1;
			sys_write(p_inode->i_nmbr, (char *) entry, sizeof(entry), increment*(sizeof(struct dir_entry)));
			iput(p_inode);
			iput(c_inode); //iput takes care of delegating to bfree and ifree
			return 0;
		}
	
	}

	return 0;
}