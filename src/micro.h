/*
 * Name:
 *   micro.h
 *
 * Description:
 *   This header defines some useful functions for microbenchmark tests
 *
 */

#ifndef MICRO_H
#define MICRO_H

typedef unsigned char * ADDR;

#include "micro_data.h"

// L1 data cache parameters
// can be overridden by compiler -D flags
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 64
#endif

#ifndef CACHE_ASSOC
#define CACHE_ASSOC 8
#endif

#ifndef CACHE_SETS
#define CACHE_SETS 64
#endif

// derived cache parameters
#define CACHE_SIZE (BLOCK_SIZE*CACHE_ASSOC*CACHE_SETS)
#define WAY_SIZE (BLOCK_SIZE*CACHE_SETS)
#define SET_SIZE (BLOCK_SIZE*CACHE_ASSOC)
#define CACHE_BLOCKS (CACHE_SETS*CACHE_ASSOC)

// total data bytes and blocks
#define DATA_BYTES (DATA_LEN*sizeof(uint32_t))
#define DATA_BLOCKS (DATA_BYTES/BLOCK_SIZE)
#define DATA_BLOCK_WORDS (BLOCK_SIZE/sizeof(uint32_t))

#endif
