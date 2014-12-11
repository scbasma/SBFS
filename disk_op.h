#ifndef _SBFS_DISKOP_H
#define _SBFS_DISKOP_H


//int load_disk();
int read_block(void *buf, int block_number);
int write_block(const void *buf, int block_number);

#endif