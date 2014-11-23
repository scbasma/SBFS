#ifndef _INODE_H
#define _INODE_H
#include <stdint.h>

struct sbfs_inode {
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
	uint32_t *dt_blocks; //15 blocks, 12 direct, 13th indirect, 14th double indirect, 15th triple indirect


	int *block_pointers;
};

void init_inode(struct sbfs_inode *inode, char mode, int size, int blk_address);
struct sbfs_inode *get_inode();
struct sbfs_inode allocate_free_inode();
struct sbfs_inode namei(char *path);
int set_free_inode(int inode_number);

#endif