#ifndef _PACKET_SENDER_LARGE_H_
#define _PACKET_SENDER_LARGE_H_
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#define AXI_WIDTH_512 512
#define AXI_WIDTH_256 256
#define PLIO_WIDTH 128
#define DATA_TYPE 32
#define PKTTYPE 0 
#define PACKET_NUM 4
#define H1 32
#define W1 32
#define W2 32
#define A 4
#define B 4
#define C 16
#define X 4
#define Y 2
#define Z 1
#define M (H1*A*X)
#define K (W1*B*Y)
#define N (W2*C*Z)

#define A_PER_TRA (AXI_WIDTH_512/DATA_TYPE)
#define C_PER_TRA (AXI_WIDTH_256/DATA_TYPE)
#define NUM_PER_TRA (PLIO_WIDTH/DATA_TYPE)

#define LEFT_SIZE (H1*W1/NUM_PER_TRA)
#define RIGHT_SIZE (W1*W2/NUM_PER_TRA)
#define OUT_SIZE (H1*W2/NUM_PER_TRA)

typedef ap_uint<PLIO_WIDTH> data_t;
typedef ap_axiu<PLIO_WIDTH, 0, 0, 0> axis_pkt;
typedef hls::stream<axis_pkt> axis_stream;

typedef hls::stream<ap_uint<32>> axis_stream_32;
typedef hls::stream<ap_uint<64>> axis_stream_64;
typedef hls::stream<ap_uint<256>> axis_stream_256;
typedef hls::stream<ap_uint<512>> axis_stream_512;

const int boundA=M*K/A_PER_TRA;
const int boundB=K*N/A_PER_TRA;
const int boundC=M*N/C_PER_TRA;

typedef union{
    float data_cbuff;
    unsigned int uintval;
} fp_int;

#endif