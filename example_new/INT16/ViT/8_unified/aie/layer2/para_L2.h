#ifndef PARA_L2_H
#define PARA_L2_H
#include <adf/stream/types.h>
const int L2_Byte=2;
const int L2_NUM_KERNEL=3;
const int L2_NUM_KERNELS_PER_PAC=3;
const int L2_NUM_PACK_IN=L2_NUM_KERNEL/L2_NUM_KERNELS_PER_PAC;
const int L2_h1=48;
const int L2_w1=32;
const int L2_w2=48;
const int L2_boundary_i=L2_h1/16;
const int L2_boundary_j=L2_w2/2;
const int L2_boundary_k=L2_w1/16-1;
const int L2_jump_B0=L2_w1-8;
const int L2_jump_A0=L2_h1+16;
const int L2_judge_i=L2_boundary_i-1;
const int L2_judge_j=L2_boundary_j-1;

void mm5_kernel0_L2(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L2(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L2(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L2(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif