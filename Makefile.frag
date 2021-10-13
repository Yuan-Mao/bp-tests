BP_TESTS_C = \
  hello_world           \
  instr_coherence       \
  cache_hammer          \
  jalr_illegal          \
  satp_nofence          \
  timer_interrupt       \
  loop                  \
  cache_flush           \
  stream_hammer         \
  domain_fault          \
  eaddr_fault           \
  paging                \
  mapping               \
  mstatus_fs            \
  wfi                   \
  uncached_mode         \
  amo_nonblocking       \
  amo_interrupt         \
  divide_hazard         \
  unhandled_trap        \
  aviary_rom            \
  readonly              \
  epc                   \
  fflags_haz            \
  dram_stress           \
  misaligned_ldst       \

BP_MULTICORE_TESTS_C = \
  mc_sanity             \
  mc_template           \
  mc_rand_walk          \
  mc_work_share_sort    \
  mc_amo_unroll         \
  mc_uc_store           \

BP_ACCELERATOR_TESTS_C = \
  streaming_accelerator_vdp      \
  streaming_accelerator_loopback \
  streaming_accelerator_zipline  \
  coherent_accelerator_vdp       \

BP_TESTS_CPP = \
  virtual               \
  constructor           \
  template              \
  unwinding             \
  vector                \
  map                   \

BP_TESTS = $(BP_TESTS_C) $(BP_MULTICORE_TESTS_C) $(BP_ACCELERATOR_TESTS_C) $(BP_TESTS_CPP)

