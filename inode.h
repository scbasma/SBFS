

struct sbfs_inode {
	char mode;
	int size;
	int block_address;
	int *block_pointers;
};

void init_inode(struct sbfs_inode *inode, char mode, int size, int blk_address);
int namei(char *path);
int init_inode_list();