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
	log_info("SYS WRITE RECEIVED OFFSET OF %d", (int) offset);
	buf = (char *) buf;
	//sbfs_core_inode *c_inode = (sbfs_core_inode*) fd;
	sbfs_core_inode *c_inode = iget(fd);
	
	uint32_t file_offset;
	log_info("Giving bmap with inode: %d, offset: %d", c_inode->i_nmbr, (int)offset);
	int blk_nmbr = bmap(c_inode, offset, &file_offset);
	log_info("File offset from bmap: %d", (int) file_offset);
	//naive method, assuming everyting fits
	// if(!blk_nmbr){
	// 	c_inode->d_inode.dt_blocks[blk_nmbr] = balloc();
	// }



	log_info("BLOCK NMBR FROM BMAP: %d", blk_nmbr);
	int i;
	int number=0;
	int write_count = 0;
	while(number < count){
		
		// if(number >= SBFS_BLOCK_SIZE){
		// 	number=0;
		// 	write_block(block_buf, blk_nmbr);
		// 	free(block_buf);
		// 	block_buf = calloc(1, SBFS_BLOCK_SIZE);
		// 	blk_nmbr = bmap(c_inode, offset+write_count, &file_offset);
		// 	read_block(block_buf, blk_nmbr);
		// }
		
		blk_nmbr = bmap(c_inode, offset+write_count, &file_offset);
		char * block_buf=calloc(1, SBFS_BLOCK_SIZE);
		read_block(block_buf, blk_nmbr);
		for(i = file_offset; i < SBFS_BLOCK_SIZE; i++){
			if(number >= count){
				break;
			}
			block_buf[i]=buf[number];
			number++;
		}

		write_block(block_buf, blk_nmbr);
		free(block_buf);
	}
	//file_offset er offset innenfor blokken
	//offset er totalt offset
	//count er hvor langt du skal skrive fra offset
	//total størrelse er gitt 
	if(offset+count > (c_inode->d_inode.size)){
		c_inode->d_inode.size = offset+count;
		log_info("Updating size of inode to: %d", (int) (offset+count));
	}

	c_inode->status = 1;
	c_inode->d_inode.a_time = time(NULL);
	c_inode->d_inode.c_time = time(NULL);
	iput(c_inode);
	free(c_inode);
	return number;

}

size_t sys_read(uint32_t fd, char *buf, size_t count, off_t offset){
	// log_info("inode_number passed to iget from read: %d", fd);
	// sbfs_core_inode *c_node = iget(fd);
	// int number=0;
	// uint8_t file_offset;
	// if(!buf){
	// 	return number;
	// }
	// int blk_nmbr = bmap(c_node, offset, &file_offset);
	// number = file_offset;
	// char block_buf[SBFS_BLOCK_SIZE];

	// read_block(block_buf, blk_nmbr);
	// int read_count = 0;
	// while(read_count < count && read_count < c_node->d_inode.size){

	// 	if(number >= SBFS_BLOCK_SIZE){
	// 		blk_nmbr = bmap(c_node, offset+read_count, &file_offset);
	// 		number = file_offset;
	// 		read_block(block_buf, blk_nmbr);
	// 	}

	// 	buf[read_count] = block_buf[file_offset+number];
	// 	number++;
	// 	read_count++;
	// }

	// return read_count;
	sbfs_core_inode* inode = iget(fd);
  uint32_t file_offset;
	uint32_t blk_nmr;
    if (!inode)
        return -1;

    int read_count = 0;

    while (read_count < count) {

        if (offset < inode->d_inode.size) {
            blk_nmr = bmap(inode, offset, &file_offset);

            if (blk_nmr <= 0)
                break;
        } else
            break;

        char * block_buf=calloc(1, SBFS_BLOCK_SIZE);
       	read_block(block_buf, blk_nmr);

        int i;
        for (i = file_offset; i < SBFS_BLOCK_SIZE; i++)
        {
            if (read_count >= count)
                break;

            buf[read_count] = block_buf[i];

            read_count++;
        }
        offset += i - file_offset;
        free(block_buf);
    }
    free(inode);
    return count;

}

