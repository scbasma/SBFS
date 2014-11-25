#include <stdint.h>

struct dir_entry {
	uint32_t inode_number;
	uint16_t offset;
	char name[FILENAME_MAX_LENGTH];
	//uint8_t name_length;
	uint8_t file_t;
};

