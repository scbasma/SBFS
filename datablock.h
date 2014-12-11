#ifndef _DATABLOCK_H
#define _DATABLOCK_H

#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


uint32_t balloc();
void bfree(uint32_t block);
int init_dt_blks();

#endif