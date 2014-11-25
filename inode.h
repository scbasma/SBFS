#ifndef _INODE_H
#define _INODE_H
#include <stdint.h>

typedef struct {
	uint16_t mode;
	uint16_t user_id;
	uint32_t size;
	uint32_t a_time;
	uint32_t m_time;
	uint32_t c_time;
	uint16_t grp_id;
	uint16_t link_count;
	uint32_t res_blocks_nmbr;
	uint32_t flags;
	uint32_t dt_blocks[15]; //15 blocks, 12 direct, 13th indirect, 14th double indirect, 15th triple indirect
}sbfs_disk_inode;

typedef struct {
	sbfs_disk_inode d_inode;

	uint8_t status;
	uint32_t i_nmbr;
	uint16_t ref_count;


	// sbfs_core_inode *next_free;
	// sbfs_core_inode *prev_free;
	// sbfs_core_inode *next_hash;
	// sbfs_core_inode *prev_hash;

} sbfs_core_inode;

void init_inode(sbfs_disk_inode *inode, char mode, int size, int blk_address);

sbfs_core_inode *iget(uint32_t i_nmbr);


void iput(sbfs_core_inode *c_inode);

sbfs_core_inode *allocate_free_inode();

sbfs_core_inode namei(char *path);

int set_free_inode(int inode_number);

#endif