#include <sys/types.h>
#include <sys/stat.h>
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

	//sbfs_core_inode *c_inode = (sbfs_core_inode*) fd;
	sbfs_core_inode *c_inode = iget(fd);
	int number=0;
	uint8_t file_offset;
	log_info("Calling bmap");
	int blk_nmbr = bmap(c_inode, offset, &file_offset);
	
	//naive method, assuming everyting fits
	// if(!blk_nmbr){
	// 	c_inode->d_inode.dt_blocks[blk_nmbr] = balloc();
	// }

	blk_nmbr = bmap(c_inode, offset, &file_offset);


	char block_buf[SBFS_BLOCK_SIZE];
	log_info("Reading blocknr in write: %d ", blk_nmbr);
	read_block(block_buf, blk_nmbr);
	
	while(number < count){
		block_buf[file_offset+number]=buf[number];
		number++;
	}
	//file_offset er offset innenfor blokken
	//offset er totalt offset
	//count er hvor langt du skal skrive fra offset
	//total størrelse er gitt 
	if(offset+count > c_inode->d_inode.size){
		c_inode->d_inode.size = offset+count;
	}
	write_block(block_buf, blk_nmbr);
	c_inode->status = 1;
	c_inode->d_inode.a_time = time(NULL);
	c_inode->d_inode.c_time = time(NULL);
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

uint32_t sys_open(char *path, int flags, mode_t mode){
	sbfs_core_inode *inode = namei(path);

	if(!inode){
		return  -ENOENT;
	}else {
		return (inode->i_nmbr);
	}
}


int32_t sys_mknod(char *path, uint8_t file_t, mode_t mode){
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
	}

	struct dir_entry *entry = malloc(sizeof(struct dir_entry));
	sbfs_core_inode *new_inode = ialloc();

	entry->inode_number = new_inode->i_nmbr;
	strcpy(entry->name, filename);

	sys_write(inode->i_nmbr, (char *) &entry, sizeof(struct dir_entry), inode->d_inode.size);

	new_inode->d_inode.type = file_t;
	new_inode->d_inode.perm = mode & 0770;

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

			sys_write(new_inode->i_nmbr, (char *) &dir_entries, sizeof(dir_entries), 0);
			free(dot_dot_entry);
			free(dot_entry);
			
			new_inode->d_inode.link_count = 2;

	}

	inode->status = 1;
	inode->d_inode.m_time = time(NULL);

	iput(inode);

	new_inode->status = 1;
	new_inode->d_inode.m_time = time(NULL);
	new_inode->d_inode.c_time = time(NULL);

	iput(new_inode);

	return 0;


}


int sys_mkdir(char *path, mode_t mode){
	sys_mknod(path, 2, mode);
	return 0;
}


int sys_readdir(){

}


int sys_unlink(char *path){
	char *parentc, *parent;

	parentc = strdup(path);

	parent = dirname(parentc);

	sbfs_core_inode *p_inode = namei(parent);
	sbfs_core_inode *c_inode = namei(path);

	while(1){
		struct dir_entry *entry = malloc(sizeof(struct dir_entry)); 
	 	struct dir_entry *entry_ptr = entry;
	 
	 	if(sys_read(working_inode->i_nmbr, entry_ptr, sizeof(struct dir_entry), increment*sizeof(struct dir_entry))){
	 			
	 			if(entry_ptr->inode_number == NULL){ //end of entries for this directory
	 				log_info("NO ENTRY MATCHES");
	 				return -1; 
	 			}
	 			if(entry_ptr->inode_number == c_inode->i_nmbr){
	 				entry_ptr->inode_number = 0;
	 				c_inode->d_inode.ref_count -= 1;
	 				iput(p_inode);
	 				iput(c_inode); //iput takes care of delegating to bfree and ifree
	 				return 0;
	 			}
	 		
	 		}
	 	}

	return 0;
}