uint32_t sys_open(const char *path, int flags, mode_t mode){
	sbfs_core_inode *inode = namei(path);

	if(!inode){
		free(inode);
		return  -ENOENT;
	}else {
		int i_nmbr = inode->i_nmbr;
		free(inode);
		return (i_nmbr);
	}
}


uint32_t sys_mknod(const char *path, uint8_t file_t, mode_t mode){
	log_info("sys_mknod FILE TYPE: %d", file_t);
	sbfs_core_inode *inode = namei(path);
	if(inode != NULL){
		free(inode);
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
			free(parentc);
			free(filenamec);
			return -1;
		} 
	}

	//struct dir_entry root_entries[1];

	struct dir_entry *entry = calloc(1, sizeof(struct dir_entry));
	sbfs_core_inode *new_inode = ialloc();
	strcpy(entry->name,filename);
	entry->inode_number = new_inode->i_nmbr;
	log_info("MKNOD NEW ENTRY: %s, with i_nmbr: %d, with parent: %s with inmbr: %d and size: %d", path, (int) entry->inode_number, parent, inode->i_nmbr, (int)inode->d_inode.size);
	entry->offset = sizeof(struct dir_entry);
	entry->file_t = file_t;
	//root_entries[0] = *entry;
	inode->status = 1;
	inode->d_inode.m_time = time(NULL);
	inode->d_inode.a_time = time(NULL);
	//sys_write(inode->i_nmbr, (char * )root_entries, sizeof(root_entries), inode->d_inode.size);
	log_info("NEW INODE NMBR: %d", new_inode->i_nmbr);
	add_dir(inode->i_nmbr, entry);
	new_inode->d_inode.type = file_t;
	new_inode->d_inode.perm = 0777;
	new_inode->d_inode.dt_blocks[0] = balloc();

	new_inode->status = 1;
	new_inode->d_inode.m_time = time(NULL);
	new_inode->d_inode.c_time = time(NULL);
	new_inode->d_inode.a_time = time(NULL);
	new_inode->d_inode.link_count = 2;
	new_inode->d_inode.user_id = getuid();
	new_inode->d_inode.grp_id = getgid();
	iput(new_inode);

	if(file_t == 2){
			
			//struct dir_entry dir_entries[2];

			struct dir_entry *dot_entry = malloc(sizeof(struct dir_entry));
			strcpy(dot_entry->name, ".");
			dot_entry->inode_number = new_inode->i_nmbr;
			dot_entry->offset = sizeof(struct dir_entry);
			dot_entry->file_t = 2;
			add_dir(new_inode->i_nmbr, dot_entry);
			struct dir_entry *dot_dot_entry = malloc(sizeof(struct dir_entry));
			strcpy(dot_dot_entry->name, "..");
			dot_dot_entry->inode_number = new_inode->i_nmbr;
			dot_dot_entry->offset = sizeof(struct dir_entry);
			dot_dot_entry->file_t = 2;
			// dir_entries[0] = *dot_entry;
			// dir_entries[1] = *dot_dot_entry;	;
			add_dir(new_inode->i_nmbr, dot_dot_entry);
			//sys_write(new_inode->i_nmbr, (char *) &dir_entries, sizeof(dir_entries), 0);
			free(dot_dot_entry);
			free(dot_entry);

	}
	free(parentc);
	free(filenamec);
	// free(inode);
	// free(new_inode);
	// free(entry->name);
	// free(entry);
	return 0;


}


int sys_mkdir(const char *path, mode_t mode){
	const char *pathname = path;
	int ret = sys_mknod(pathname, 2, mode);
	return ret;
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
			return -1; 
		}
		if(entry->inode_number == c_inode->i_nmbr){
			strcpy(entry->name, "\0");
			entry->inode_number = 0;
			c_inode->d_inode.link_count -= 1;
			sys_write(p_inode->i_nmbr, (char *) entry, sizeof(entry), increment*(sizeof(struct dir_entry)));
			//add_dir(p_inode->i_nmbr, entry);
			iput(p_inode);
			iput(c_inode); //iput takes care of delegating to bfree and ifree
			return 0;
		}
	
	}

	return 0;
}