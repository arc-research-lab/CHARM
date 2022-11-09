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
    elif (( ${n} == 19 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        l_buff="${Value[0]}";
    elif (( ${n} == 20 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        r_buff="${Value[0]}";
    elif (( ${n} == 21 ))
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
let port_row_in=${A}*${B}*${C}/${NUM_PACK}/${R_BRO};
let port_col_in=${C}*${B}*${A}/${NUM_PACK}/${C_BRO};
let port_out=${A}*${C}/${NUM_PACK};
let pipe_length=${X}*${Y}*${Z};
let array_size=${pipe_length}*${NUM_PACK};


mkdir -p ${dir_name}/kernel


if [ ${B} == 4 ] || [ ${B} == 3 ]
then
echo \
"
#include <stdint.h>
#include \"packet_sender.hpp\"
">> ./${dir_name}/kernel/dma.cpp;

./src_gen/PL_Gen/gen_header.sh ${dir_name} ${NUM_PACK} ${pipe_length} ${array_size} ${X} ${Y} ${Z};

./src_gen/PL_Gen/gen_address.sh ${dir_name};

./src_gen/PL_Gen/gen_ld_st.sh ${dir_name} ${data_type};

./src_gen/PL_Gen/gen_reshapec.sh ${dir_name} ${data_type};

./src_gen/PL_Gen/gen_senda.sh ${dir_name} ${data_type} ${NUM_TXA};

./src_gen/PL_Gen/gen_sendb.sh ${dir_name} ${data_type} ${NUM_TXB};

./src_gen/PL_Gen/gen_compute.sh ${dir_name} ${port_row_in} ${port_col_in} ${port_out} ${l_buff} ${r_buff} ${o_buff} ${NUM_TXA} ${NUM_TXB} ${A} ${C};

./src_gen/PL_Gen/gen_top.sh ${dir_name} ${port_row_in} ${port_col_in} ${port_out};

fi