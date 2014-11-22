#include <stdint.h>

struct sbfs_dir_entry {
	char name[FILENAME_MAX_LENGTH];
	uint16_t offset;
	uint32_t inode_number;
};

