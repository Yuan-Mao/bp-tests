/*
 * Name:
 *   mc_sync.c
 *
 * Description:
 *   This program has each core repeatedly synchronize on a global variable.
 *
 *   N defines the number of iterations each core executes.
 */

#include <stdint.h>
#include "bp_utils.h"
#include "mc_util.h"
#include "aviary.h"

#ifndef N
#define N 512
#endif

volatile uint64_t __attribute__((aligned(64))) global_lock = 0;
volatile uint64_t __attribute__((aligned(64))) start_barrier_mem = 0;
volatile uint64_t __attribute__((aligned(64))) end_barrier_mem = 0;

uint64_t thread_main(uint64_t core_id, uint32_t num_cores, uint32_t iterations) {
  uint64_t sum = 0;
  for (int i = 0; i < iterations; i++) {
    sum += 1;
  }

  // synchronize at end of computation by incrementing the end barrier
  lock(&global_lock);
  end_barrier_mem += 1;
  unlock(&global_lock);

  return sum;
}

uint64_t main(uint64_t argc, char * argv[]) {
  uint64_t core_id;
  __asm__ volatile("csrr %0, mhartid": "=r"(core_id): :);
  uint32_t num_cores = bp_param_get(PARAM_CC_X_DIM) * bp_param_get(PARAM_CC_Y_DIM);

  // all threads execute
  uint64_t sum = thread_main(core_id, num_cores, N);

  if (core_id == 0) {
    // core 0 waits for all threads to finish
    // wait for all threads to finish
    while (end_barrier_mem != num_cores) { }
    if (sum != (uint64_t)N) {
      return 1;
    }
    return 0;
  } else {
    if (sum != (uint64_t)N) {
      bp_finish(1);
    } else {
      bp_finish(0);
    }
    while (1) { }
  }

  // no core should reach this, return non-zero (error)
  return 1;
}
