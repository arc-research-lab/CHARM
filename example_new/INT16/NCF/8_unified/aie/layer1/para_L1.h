#ifndef PARA_L1_H
#define PARA_L1_H
#include <adf/stream/types.h>
const int L1_Byte=2;
const int L1_NUM_KERNEL=3;
const int L1_NUM_KERNELS_PER_PAC=3;
const int L1_NUM_PACK_IN=L1_NUM_KERNEL/L1_NUM_KERNELS_PER_PAC;
const int L1_h1=48;
const int L1_w1=32;
const int L1_w2=48;
const int L1_boundary_i=L1_h1/16;
const int L1_boundary_j=L1_w2/2;
const int L1_boundary_k=L1_w1/16-1;
const int L1_jump_B0=L1_w1-8;
const int L1_jump_A0=L1_h1+16;
const int L1_judge_i=L1_boundary_i-1;
const int L1_judge_j=L1_boundary_j-1;

void mm5_kernel0_L1(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L1(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L1(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L1(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif