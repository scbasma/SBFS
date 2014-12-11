#include <stdlib.h>
#include "bitmap.h"
#include "dbg.h"



void setBit(int *bitmap, int pos){
	unsigned int ptPos = pos/32;
	unsigned int bitPos = pos%32;

	unsigned int shifter = 1;
	// int i;
	// for(i = 0; i < pos; i++){
	// 	*bitmap++;
	// }
	unsigned int bit = (bitmap[ptPos] | (shifter<<bitPos));
	bitmap[ptPos] = bit;
	
}

int getBit(int *bitmap, int pos){
	int ptPos = pos/32;
	int bitPos = pos%32;
	unsigned int shifter = 1;
	int i;
	for(i = 0; i < ptPos; i++){
		bitmap++;
	}
	unsigned int bit = (*bitmap & (shifter<<bitPos)) != 0;
	return bit;  

}

int clearBit(int *bitmap, int pos){

	int ptPos = pos/32;
	int bitPos = pos%32;

	unsigned int shifter = 1;
	int i;
	for(i = 0; i < ptPos; i++){
		bitmap++;
	}

	*bitmap = *bitmap & (~(shifter << bitPos));
	return 0;
}



// int main(int argc, int *argv[]){
// 	int *memory_block = (int*) calloc(1024, sizeof(int));
// 	int i;
// 	int *i_bitmap;
// 	i_bitmap = memory_block;

// 	for(i=0; i<100; i++){
// 		setBit(i_bitmap, i);
// 	}
	
// 	for(i = 0; i < 100; i++){
// 		if(getBit(i_bitmap, i) != 1){
// 			log_info("Bit not 1 at pos: %d", i);
// 		}
// 	}

// 	for(i = 0; i < 10; i++){
// 		clearBit(i_bitmap, i);
// 	}

// 	for(i = 0; i < 10; i++){
// 		if(getBit(i_bitmap, i) != 0){
// 			log_info("Bit not 0 at pos: %d", i);
// 		}
// 	}

// 	free(i_bitmap);
// 	return 0;

// }



