#include <stdlib.h> 

//int load_disk();
int read_block(void *buf, int block_number);
int write_block(void *buf, int block_number);