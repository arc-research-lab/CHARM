#ifndef PARA_L3_H
#define PARA_L3_H
#include <adf/stream/types.h>
const int L3_Byte=2;
const int L3_NUM_KERNEL=3;
const int L3_NUM_KERNELS_PER_PAC=3;
const int L3_NUM_PACK_IN=L3_NUM_KERNEL/L3_NUM_KERNELS_PER_PAC;
const int L3_h1=48;
const int L3_w1=32;
const int L3_w2=48;
const int L3_boundary_i=L3_h1/16;
const int L3_boundary_j=L3_w2/2;
const int L3_boundary_k=L3_w1/16-1;
const int L3_jump_B0=L3_w1-8;
const int L3_jump_A0=L3_h1+16;
const int L3_judge_i=L3_boundary_i-1;
const int L3_judge_j=L3_boundary_j-1;

void mm5_kernel0_L3(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L3(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L3(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L3(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif