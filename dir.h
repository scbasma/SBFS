

struct sbfs_dir_entry {
	char name[FILENAME_MAX_LENGTH];
	int size;
	int inode_number;
};

struct sbfs_dir_entry entries[MAX_NUMBER_OF_FILES_IN_DIR]; 
