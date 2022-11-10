if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_conn.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

if [ -f ./${dir_name}/conn.cfg ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/conn.cfg exists." 
    echo "******************************************"
    echo ""
    exit;
fi

for ((n=1;n<=21;n++));
do
	read -r line
    if (( ${n} == 9 ))
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
 	fi
done < "$input"

let port_row_in=${A}*${B}*${C}/${NUM_PACK}/${R_BRO};
let port_col_in=${C}*${B}*${A}/${NUM_PACK}/${C_BRO};
let port_out=${A}*${C}/${NUM_PACK};

mkdir -p ${dir_name}/kernel
if [ ${B} == 4 ] || [ ${B} == 3 ]
then
echo \
"
[connectivity]
nk=dma:1:dma_0
">> ./${dir_name}/conn.cfg;

for ((i=0;i<${port_row_in};i++));
do  
    echo \
    "stream_connect = dma_0.txA_${i}:ai_engine_0.in_r${i}">> ./${dir_name}/conn.cfg;
done

for ((i=0;i<${port_col_in};i++));
do  
    echo \
    "stream_connect = dma_0.txB_${i}:ai_engine_0.in_c${i}">> ./${dir_name}/conn.cfg;
done

for ((i=0;i<${port_out};i++));
do  
    echo \
    "stream_connect = ai_engine_0.out_${i}:dma_0.rxC_${i}">> ./${dir_name}/conn.cfg;
done

echo \
"
[vivado]
prop=run.impl_1.STEPS.PLACE_DESIGN.ARGS.DIRECTIVE=EarlyBlockPlacement
prop=run.impl_1.STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE=Explore
prop=run.impl_1.STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE=Explore
">> ./${dir_name}/conn.cfg;

fi