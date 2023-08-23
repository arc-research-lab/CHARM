#ifndef PARA_L6_H
#define PARA_L6_H
#include <adf/stream/types.h>
const int L6_Byte=2;
const int L6_NUM_KERNEL=3;
const int L6_NUM_KERNELS_PER_PAC=3;
const int L6_NUM_PACK_IN=L6_NUM_KERNEL/L6_NUM_KERNELS_PER_PAC;
const int L6_h1=48;
const int L6_w1=32;
const int L6_w2=48;
const int L6_boundary_i=L6_h1/16;
const int L6_boundary_j=L6_w2/2;
const int L6_boundary_k=L6_w1/16-1;
const int L6_jump_B0=L6_w1-8;
const int L6_jump_A0=L6_h1+16;
const int L6_judge_i=L6_boundary_i-1;
const int L6_judge_j=L6_boundary_j-1;

void mm5_kernel0_L6(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L6(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L6(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L6(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif