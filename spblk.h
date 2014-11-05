#include <stdlib.h>

struct sbfs_sp_blk{
	int version;
	int magic_number;
	int block_count;
	int block_size;
	int A;
	int number_of_inodes;
	int next_free_inode;
	int next_free_data_block;
};

struct sbfs_sp_blk *init_sp_blk();
int get_next_free_data_block(struct sbfs_sp_blk *sp_blk);
int get_next_free_inode(struct sbfs_sp_blk *sp_blk);
size_t get_spblk_size();


