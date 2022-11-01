if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
	echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_para.sh ../../config_files/input.cfg ."
    echo "******************************************"
	echo ""
    exit
fi

if [ -f ./${dir_name}/aie/para.h ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/aie/para.h exists." 
    echo "******************************************"
    echo ""
    exit;
fi

for ((n=1;n<=21;n++));
do
	read -r line
	if (( ${n} == 2 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		data_type="${Value[0]}"; 
	elif (( ${n} == 4 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		kernel_type="${Value[0]}";
	elif (( ${n} == 5 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		i="${Value[0]}";
	elif (( ${n} == 6 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		k="${Value[0]}";
	elif (( ${n} == 7 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		j="${Value[0]}";
	elif (( ${n} == 8 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		AIE_ArrGen="${Value[0]}";
 	elif (( ${n} == 15 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Sys_Gen="${Value[0]}";
 	fi
done < "$input"

if [ ${data_type} == "int32" ] || [ ${data_type} == "fp32" ]
then
	let size=${i}*${j}+${j}*${k}+${i}*${k};
	if [ ${size} -gt 4096 ]
	then
		echo ""
    	echo "******************************************"
		echo "
		${size}=i*k+k*j+i*j should not be more than 4096 for int32 and fp32
		";
		echo "******************************************"
    	echo ""
	fi
fi

if (( ${AIE_ArrGen} == 1 )) || (( ${Sys_Gen} == 1 ))
then
	kernel_type=1;
	if [ ${data_type} == "fp32" ] || [ ${data_type} == "int32" ]
	then
		i=32;
		j=32;
		k=32;
	elif [ ${data_type} == "int16" ]
	then
		i=48;
		j=48;
		k=48;
	fi
fi

mkdir -p ${dir_name}/aie
if [ ${data_type} == "int32" ]
then
echo \
"#ifndef __PARA_H__
#define __PARA_H__

#include <adf/stream/types.h>

#define h1 ${i}
#define w1 ${k}
#define w2 ${j}
const int boundary_i=h1/8;
const int boundary_j=w2/2;
const int boundary_k=w1/8-1;
const int judge_i=boundary_i-1;
const int judge_j=boundary_j-1;

void mm_kernel0(input_window_int32* matA, input_window_int32* matB, output_window_int32* matC);
">> ./${dir_name}/aie/para.h;

if [ ${kernel_type} == 1 ] 
then
echo \
"void mm_kernel1(input_window_int32* matA, input_window_int32* matB, input_window_int32* acc_in, output_window_int32* matC);
">> ./${dir_name}/aie/para.h;
fi

echo \
"#endif
">> ./${dir_name}/aie/para.h;


elif [ ${data_type} == "fp32" ]
then
echo \
"#ifndef __PARA_H__
#define __PARA_H__

#include <adf/stream/types.h>

#define h1 ${i}
#define w1 ${k}
#define w2 ${j}
const int boundary_i=h1/8;
const int boundary_j=w2/2;
const int boundary_k=w1/8-1;
const int judge_i=boundary_i-1;
const int judge_j=boundary_j-1;

void mm_kernel0(input_window_float* matA, input_window_float* matB, output_window_float* matC);
">> ./${dir_name}/aie/para.h;

if [ ${kernel_type} == 1 ] 
then
echo \
"void mm_kernel1(input_window_float* matA, input_window_float* matB, input_window_float* acc_in, output_window_float* matC);
">> ./${dir_name}/aie/para.h;
fi

echo \
"#endif
">> ./${dir_name}/aie/para.h;

elif [ ${data_type} == "int16" ]
then
echo \
"#ifndef __PARA_H__
#define __PARA_H__

#include <adf/stream/types.h>

#define h1 ${i}
#define w1 ${k}
#define w2 ${j}
const int boundary_i=h1/16;
const int boundary_j=w2/2;
const int boundary_k=w1/16-1;
const int judge_i=boundary_i-1;
const int judge_j=boundary_j-1;

void mm_kernel0(input_window_int16* matA, input_window_int16* matB, output_window_int16* matC);
">> ./${dir_name}/aie/para.h;

if [ ${kernel_type} == 1 ] 
then
echo \
"void mm_kernel1(input_window_int16* matA, input_window_int16* matB, input_window_int16* acc_in, output_window_int16* matC);
">> ./${dir_name}/aie/para.h;
fi

echo \
"#endif
">> ./${dir_name}/aie/para.h;

fi
