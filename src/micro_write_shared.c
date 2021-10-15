
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

  // cores > 0 read block then spin
  if (core_id > 0) {
    uint32_t read_value = DATA[0];
    lock(&global_lock);
    end_barrier_mem += 1;
    bp_finish(0);
    unlock(&global_lock);
    while (1) { }
  } else {
    lock(&global_lock);
    end_barrier_mem += 1;
    unlock(&global_lock);
    // core 0 waits for all threads to finish
    // wait for all threads to finish
    while (end_barrier_mem != num_cores) { }
    uint32_t write_value = 255;
    DATA[0] = write_value;
    return 0;
  }

  // no core should reach this, return non-zero (error)
  return 1;
}
