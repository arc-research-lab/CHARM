#ifndef PARA_L6_H
#define PARA_L6_H
#include <adf/stream/types.h>
const int L6_Byte=1;
const int L6_NUM_KERNEL=2;
const int L6_NUM_KERNELS_PER_PAC=2;
const int L6_NUM_PACK_IN=L6_NUM_KERNEL/L6_NUM_KERNELS_PER_PAC;
const int L6_h1=64;
const int L6_w1=64;
const int L6_w2=64;
const int L6_boundary_i=L6_h1/8;
const int L6_boundary_j=L6_w2/4;
const int L6_boundary_k=L6_w1/16-1;
const int L6_judge_j=L6_boundary_j-1;
const int L6_jumpA0=32;
const int L6_jumpA1=(L6_h1-8)*L6_w1+32;
const int L6_jumpB0=32;
const int L6_jumpB1=(L6_w2-2)*16;
const int L6_jumpB2=(L6_w2+2)*16;

void mm7_kernel0_L6(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm7_kernel1_L6(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm7_kernel2_L6(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm7_kernel3_L6(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif