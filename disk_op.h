#include <stdlib.h> 

void load_disk();
void read_block(void *buf, int block_number);
void write_block(void *buf, int block_number);