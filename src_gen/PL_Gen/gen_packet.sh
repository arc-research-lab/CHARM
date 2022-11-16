if [ "$#" -eq 5 ] 
then
  	input=$1;
    dir_name=$2;
	AXI_WIDTH_A=$3;
	AXI_WIDTH_B=$4;
	AXI_WIDTH_C=$5;
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

if [ -f ./${dir_name}/kernel/packet_sender.hpp ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/kernel/packet_sender.hpp exists." 
    echo "******************************************"
    echo ""
    exit;
fi

for ((n=1;n<=22;n++));
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
		NUM_PACK_IN="${Value[0]}";
	elif (( ${n} == 10 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		NUM_PACK_OUT="${Value[0]}";
    elif (( ${n} == 11 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		A="${Value[0]}";
    elif (( ${n} == 12 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		B="${Value[0]}";
 	elif (( ${n} == 13 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C="${Value[0]}";
    elif (( ${n} == 14 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		R_BRO="${Value[0]}";
    elif (( ${n} == 15 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C_BRO="${Value[0]}";
	elif (( ${n} == 17 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		X="${Value[0]}";
    elif (( ${n} == 18 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Y="${Value[0]}";
    elif (( ${n} == 19 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Z="${Value[0]}";
 	fi
done < "$input"

if [ ${data_type} == "fp32" ] || [ ${data_type} == "int32" ]
then
	DATA_T=32;
elif [ ${data_type} == "int16" ]
then
	DATA_T=16;
elif [ ${data_type} == "int8" ]
then
	DATA_T=8;
fi



mkdir -p ${dir_name}/kernel
echo \
"
#ifndef _PACKET_SENDER_H_
#define _PACKET_SENDER_H_
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define DATA_TYPE ${DATA_T}

#define AXI_WIDTH_A ${AXI_WIDTH_A}
#define AXI_WIDTH_B ${AXI_WIDTH_B}
#define AXI_WIDTH_C ${AXI_WIDTH_C}
#define PLIO_WIDTH 128

#define PKTTYPE 0 
#define PACKET_NUM_IN ${NUM_PACK_IN}
#define PACKET_NUM_OUT ${NUM_PACK_OUT}
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

const int A_PER_TRA=(AXI_WIDTH_A/DATA_TYPE);
const int B_PER_TRA=(AXI_WIDTH_B/DATA_TYPE);
const int C_PER_TRA=(AXI_WIDTH_C/DATA_TYPE);

#define NUM_PER_TRA (PLIO_WIDTH/DATA_TYPE)

#define LEFT_SIZE (H1*W1/NUM_PER_TRA)
#define RIGHT_SIZE (W1*W2/NUM_PER_TRA)
#define OUT_SIZE (H1*W2/NUM_PER_TRA)

typedef ap_uint<PLIO_WIDTH> data_t;
typedef ap_axiu<PLIO_WIDTH, 0, 0, 0> axis_pkt;
typedef hls::stream<axis_pkt> axis_stream;

typedef hls::stream<ap_uint<32>> axis_stream_32;
typedef hls::stream<ap_uint<AXI_WIDTH_A>> axis_stream_A;
typedef hls::stream<ap_uint<AXI_WIDTH_B>> axis_stream_B;
typedef hls::stream<ap_uint<AXI_WIDTH_C>> axis_stream_C;

const int boundA=M*K/A_PER_TRA;
const int boundB=K*N/B_PER_TRA;
const int boundC=M*N/C_PER_TRA;">> ./${dir_name}/kernel/packet_sender.hpp

if [ ${data_type} == "fp32" ]
then
echo \
"
typedef union{
    float data_cbuff;
    unsigned int uintval;
} fp_int;">> ./${dir_name}/kernel/packet_sender.hpp

elif [ ${data_type} == "int16" ]
then
echo \
"
typedef struct{
   unsigned short int low;
   unsigned short int high;
} comb_32;
">> ./${dir_name}/kernel/packet_sender.hpp

elif [ ${data_type} == "int8" ]
then
echo \
"
typedef struct{
   ap_unit<8> low0;
   ap_unit<8> low1;
   ap_unit<8> high0;
   ap_unit<8> high1;
} comb_32;
">> ./${dir_name}/kernel/packet_sender.hpp
fi

echo \
"
#endif
">> ./${dir_name}/kernel/packet_sender.hpp