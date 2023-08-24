#ifndef PARA_L5_H
#define PARA_L5_H
#include <adf/stream/types.h>
const int L5_Byte=1;
const int L5_NUM_KERNEL=2;
const int L5_NUM_KERNELS_PER_PAC=2;
const int L5_NUM_PACK_IN=L5_NUM_KERNEL/L5_NUM_KERNELS_PER_PAC;
const int L5_h1=64;
const int L5_w1=64;
const int L5_w2=64;
const int L5_boundary_i=L5_h1/8;
const int L5_boundary_j=L5_w2/4;
const int L5_boundary_k=L5_w1/16-1;
const int L5_judge_j=L5_boundary_j-1;
const int L5_jumpA0=32;
const int L5_jumpA1=(L5_h1-8)*L5_w1+32;
const int L5_jumpB0=32;
const int L5_jumpB1=(L5_w2-2)*16;
const int L5_jumpB2=(L5_w2+2)*16;

void mm7_kernel0_L5(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm7_kernel1_L5(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm7_kernel2_L5(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm7_kernel3_L5(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif