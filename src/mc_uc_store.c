
#include <stdint.h>
#include <bp_utils.h>
#include "mc_util.h"

#define N 100

#define LOOP_INNER \
{                  \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
  __asm__ __volatile__ ("sd a5, (a4)"); \
}

#define LOOP_BODY \
{                 \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
  LOOP_INNER;     \
}

#define LOOP \
{            \
  __asm__ goto __volatile__ ( \
    "mv a4, %0;" \
    "li a5, 1;" \
    "li a6, 0;" \
    "mv a7, %1;" \
    "j %l2;" \
    : \
    : "r" (target_addr), "r" (iters) \
    : "a4", "a5", "a6", "a7"\
    : loop_check \
    ); \
  loop_body:\
  LOOP_BODY;\
  loop_check:\
  __asm__ goto __volatile__ ( \
    "addi a6, a6, 1;" \
    "bge a7, a6, %l0;" \
    : \
    : \
    : "a4", "a5", "a6", "a7"\
    : loop_body \
    ); \
}

volatile uint64_t __attribute__((aligned(64))) global_lock = 0;
volatile uint64_t __attribute__((aligned(64))) end_barrier_mem = 0;

uint64_t main(uint64_t argc, char * argv[]) {

  uint64_t core_id;
  __asm__ volatile("csrr %0, mhartid": "=r"(core_id): :);
  uint32_t num_cores = bp_param_get(PARAM_CC_X_DIM) * bp_param_get(PARAM_CC_Y_DIM);

  char* target_addr = PUTCHAR_BASE_ADDR;
  int iters = N/100;
  LOOP;

  lock(&global_lock);
  end_barrier_mem += 1;
  unlock(&global_lock);

  if (core_id == 0) {
    // core 0 waits for all threads to finish
    // wait for all threads to finish
    while (end_barrier_mem != num_cores) { }
    return 0;
  } else {
    bp_finish(0);
    while (1) { }
  }

  // no core should reach this, return non-zero (error)
  return 1;
}
