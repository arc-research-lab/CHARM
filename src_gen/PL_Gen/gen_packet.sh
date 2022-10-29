if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_packet.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

for ((n=1;n<=24;n++));
do
	read -r line
	if (( ${n} == 2 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		data_type="${Value[0]}"; 
	elif (( ${n} == 5 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		mm_i="${Value[0]}";
	elif (( ${n} == 6 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		mm_k="${Value[0]}";
	elif (( ${n} == 7 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		mm_j="${Value[0]}";
    elif (( ${n} == 9 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		NUM_PACK="${Value[0]}";
    elif (( ${n} == 10 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		A="${Value[0]}";
    elif (( ${n} == 11 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		B="${Value[0]}";
 	elif (( ${n} == 12 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C="${Value[0]}";
    elif (( ${n} == 13 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		R_BRO="${Value[0]}";
    elif (( ${n} == 14 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C_BRO="${Value[0]}";
	elif (( ${n} == 16 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		X="${Value[0]}";
    elif (( ${n} == 17 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Y="${Value[0]}";
    elif (( ${n} == 18 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Z="${Value[0]}";
 	fi
done < "$input"

mkdir -p ${dir_name}/kernel
echo \
"
#ifndef _PACKET_SENDER_H_
#define _PACKET_SENDER_H_
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
#define H1 ${mm_i}
#define W1 ${mm_k}
#define W2 ${mm_j}
#define A ${A}
#define B ${B}
#define C ${C}
#define X ${X}
#define Y ${Y}
#define Z ${Z}
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
">> ./${dir_name}/kernel/packet_sender.hpp