#ifndef PARA_L0_H
#define PARA_L0_H
#include <adf/stream/types.h>
const int L0_Byte=1;
const int L0_NUM_KERNEL=4;
const int L0_NUM_KERNELS_PER_PAC=2;
const int L0_NUM_PACK_IN=L0_NUM_KERNEL/L0_NUM_KERNELS_PER_PAC;
const int L0_h1=64;
const int L0_w1=64;
const int L0_w2=64;
const int L0_boundary_i=L0_h1/8;
const int L0_boundary_j=L0_w2/4;
const int L0_boundary_k=L0_w1/16-1;
const int L0_judge_j=L0_boundary_j-1;
const int L0_jumpA0=32;
const int L0_jumpA1=(L0_h1-8)*L0_w1+32;
const int L0_jumpB0=32;
const int L0_jumpB1=(L0_w2-2)*16;
const int L0_jumpB2=(L0_w2+2)*16;

void mm7_kernel0_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm7_kernel1_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm7_kernel2_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm7_kernel3_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif