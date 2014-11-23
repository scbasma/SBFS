#include <stdint.h>

typedef enum {0, 1, 2, 3, 4, 5, 6, 7} file_type;

struct dir_entry {
	uint32_t inode_number;
	uint16_t offset;
	char *name;
	uint8_t name_length;
	file_type file;
};

