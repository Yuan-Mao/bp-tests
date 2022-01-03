/*
 * Name:
 *   mc_asm_loop.h
 *
 * Description:
 *   This header defines useful inline assembly constructs.
 *
 *   Prior to including this file, LOOP_INSTR must be defined as an inline assembly block
 *
 *   Prior to executing the LOOP; statement in code, amo_target_addr and iters must be defined.
 *   iters has C type int, amo_target_addr has C type uint64_t*
 *
 */

#ifndef MC_ASM_LOOP_H
#define MC_ASM_LOOP_H

#define LOOP_INNER \
{                  \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
  LOOP_INSTR;      \
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
    : "r" (amo_target_addr), "r" (iters) \
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

#endif
