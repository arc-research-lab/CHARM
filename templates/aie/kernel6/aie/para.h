#ifndef PARA_L{{layer}}_H
#define PARA_L{{layer}}_H
#include <adf/stream/types.h>
const int L{{layer}}_Byte={{ data_type }};
const int L{{layer}}_NUM_KERNEL={{B}};
const int L{{layer}}_h1={{ h1 }};
const int L{{layer}}_w1={{ w1 }};
const int L{{layer}}_w2={{ w2 }};
const int L{{layer}}_boundary_i=L{{layer}}_h1/8;
const int L{{layer}}_boundary_j=L{{layer}}_w2/4;
const int L{{layer}}_boundary_k=L{{layer}}_w1/16-1;
const int L{{layer}}_judge_j=L{{layer}}_boundary_j-1;
const int L{{layer}}_jumpA0=32;
const int L{{layer}}_jumpA1=(L{{layer}}_h1-8)*L{{layer}}_w1+32;
const int L{{layer}}_jumpB0=32;
const int L{{layer}}_jumpB1=(L{{layer}}_w2-2)*16;
const int L{{layer}}_jumpB2=(L{{layer}}_w2+2)*16;

void mm6_kernel0_L{{layer}}(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm6_kernel1_L{{layer}}(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm6_kernel2_L{{layer}}(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_window_int8 * __restrict matC);
void mm6_kernel3_L{{layer}}(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_window_int8 * __restrict matC);

#endif