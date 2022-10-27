
#ifndef _PACKET_SENDER_SMALL_H_
#define _PACKET_SENDER_SMALL_H_
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

const int AXI_WIDTH_512=512;
const int AXI_WIDTH_256=256;
const int PLIO_WIDTH=128;
const int DATA_TYPE=32;
const int PKTTYPE=0; 
const int PACKET_NUM=2;
const int PACKET_NUM_O=4;
const int H1=32;
const int W1=32;
const int W2=32;
const int A=8;
const int B=2;
const int C=2;
const int X=2;
const int Y=1;
const int Z=1;
const int M=(H1*A*X);
const int K=(W1*B*Y);
const int N=(W2*C*Z);
const int A_PER_TRA=AXI_WIDTH_512/DATA_TYPE;
const int C_PER_TRA=AXI_WIDTH_256/DATA_TYPE;
const int NUM_PER_TRA=PLIO_WIDTH/DATA_TYPE;
const int LEFT_SIZE=H1*W1/NUM_PER_TRA;
const int RIGHT_SIZE=W1*W2/NUM_PER_TRA;
const int OUT_SIZE=H1*W2/NUM_PER_TRA;   //256

typedef ap_uint<PLIO_WIDTH> data_t;
typedef ap_axiu<PLIO_WIDTH, 0, 0, 0> axis_pkt;
typedef hls::stream<axis_pkt> axis_stream;


// typedef qdma_axis<32, 0, 0, 0> axis_pkt_32;
// typedef hls::stream<axis_pkt_32> axis_stream_32;
// typedef qdma_axis<64, 0, 0, 0> axis_pkt_64;
// typedef hls::stream<axis_pkt_64> axis_stream_64;
// typedef qdma_axis<256, 0, 0, 0> axis_pkt_256;
// typedef hls::stream<axis_pkt_256> axis_stream_256;
// typedef qdma_axis<512, 0, 0, 0> axis_pkt_512;
// typedef hls::stream<axis_pkt_512> axis_stream_512;

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


