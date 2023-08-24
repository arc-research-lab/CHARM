#ifndef PARA_L7_H
#define PARA_L7_H
#include <adf/stream/types.h>
const int L7_Byte=1;
const int L7_NUM_KERNEL=2;
const int L7_NUM_KERNELS_PER_PAC=2;
const int L7_NUM_PACK_IN=L7_NUM_KERNEL/L7_NUM_KERNELS_PER_PAC;
const int L7_h1=64;
const int L7_w1=64;
const int L7_w2=64;
const int L7_boundary_i=L7_h1/8;
const int L7_boundary_j=L7_w2/4;
const int L7_boundary_k=L7_w1/16-1;
const int L7_judge_j=L7_boundary_j-1;
const int L7_jumpA0=32;
const int L7_jumpA1=(L7_h1-8)*L7_w1+32;
const int L7_jumpB0=32;
const int L7_jumpB1=(L7_w2-2)*16;
const int L7_jumpB2=(L7_w2+2)*16;

void mm7_kernel0_L7(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm7_kernel1_L7(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm7_kernel2_L7(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm7_kernel3_L7(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif