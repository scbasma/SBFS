#include <stdlib.h> 


#define SBFS_BLOCK_SIZE 4096
#define SBFS_NUMBER_OF_BLOCKS 64
#define SBFS_DISK_SIZE SBFS_BLOCK_SIZE*SBFS_NUMBER_OF_BLOCKS
#define SBFS_NUMBER_OF_INODES 10
#define SBFS_NUMBER_OF_DATA_BLOCKS 53
#define ROOT_INODE_NUMBER 3
#define FIRST_DATA_BLOCK ROOT_INODE_NUMBER+SBFS_NUMBER_OF_INODES  
#define FILENAME_MAX_LENGTH 255
#define MAX_NUMBER_OF_FILES_IN_DIR 20

extern char *disk;


//int sbfs_mknod(const char *pathname, mode_t mode, dev_t dev);

// 1. Finn inode (sett den incore med namei)
// 2. initialiser inoden
// 3. Lag dir entry (sett inode nummer og filnavn). Dir entry er bare en spesiell fil som inneholder filnavn og inode nummer



//int sbfs_open(char *pathname, mode_t mode);
//int sbfs_write(char *pathname, const char *buf);