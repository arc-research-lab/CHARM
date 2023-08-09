#ifndef PARA_L{{layer}}_H
#define PARA_L{{layer}}_H
#include <adf/stream/types.h>
const int L{{layer}}_Byte={{ data_type }};
const int L{{layer}}_NUM_KERNEL={{B}};
const int L{{layer}}_NUM_KERNELS_PER_PAC={{PACK_IN}};
const int L{{layer}}_NUM_PACK_IN=L{{layer}}_NUM_KERNEL/L{{layer}}_NUM_KERNELS_PER_PAC;
const int L{{layer}}_h1={{ h1 }};
const int L{{layer}}_w1={{ w1 }};
const int L{{layer}}_w2={{ w2 }};
const int L{{layer}}_boundary_i=L{{layer}}_h1/8;
const int L{{layer}}_boundary_j=L{{layer}}_w2/2;
const int L{{layer}}_boundary_k=L{{layer}}_w1/8-1;
const int L{{layer}}_jump_B0=L{{layer}}_w1-4;
const int L{{layer}}_jump_A0=L{{layer}}_h1+8;
const int L{{layer}}_judge_i=L{{layer}}_boundary_i-1;
const int L{{layer}}_judge_j=L{{layer}}_boundary_j-1;

//matA: [W1][H1]  matB [W2][W1] matC [W2/2][H1/8][2][8]

void mm1_kernel0_L{{layer}}(input_window_float* __restrict matA, input_window_float*  __restrict matB, output_stream_accfloat* __restrict matC);
void mm1_kernel1_L{{layer}}(input_window_float* __restrict matA, input_window_float*  __restrict matB, input_stream_accfloat* __restrict acc_in, output_stream_accfloat* __restrict matC);
void mm1_kernel2_L{{layer}}(input_window_float* __restrict matA, input_window_float*  __restrict matB, input_stream_accfloat* __restrict acc_in, output_window_float * __restrict matC);

#endif