#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "datablock.h"
#include "sbfs.h"
#include "dbg.h"

uint32_t balloc(){
		uint32_t i;
		uint32_t *d_bitmap;
		int block_nmbr = 1;
		while(block_nmbr < (NUMBER_OF_DATA_BITMAP_BLOCKS+1)){
			d_bitmap = malloc(SBFS_BLOCK_SIZE);
			read_block(d_bitmap, block_nmbr);
			for(i = 0; i < SBFS_BLOCK_SIZE*8; i++){
				int free_block = getBit(d_bitmap, i);
				if(free_block == 0){
					log_info("block number i in balloc: %d", i);
					uint32_t free_block_number = i*block_nmbr;
					setBit(d_bitmap, i);
					write_block(d_bitmap, block_nmbr);
					free(d_bitmap);
					return free_block_number;
				}
			}
			block_nmbr++;
		}

		free(d_bitmap);
		return 0; //nothing found, 
}

void bfree(uint32_t block_nmbr){
	int block_pos = (block_nmbr/(SBFS_BLOCK_SIZE*8)) + 1; //data blocks bitmap start at 1
	int block_bit = block_nmbr%(SBFS_BLOCK_SIZE*8);
	int *d_bitmap = malloc(SBFS_BLOCK_SIZE);
	read_block(d_bitmap, block_pos);
	clearBit(d_bitmap, block_bit);
	write_block(d_bitmap, block_pos);
	free(d_bitmap);
}