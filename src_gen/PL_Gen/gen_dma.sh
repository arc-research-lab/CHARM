if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_dma.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

if [ -f ./${dir_name}/kernel/dma.cpp ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/kernel/dma.cpp exists." 
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
	elif (( ${n} == 4 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		kernel_type="${Value[0]}";
	elif (( ${n} == 6 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		mm_k="${Value[0]}";
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
    elif (( ${n} == 20 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        l_buff="${Value[0]}";
    elif (( ${n} == 21 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        r_buff="${Value[0]}";
    elif (( ${n} == 22 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        o_buff="${Value[0]}";
 	fi
done < "$input"

let NUM_TXA=${C}/${R_BRO};
let NUM_TXB=${A}/${C_BRO};
let port_row_in=${A}*${B}*${C}/${NUM_PACK_IN}/${R_BRO};
let port_col_in=${C}*${B}*${A}/${NUM_PACK_IN}/${C_BRO};
let port_out=${A}*${C}/${NUM_PACK_OUT};
let pipe_length=${X}*${Y}*${Z};
let array_size=${pipe_length}*${NUM_PACK_IN};


mkdir -p ${dir_name}/kernel

if [ ${data_type} == "fp32" ] || [ ${data_type} == "int32" ]
then
	AXI_WIDTH_A=512;
	AXI_WIDTH_B=512;
	AXI_WIDTH_C=512;
	BUFF_WIDTH=128;
elif [ ${data_type} == "int16" ]
then
	BUFF_WIDTH=128;
	if [ ${mm_k} == 32 ]
	then
		AXI_WIDTH_A=512;
		AXI_WIDTH_B=512;
		AXI_WIDTH_C=512;
	else
		if [ $((${A}%2)) == 0 ]
		then
			AXI_WIDTH_A=512;
		else
			AXI_WIDTH_A=256;
		fi

		if [ $((${NUM_PACK_IN}%2)) == 0 ]
		then
			AXI_WIDTH_B=512;
		else
			AXI_WIDTH_B=256;
		fi

		if [[( $((${A}%2)) == 0 && $((${NUM_PACK_IN}%2)) != 0 ) || ( $((${A}%2)) != 0 && $((${NUM_PACK_IN}%2)) == 0 )]] && [ ${C} -ge ${NUM_PACK_OUT} ]
		then
			AXI_WIDTH_C=512;
		else
			AXI_WIDTH_C=256;
		fi
	fi
elif [ ${data_type} == "int8" ]
then
	AXI_WIDTH_A=512;
	AXI_WIDTH_B=512;
	AXI_WIDTH_C=512;
	BUFF_WIDTH=64;
fi

if [ ${B} == 4 ] || [ ${B} == 3 ]
then
echo \
"
#include <stdint.h>
#include \"packet_sender.hpp\"
">> ./${dir_name}/kernel/dma.cpp;
./src_gen/PL_Gen/gen_packet.sh $1 $2 ${AXI_WIDTH_A} ${AXI_WIDTH_B} ${AXI_WIDTH_C} ${BUFF_WIDTH};

./src_gen/PL_Gen/gen_header.sh ${dir_name} ${NUM_PACK_IN} ${pipe_length} ${array_size} ${X} ${Y} ${Z};

./src_gen/PL_Gen/gen_address.sh ${dir_name};

./src_gen/PL_Gen/gen_ld_st.sh ${dir_name} ${data_type} ${AXI_WIDTH_A} ${AXI_WIDTH_B} ${AXI_WIDTH_C} ${mm_k} ${NUM_PACK_IN};

./src_gen/PL_Gen/gen_reshapec.sh ${dir_name} ${data_type} ${NUM_PACK_OUT};

./src_gen/PL_Gen/gen_senda.sh ${dir_name} ${data_type} ${NUM_TXA};

./src_gen/PL_Gen/gen_sendb.sh ${dir_name} ${data_type} ${NUM_TXB} ${AXI_WIDTH_B} ${mm_k};

./src_gen/PL_Gen/gen_compute.sh ${dir_name} ${port_row_in} ${port_col_in} ${port_out} ${l_buff} ${r_buff} ${o_buff} ${NUM_TXA} ${NUM_TXB} ${A} ${C} ${AXI_WIDTH_A} ${AXI_WIDTH_B} ${AXI_WIDTH_C} ${BUFF_WIDTH} ${data_type};

./src_gen/PL_Gen/gen_top.sh ${dir_name} ${port_row_in} ${port_col_in} ${port_out};

fi