#ifndef PARA_L1_H
#define PARA_L1_H
#include <adf/stream/types.h>
const int L1_Byte=1;
const int L1_NUM_KERNEL=4;
const int L1_NUM_KERNELS_PER_PAC=2;
const int L1_NUM_PACK_IN=L1_NUM_KERNEL/L1_NUM_KERNELS_PER_PAC;
const int L1_h1=64;
const int L1_w1=64;
const int L1_w2=64;
const int L1_boundary_i=L1_h1/8;
const int L1_boundary_j=L1_w2/4;
const int L1_boundary_k=L1_w1/16-1;
const int L1_judge_j=L1_boundary_j-1;
const int L1_jumpA0=32;
const int L1_jumpA1=(L1_h1-8)*L1_w1+32;
const int L1_jumpB0=32;
const int L1_jumpB1=(L1_w2-2)*16;
const int L1_jumpB2=(L1_w2+2)*16;

void mm7_kernel0_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm7_kernel1_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm7_kernel2_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm7_kernel3_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif