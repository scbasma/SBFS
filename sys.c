#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "inode.h"
#include "disk_op.h"
#include "spblk.h"
#include "sbfs.h"
#include "dbg.h"
#include "time.h"

size_t sys_write(uint64_t fd, const char *buf, size_t count, off_t offset){

	sbfs_core_inode *c_node = (sbfs_core_inode*) fd;
	
	int number=0;
	uint8_t file_offset;
	log_info("Calling bmap");
	int blk_nmbr = bmap(c_node, offset, &file_offset);
	
	//naive method, assuming everyting fits
	if(!blk_nmbr){
		c_node->d_inode.dt_blocks[blk_nmbr] = get_free_dblock();
	}

	blk_nmbr = bmap(c_node, offset, &file_offset);


	char block_buf[SBFS_BLOCK_SIZE];

	read_block(block_buf, blk_nmbr);
	
	while(number < count){
		block_buf[file_offset+number]=buf[number];
		number++;
	}
	write_block(block_buf, blk_nmbr);
	c_inode->status = 1;
	c_inode->d_inode.a_time = time(NULL);
	c_inode->d_inode.c_time = time(NULL);

	return number;

}

size_t sys_read(uint64_t fd, char *buf, size_t count, off_t offset){
	sbfs_core_inode *c_node = (sbfs_core_inode*) fd;
	
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