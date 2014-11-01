#include <stdlib.h> 


#define SBFS_BLOCK_SIZE 4096
#define SBFS_NUMBER_OF_BLOCKS 64
#define SBFS_DISK_SIZE SBFS_BLOCK_SIZE*SBFS_NUMBER_OF_BLOCKS
#define START_INODE_NUMBER 2  
#define FILENAME_MAX_LENGTH 255

extern char *disk;


//for each file in a directory, store filename and inode_number

struct sbfs_dir_entry {
	char filename[FILENAME_MAX_LENGTH];
	int inode_number;
};


struct sbfs_super_block {
	int version;
	int magic_number;
	int block_count;
	int block_size;
	int *free_block;
	int inode_number;
	int *free_inodes;
};

struct sbfs_inode {
	char mode;
	int size;
	int blocks;
	int *block_pointers;
};