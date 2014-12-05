#ifndef _SBFS_H
#define _SBFS_H
#include <stdlib.h> 
#include "spblk.h"
#include "inode.h"

#define SBFS_BLOCK_SIZE 4096
#define SBFS_NUMBER_OF_BLOCKS 32768 //4 blocks of data bitmaps
#define SBFS_DISK_SIZE SBFS_BLOCK_SIZE*SBFS_NUMBER_OF_BLOCKS
#define SBFS_NUMBER_OF_INODES 65536 // equals two blocks of bitmaps
//#define SBFS_NUMBER_OF_DATA_BLOCKS 53
#define ROOT_INODE_NUMBER 2
#define INODE_NMBR_PER_BLOCK (SBFS_BLOCK_SIZE/sizeof(sbfs_disk_inode))
#define NUMBER_OF_INODE_BLOCKS SBFS_NUMBER_OF_INODES/INODE_NMBR_PER_BLOCK
#define FIRST_DATA_BLOCK ROOT_INODE_NUMBER+NUMBER_OF_BITMAP_BLOCKS+NUMBER_OF_INODE_BLOCKS  
#define FILENAME_MAX_LENGTH 255
#define MAX_NUMBER_OF_FILES_IN_DIR 1000 
#define FIRST_BITMAP_BLOCK_POS 1 //spblk at 0
#define NUMBER_OF_INODE_BITMAP_BLOCKS 2
#define NUMBER_OF_DATA_BITMAP_BLOCKS 4
#define NUMBER_OF_BITMAP_BLOCKS NUMBER_OF_INODE_BITMAP_BLOCKS+NUMBER_OF_DATA_BITMAP_BLOCKS
#define FIRST_INODE_BLOCK_NMBR NUMBER_OF_BITMAP_BLOCKS+1 
#define NMBR_INDIRECT_POINTERS_IN_BLOCK SBFS_BLOCK_SIZE/(sizeof(uint32_t))



extern void *disk;
//extern int disk;

extern sbfs_sp_blk *sp_blk;


//int sbfs_mknod(const char *pathname, mode_t mode, dev_t dev);

// 1. Finn inode (sett den incore med namei)
// 2. initialiser inoden
// 3. Lag dir entry (sett inode nummer og filnavn). Dir entry er bare en spesiell fil som inneholder filnavn og inode nummer



//int sbfs_open(char *pathname, mode_t mode);
//int sbfs_write(char *pathname, const char *buf);

#endif