#ifndef PARA_L0_H
#define PARA_L0_H
#include <adf/stream/types.h>
const int L0_Byte=4;
const int L0_NUM_KERNEL=4;
const int L0_NUM_KERNELS_PER_PAC=4;
const int L0_NUM_PACK_IN=L0_NUM_KERNEL/L0_NUM_KERNELS_PER_PAC;
const int L0_h1=32;
const int L0_w1=32;
const int L0_w2=32;
const int L0_boundary_i=L0_h1/8;
const int L0_boundary_j=L0_w2/2;
const int L0_boundary_k=L0_w1/8-1;
const int L0_jump_B0=L0_w1-4;
const int L0_jump_A0=L0_h1+8;
const int L0_judge_i=L0_boundary_i-1;
const int L0_judge_j=L0_boundary_j-1;

void mm1_kernel0_L0(input_window_float* __restrict matA, input_window_float*  __restrict matB, output_stream_accfloat* __restrict matC);
void mm1_kernel1_L0(input_window_float* __restrict matA, input_window_float*  __restrict matB, input_stream_accfloat* __restrict acc_in, output_stream_accfloat* __restrict matC);
void mm1_kernel2_L0(input_window_float* __restrict matA, input_window_float*  __restrict matB, input_stream_accfloat* __restrict acc_in, output_window_float * __restrict matC);
void mm1_kernel3_L0(input_window_float* __restrict matA, input_window_float*  __restrict matB, output_window_float * __restrict matC);

#endif