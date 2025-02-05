#ifndef VDP_H
#define VDP_H
#include <stdint.h>
#include "aviary.h"

////////////////////////////////BP-CONFIG///////////////////////////////////
#define BP_CONFIG (bp_param_get(PARAM_CC_X_DIM)*bp_param_get(PARAM_CC_Y_DIM))
#define BP_CFG_WIDTH 24
#define CACCEL_Y_DIM bp_param_get(PARAM_CC_Y_DIM)
#define CACCEL_VDP_BASE_ADDR (uint64_t *) ((uint64_t) BP_CONFIG<<BP_CFG_WIDTH) 
#define SACCEL_VDP_BASE_ADDR (uint64_t *) ((uint64_t) (BP_CONFIG+CACCEL_Y_DIM)<<BP_CFG_WIDTH)
#define BP_DADDR_WIDTH bp_param_get(PARAM_DADDR_WIDTH)
#define SACCEL_MEM_BASE   (uint64_t *)((uint64_t) 1<<(BP_DADDR_WIDTH))
/////////////////////////////////VDP/////////////////////////////////////////
#define ACCEL_VPD_INPUT_A_PTR    0
#define ACCEL_VPD_INPUT_B_PTR    1
#define ACCEL_VPD_INPUT_LEN      2
#define ACCEL_VPD_START_CMD      3
#define ACCEL_VPD_RESP_STATUS    4
#define ACCEL_VPD_RESP_PTR       5
#define ACCEL_VPD_RESP_LEN       6
#define ACCEL_VPD_OPERATION      7
#define ACCEL_VPD_NUM_CSRs       8

struct VDP_CSR
{ 
  uint64_t *input_a_ptr;
  uint64_t *input_b_ptr;
  uint64_t input_length;
  uint64_t *resp_ptr;
  uint64_t *tlv_header_ptr;
};

void bp_set_mmio_csr64(uint64_t *accel_base_address, uint8_t csr_idx, uint64_t csr_value);
uint64_t bp_get_mmio_csr64(uint64_t *accel_base_address, uint8_t csr_idx); 
void bp_set_mmio_csr32(uint32_t *accel_base_address, uint8_t csr_idx, uint32_t csr_value);
uint32_t bp_get_mmio_csr32(uint32_t *accel_base_address, uint8_t csr_idx); 
void dma_cpy(uint64_t *src, uint64_t *dest, uint64_t length);
void bp_hw_dma(uint64_t *cfg_base_dma_addr, uint64_t *src, uint64_t length, uint64_t type);
void bp_vdp_config_accelerator(uint64_t *base_cfg_addr, uint64_t *input_a_ptr,
                               uint64_t *input_b_ptr, uint64_t input_length, 
                               uint64_t operation, uint64_t *resp_ptr, uint64_t resp_length);
void bp_vdp_accelerator_start_cmd(uint64_t *base_cfg_addr);
void bp_wait_for_completion(uint64_t *base_cfg_addr, uint64_t csr_idx);
void bp_call_vector_dot_product_accelerator(uint8_t type, struct VDP_CSR vdp_csrs);

/////////////////////////////////LOOPBACK////////////////////////////////////
#define ACCEL_LOOPBACK_WR_CNT    0
uint64_t bp_call_loopback_accelerator(uint64_t *input_ptr, uint64_t *resp_ptr, uint64_t len);

/////////////////////////////////ZIPLINE/////////////////////////////////////
//Zipline CSR IDX
#define TLV_TYPE       0 
#define DATA_TLV_LEN   1
//HW DMA CSR IDX 
#define DATA_PTR       0
#define DATA_LEN       1
#define START_DMA      2
#define DONE_DMA       3
#define DMA_TYPE       4 //0:read, 1:write


struct dma_cfg 
{
  uint64_t *data_ptr;
  uint64_t length;
  uint8_t  type;//0:req, 1:cmd, 2:frmd, 3:data, 4:cqe 
};

struct Zipline_CSR
{
  struct dma_cfg *input_ptr;
  uint64_t *resp_ptr;
};

uint64_t bp_call_zipline_accelerator(uint8_t type, struct Zipline_CSR vdp_csrs, uint64_t input_tlv_num);
#endif
