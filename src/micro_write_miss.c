
#include <stdint.h>
#include "micro.h"
#include "bp_utils.h"
#include "mc_util.h"
#include "aviary.h"

volatile uint64_t __attribute__((aligned(64))) global_lock = 0;
volatile uint64_t __attribute__((aligned(64))) end_barrier_mem = 0;

int main(int argc, char **argv) {

  uint64_t core_id;
  __asm__ volatile("csrr %0, mhartid": "=r"(core_id): :);
  uint32_t num_cores = bp_param_get(PARAM_CC_X_DIM) * bp_param_get(PARAM_CC_Y_DIM);

  // perform accesses - make sure each core accesses a block from each CCE
  // blocks are striped across CCEs
  // iter 0: core 0 accesses block 0, core 1 accesses block 1
  // iter 1: core 0 accesses block 1, core 1 accesses block 2
  // each iteration, num_cores blocks are accessed by the num_cores cores

  // number of words per iteration
  uint64_t stride = (DATA_BLOCK_WORDS * num_cores);
  // start at word 0 in the data arrray
  uint32_t base_word = 0;

  for (int iter = 0; iter < num_cores; iter++) {
    // which block to access this iteration, as offset from current set of blocks
    uint32_t block = (core_id + iter) % num_cores;
    // compute word to access in DATA array
    uint32_t data_word = base_word + (block * DATA_BLOCK_WORDS);
    // perform write
    DATA[data_word] = 255;
    // increment the base_word each iteration
    base_word += stride;
  }

  if (core_id == 0) {
    // synchronize at end of computation by incrementing the end barrier
    lock(&global_lock);
    end_barrier_mem += 1;
    unlock(&global_lock);
    // core 0 waits for all threads to finish
    // wait for all threads to finish
    while (end_barrier_mem != num_cores) { }
    return 0;
  } else {
    // synchronize at end of computation by incrementing the end barrier
    lock(&global_lock);
    end_barrier_mem += 1;
    bp_finish(0);
    unlock(&global_lock);
    while (1) { }
  }

  // no core should reach this, return non-zero (error)
  return 1;
}
