#include <stdlib.h>

struct sbfs_sp_blk{
	uint16_t version;
	uint32_t magic_number;
	uint32_t block_count;
	uint16_t block_size;
	uint32_t first_free_inode;
	uint32_t number_of_inodes;
	uint16_t next_free_inode;
	uint32_t next_free_data_block;
	uint32_t first_free_data_block; //the block spblk is written to
	uint32_t m_time; //last mounted
	uint32_t w_time; //last write
	uint16_t fs_state;
};

struct sbfs_sp_blk *init_sp_blk();
int get_next_free_data_block(struct sbfs_sp_blk *sp_blk);
int get_next_free_inode(struct sbfs_sp_blk *sp_blk);
size_t get_spblk_size();


