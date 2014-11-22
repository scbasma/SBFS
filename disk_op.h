#include <stdlib.h> 

int load_disk();
void read_block(void *buf, int block_number);
int write_block(void *buf, int block_number);