#ifndef PARA_L7_H
#define PARA_L7_H
#include <adf/stream/types.h>
const int L7_Byte=2;
const int L7_NUM_KERNEL=3;
const int L7_NUM_KERNELS_PER_PAC=3;
const int L7_NUM_PACK_IN=L7_NUM_KERNEL/L7_NUM_KERNELS_PER_PAC;
const int L7_h1=48;
const int L7_w1=32;
const int L7_w2=48;
const int L7_boundary_i=L7_h1/16;
const int L7_boundary_j=L7_w2/2;
const int L7_boundary_k=L7_w1/16-1;
const int L7_jump_B0=L7_w1-8;
const int L7_jump_A0=L7_h1+16;
const int L7_judge_i=L7_boundary_i-1;
const int L7_judge_j=L7_boundary_j-1;

void mm5_kernel0_L7(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_stream_acc48* __restrict matC);
void mm5_kernel1_L7(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm5_kernel2_L7(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int16 * __restrict matC);
void mm5_kernel3_L7(input_window_int16* __restrict matA, input_window_int16*  __restrict matB, output_window_int16 * __restrict matC);

#endif