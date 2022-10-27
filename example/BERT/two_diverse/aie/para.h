#ifndef __PARA_H__
#define __PARA_H__

#include <adf/stream/types.h>

#define h1 32
#define w1 32
#define w2 32
const int boundary_i=h1/8;
const int boundary_j=w2/2;
const int boundary_k=w1/8-1;
const int judge_i=boundary_i-1;
const int judge_j=boundary_j-1;
void mm_kernel0(input_window_float* matA, input_window_float* matB, output_window_float* matC);
void mm_kernel1(input_window_float* matA, input_window_float* matB, input_window_float* acc_in, output_window_float* matC);
#endif


#define NUM_ENGINES_PER_PAC_LARGE 4
#define NUM_ENGINES_PER_PAC_SMALL 2

