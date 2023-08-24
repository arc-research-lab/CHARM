#ifndef PARA_L4_H
#define PARA_L4_H
#include <adf/stream/types.h>
const int L4_Byte=2;
const int L4_NUM_KERNEL=3;
const int L4_NUM_KERNELS_PER_PAC=3;
const int L4_NUM_PACK_IN=L4_NUM_KERNEL/L4_NUM_KERNELS_PER_PAC;
const int L4_h1=48;
const int L4_w1=32;
const int L4_w2=48;
const int L4_boundary_i=L4_h1/16;
const int L4_boundary_j=L4_w2/2;
const int L4_boundary_k=L4_w1/16-1;
const int L4_jump_B0=L4_w1-8;
const int L4_jump_A0=L4_h1+16;
const int L4_judge_i=L4_boundary_i-1;
const int L4_judge_j=L4_boundary_j-1;

void mm5_kernel0_L4(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L4(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L4(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L4(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif