/*
 * Name:
 *   mc_sanity.c
 *
 * Description:
 *   This program has each core write to unique entries in memory and then locally sum
 *   the values. The cores deliberately false share cache blocks, forcing the coherence
 *   system to transfer blocks between the cores as a basic sanity check of coherence
 *   functionality in a multicore system.
 *
 *   MATRIX_SIZE defines the maximum size of the data matrix used in the test.
 *   N defines the number of matrix elements accessed per core.
 *   Constraint: N * num_cores <= MATRIX_SIZE
 */

#include <stdint.h>
#include "bp_utils.h"
#include "mc_util.h"
#include "aviary.h"

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 8192
#endif

#ifndef N
#define N 512
#endif

volatile uint64_t __attribute__((aligned(64))) global_lock = 0;
volatile uint64_t __attribute__((aligned(64))) start_barrier_mem = 0;
volatile uint64_t __attribute__((aligned(64))) end_barrier_mem = 0;

typedef uint64_t matrix[MATRIX_SIZE];
matrix MATRIX;

uint64_t thread_main(uint64_t core_id, uint32_t num_cores, uint32_t iterations) {
  uint64_t sum = 0;
  for (int i = 0; i < iterations; i++) {
    MATRIX[i*num_cores + core_id] = 1;
    sum += MATRIX[i*num_cores + core_id];
    if (sum != i+1) {
      // break early if sum differs from expected value
      // synchronize on end barrier then return current sum
      lock(&global_lock);
      end_barrier_mem += 1;
      unlock(&global_lock);
      return sum;
    }
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
  // fail if matrix is too small
  if (N*num_cores > MATRIX_SIZE) {
    return 1;
  }

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
