if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./project_setup.sh ./config_files/input.cfg ./fp32_kernel"
    echo "******************************************"
    echo ""
    exit
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
	elif (( ${n} == 3 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		KernelGen="${Value[0]}"; 
	elif (( ${n} == 8 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		AIEArrGen="${Value[0]}";
    elif (( ${n} == 16 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		SysGen="${Value[0]}";
 	fi
done < "$input"

if (( ${SysGen} == 1 )) && ( (( ${KernelGen} != 1 )) || (( ${AIEArrGen} != 1 )) )
then
    echo ""
    echo "******************************************"
    echo "Error: When \"SysGen\" is specified to 1, \"KernelGen\" and \"AIEArrGen\" must also be 1."
    echo "******************************************"
    echo ""
    exit
fi

if (( ${AIEArrGen} == 1 )) && (( ${KernelGen} != 1 ))
then
    echo ""
    echo "******************************************"
    echo "Error: When \"AIEArrGen\" is specified to 1, \"KernelGen\" must also be 1."
    echo "******************************************"
    echo ""
    exit
fi

if [ ${data_type} == "fp32" ]
then
	src_dir="aie_fp32";
elif [ ${data_type} == "int32" ]
then
    src_dir="aie_int32";
elif [ ${data_type} == "int16" ]
then
	src_dir="aie_int16";
fi


if (( ${KernelGen} == 1 ))
then
	./src_gen/Kernel_Gen/gen_gracpp.sh $1 $2;
    ./src_gen/Kernel_Gen/gen_grah.sh $1 $2;
    ./src_gen/Kernel_Gen/gen_para.sh $1 $2;
    cp -r src/${src_dir}/* $2/aie;
    cp -r src/Makefile* $2;
fi

if (( ${AIEArrGen} == 1 ))
then
	./src_gen/AIE_ArrGen/gen_graph.sh $1 $2;
    ./src_gen/AIE_ArrGen/gen_topcpp.sh $1 $2;
    ./src_gen/AIE_ArrGen/gen_toph.sh $1 $2;
fi

if (( ${SysGen} == 1 ))
then
	./src_gen/PL_Gen/gen_conn.sh $1 $2;
    ./src_gen/PL_Gen/gen_dma.sh $1 $2;
    ./src_gen/PL_Gen/gen_packet.sh $1 $2;
    ./src_gen/Host_Gen/gen_host.sh $1 $2;
fi

