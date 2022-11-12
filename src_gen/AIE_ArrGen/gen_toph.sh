if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_toph.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
	echo "AIE_ArrGen/gen_toph"
    exit
fi

if [ -f ./${dir_name}/aie/mm_top.h ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/aie/mm_top.h exists." 
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
 	fi
done < "$input"

let AIE_NUM=${A}*${B}*${C};
if [ ${AIE_NUM} -gt 400 ]
then
	echo ""
	echo "******************************************"
	echo "
	${AIE_NUM}=A * B * C is the number of AIEs used in the design and should be less than 400
	";
	echo "******************************************"
	echo ""
	echo "AIE_ArrGen/gen_toph"
	exit
fi

let NUM_INSTANCES=${A}*${C};

if [ ${data_type} == "fp32" ] || [ ${kernel_type} == "int32" ]
then
	BPE=4;
elif [ ${data_type} == "int16" ]
then
	BPE=2;
fi

mkdir -p ${dir_name}/aie
if [ ${B} == 4 ] 
then
echo \
"
#include \"mm_graph_x${B}.h\"
const int ROW=${A};
const int COL=${C};
const int R_BRO=${R_BRO};
const int C_BRO=${C_BRO};
const int NUM_PACKET_PAC=NUM_ENGINES_PER_PAC/${NUM_PACK_IN};    //number of packet in each graph
const int NUM_INSTANCES=ROW*COL;
const int NUM_OUT_PACK=NUM_INSTANCES/${NUM_PACK_OUT};
using namespace adf;
template <int COL_OFFSET,int ROW_OFFSET>
class mm_x${NUM_INSTANCES}_x${B}_graph : public adf::graph {
public:
	input_port in_row[ROW*NUM_PACKET_PAC*COL/R_BRO];
	input_port in_col[COL*NUM_PACKET_PAC*ROW/C_BRO];
    adf::pktmerge<${NUM_PACK_OUT}>  mg_out[NUM_OUT_PACK];
	output_port out[NUM_OUT_PACK];
">> ./${dir_name}/aie/mm_top.h;

for ((a=0;a<${A};a++));
do 
    for ((c=0;c<${C};c++));
    do  
        let aie=${a}*${C}+${c};
        let div=(${NUM_INSTANCES}/2);
        let layout=${aie}/${div};
		let temp0=${aie}/${C};
		let temp1=$(((${A}+1)/2));
		let pos_row=$(((${temp0}%${temp1})*2))+${temp0}/${temp1};
        if [ ${layout} == 0 ] 
        then
            echo \
            "	mm_x${B}_graph<COL_OFFSET+${aie}, ROW_OFFSET+0>  mm_x${B}_${c}_${pos_row}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
        else
            let aie1=${aie}-${div};
            echo \
            "	mm_x${B}_graph<COL_OFFSET+${aie1}, ROW_OFFSET+4>  mm_x${B}_${c}_${pos_row}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
        fi
    done
done

echo \
"
	mm_x${NUM_INSTANCES}_x${B}_graph() {
		for (int i =0; i<NUM_OUT_PACK; i++)  {
			mg_out[i] = adf::pktmerge<${NUM_PACK_OUT}>::create();
		}
">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${A};i++));
do
    for ((j=0;j<${C};j++));
    do  
        let row=${C}/${R_BRO}*${i}+${j}/${R_BRO};
        echo \
        "	    connect< stream, window< h1*w1*${BPE} > >(in_row[${row}], mm_x${B}_${j}_${i}.in[0]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo "">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${C};i++));
do
    for ((j=0;j<${A};j++));
    do  
        let col=${A}/${C_BRO}*${i}+${j}/${C_BRO};
        echo \
        "	    connect< stream, window< w1*w2*${BPE} > >(in_col[${col}], mm_x${B}_${i}_${j}.in[1]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo "">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${A};i++));
do
    for ((j=0;j<${C};j++));
    do  
        let aie=${i}*${C}+${j};
        let out=${aie}/${NUM_PACK_OUT};
        let port=${aie}%${NUM_PACK_OUT};
        echo \
        "	    connect<adf::window<h1*w2*${BPE}>, adf::pktstream > (mm_x${B}_${j}_${i}.out, mg_out[${out}].in[${port}]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo \
"
	    for (int i=0; i<NUM_OUT_PACK; i++)  {
	    	adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
	    }
    }
};
">> ./${dir_name}/aie/mm_top.h;

elif [ ${B} == 3 ]
then

if [ ${AIE_NUM} -gt 300 ]
then
echo \
"#include \"mm_graph_x${B}_type0.h\"
#include \"mm_graph_x${B}_type1.h\"
#include \"mm_graph_x${B}.h\"
">> ./${dir_name}/aie/mm_top.h;
else
echo \
"#include \"mm_graph_x${B}.h\"
">> ./${dir_name}/aie/mm_top.h;
fi

echo \
"
const int ROW=${A};
const int COL=${C};
const int R_BRO=${R_BRO};
const int C_BRO=${C_BRO};
const int NUM_PACKET_PAC=NUM_ENGINES_PER_PAC/${NUM_PACK_IN};    //number of packet in each graph
const int NUM_INSTANCES=ROW*COL;
const int NUM_OUT_PACK=NUM_INSTANCES/${NUM_PACK_OUT};
using namespace adf;

template <int COL_OFFSET,int ROW_OFFSET>
class mm_x${NUM_INSTANCES}_x${B}_graph : public adf::graph {

public:
	input_port in_row[ROW*NUM_PACKET_PAC*COL/R_BRO];
	input_port in_col[COL*NUM_PACKET_PAC*ROW/C_BRO];

    adf::pktmerge<${NUM_PACK_OUT}>  mg_out[NUM_OUT_PACK];
	output_port out[NUM_OUT_PACK];
">> ./${dir_name}/aie/mm_top.h;

if [ ${AIE_NUM} -gt 300 ]
then
	if [ ${C} -gt 16 ]
	then
		echo ""
		echo "******************************************"
		echo "When B=3 and the number of AIE is greater than 300, C should be less than or equal to 16";
		echo "******************************************"
		echo ""
		echo "AIE_ArrGen/gen_toph"
		exit
	else
		let num_block_origin=50/${C};
		let col_origin=${num_block_origin}*${C};
		let num_col_new=$(((${AIE_NUM}-${col_origin}*${B}*2)/2));
		let start_new=48-${num_col_new};
		let new_graph=${A}-${num_block_origin}*2;
		 
		for ((row=0;row<2;row++));
		do
			for ((col=0;col<${col_origin};col++));
	    	do	
				let c=${col}%${C};
				let a=$((${col}/${C}*2+${row}));
				let row_pos=${row}*${B};
				echo \
	    	        "	mm_x${B}_graph<COL_OFFSET+${col}, ROW_OFFSET+${row_pos}>  mm_x${B}_${c}_${a}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
			done
		done

		let a_start=${num_block_origin}*2;
		for ((a=${a_start};a<${a_start}+${new_graph};a++));
		do	
			for ((col=0;col<${C};col++));
	    	do	
				let temp0=${col}%2;
				let temp1=${col}/2;
				let real_a=${a}-${a_start};
				let col_pos=${start_new}+${temp1}*3+${temp0}*2+${num_col_new}/${new_graph}*${real_a};
				let row_pos=2*${B};
				if [ ${temp0} == 0 ]
				then
					echo \
	    	        	"	mm_x${B}_graph_type0<COL_OFFSET+${col_pos}, ROW_OFFSET+${row_pos}>  mm_x${B}_${col}_${a}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
				else
					echo \
	    	        	"	mm_x${B}_graph_type1<COL_OFFSET+${col_pos}, ROW_OFFSET+${row_pos}>  mm_x${B}_${col}_${a}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
				fi
			done
		done
	fi
# Row 7 : AIE <-- AIE <--
# Row 6 : --> AIE --> AIE
#Following code is a puzzle solution
else
	for ((a=0;a<${A};a++));
	do 
	    for ((c=0;c<${C};c++));
	    do  
	        let aie=${a}*${C}+${c};
	        let div=(${NUM_INSTANCES}/2);
	        let layout=${aie}/${div};
			let temp0=${aie}/${C};
			let temp1=$(((${A}+1)/2));
			let pos_row=$(((${temp0}%${temp1})*2))+${temp0}/${temp1};
	        if [ ${layout} == 0 ]
	        then

	            echo \
	            "	mm_x${B}_graph<COL_OFFSET+${aie}, ROW_OFFSET+0>  mm_x${B}_${c}_${pos_row}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
	        else
	            let aie1=${aie}-${div};
	            echo \
	            "	mm_x${B}_graph<COL_OFFSET+${aie1}, ROW_OFFSET+4>  mm_x${B}_${c}_${pos_row}; //(col,row)">> ./${dir_name}/aie/mm_top.h;
	        fi
	    done
	done
fi

echo \
"
	mm_x${NUM_INSTANCES}_x${B}_graph() {
		for (int i =0; i<NUM_OUT_PACK; i++)  {
			mg_out[i] = adf::pktmerge<${NUM_PACK_OUT}>::create();
		}
">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${A};i++));
do
    for ((j=0;j<${C};j++));
    do  
        let row=${C}/${R_BRO}*${i}+${j}/${R_BRO};
        echo \
        "	    connect< stream, window< h1*w1*${BPE} > >(in_row[${row}], mm_x${B}_${j}_${i}.in[0]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo "">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${C};i++));
do
    for ((j=0;j<${A};j++));
    do  
        let col=${A}/${C_BRO}*${i}+${j}/${C_BRO};
        echo \
        "	    connect< stream, window< w1*w2*${BPE} > >(in_col[${col}], mm_x${B}_${i}_${j}.in[1]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo "">> ./${dir_name}/aie/mm_top.h;

for ((i=0;i<${A};i++));
do
    for ((j=0;j<${C};j++));
    do  
        let aie=${i}*${C}+${j};
        let out=${aie}/${NUM_PACK_OUT};
        let port=${aie}%${NUM_PACK_OUT};
        echo \
        "	    connect<adf::window<h1*w2*${BPE}>, adf::pktstream > (mm_x${B}_${j}_${i}.out, mg_out[${out}].in[${port}]);">> ./${dir_name}/aie/mm_top.h;
    done
done

echo \
"
	    for (int i=0; i<NUM_OUT_PACK; i++)  {
	    	adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
	    }
    }
};
">> ./${dir_name}/aie/mm_top.h;

fi