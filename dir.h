#include <stdint.h>

struct dir_entry {
	uint32_t inode_number;
	uint16_t offset;
	char name[255+1];
	//uint8_t name_length;
	uint8_t file_t;
};


struct dir_entry *get_dir(sbfs_core_inode *c_inode, uint32_t offset);

int add_dir(uint32_t i_nmbr, const struct dir_entry *entry);
