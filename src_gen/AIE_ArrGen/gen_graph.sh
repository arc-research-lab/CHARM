if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_graph.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
	echo "AIE_ArrGen/gen_graph"
    exit
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
    elif (( ${n} == 9 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		NUM_PACK_IN="${Value[0]}";
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

if [ -f ./${dir_name}/aie/mm_graph_x${B}.h ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/aie/mm_graph_x${B}.h exists." 
    echo "******************************************"
    echo ""
    exit;
fi

if [ ${data_type} == "fp32" ] || [ ${kernel_type} == "int32" ]
then
	BPE=4;
elif [ ${data_type} == "int16" ]
then
	BPE=2;
fi

if [ ${B} != 3 ] && [ ${B} != 4 ] && [ ${B} != 8 ]
then
    echo ""
    echo "******************************************"
    echo "Currently, CHARM supports B=3 or B=4 or B=8"
    echo "******************************************"
    echo ""
	echo "AIE_ArrGen/gen_graph"
    exit
fi

let PACK_PER_B=${B}/${NUM_PACK_IN};

mkdir -p ${dir_name}/aie

if [ ${PACK_PER_B} == 1 ]
then
echo \
"
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <adf.h>
#include \"para.h\"
using namespace adf;
#define NUM_ENGINES_PER_PAC ${B}

template <int COL_OFFSET, int ROW_OFFSET>
class mm_x${B}_graph : public adf::graph {
private:
	adf::kernel mm_x${B} [NUM_ENGINES_PER_PAC];
	adf::pktsplit<${NUM_PACK_IN}>  sp_a0;
	adf::pktsplit<${NUM_PACK_IN}>  sp_b0;

public:
	adf::port<input>  in[2];
  	adf::port<output>  out;

	mm_x${B}_graph() {
    
		// packet stream to different engines
		sp_a0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		adf::connect< adf::pktstream > (in[0], sp_a0.in[0]);
		adf::connect< adf::pktstream > (in[1], sp_b0.in[0]);

		// create NUM_ENGINES_PER_COL get_particles_i and n-body kernels
		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			if(row==0){
				mm_x${B}[row]   = adf::kernel::create(mm_kernel0);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel0.cc\";
			}
			else{
				mm_x${B}[row]   = adf::kernel::create(mm_kernel1);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel1.cc\";
			}
		}
		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			adf::runtime<ratio>(mm_x${B}[row]) = 1;
			adf::location<kernel>(mm_x${B}[row]) = adf::tile(COL_OFFSET,ROW_OFFSET+row);


			adf::connect<pktstream, window<h1*w1*${BPE}>> (sp_a0.out[row], mm_x${B}[row].in[0]);
			adf::connect<pktstream, window<w1*w2*${BPE}>> (sp_b0.out[row], mm_x${B}[row].in[1]);

			if(row<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*${BPE}>> (mm_x${B}[row].out[0], mm_x${B}[row+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*${BPE}>>(mm_x${B}[row].out[0], out);
			}
		}
		
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}.h;


elif [ ${PACK_PER_B} == 2 ]
then
echo \
"
#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <adf.h>
#include \"para.h\"
using namespace adf;
#define NUM_ENGINES_PER_PAC ${B}

template <int COL_OFFSET, int ROW_OFFSET>
class mm_x${B}_graph : public adf::graph {
private:
	adf::kernel mm_x${B} [NUM_ENGINES_PER_PAC];
	adf::pktsplit<${NUM_PACK_IN}>  sp_a0;
	adf::pktsplit<${NUM_PACK_IN}>  sp_a1;
	adf::pktsplit<${NUM_PACK_IN}>  sp_b0;
	adf::pktsplit<${NUM_PACK_IN}>  sp_b1;

public:
	adf::port<input>  in[4];
  	adf::port<output>  out;

	mm_x${B}_graph() {
    
		sp_a0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_a1  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_b1  = adf::pktsplit<${NUM_PACK_IN}>::create();
		adf::connect< adf::pktstream > (in[0], sp_a0.in[0]);
		adf::connect< adf::pktstream > (in[1], sp_b0.in[0]);
		adf::connect< adf::pktstream > (in[2], sp_a1.in[0]);
		adf::connect< adf::pktstream > (in[3], sp_b1.in[0]);

		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			if(row==0){
				mm_x${B}[row]   = adf::kernel::create(mm_kernel);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel.cc\";
			}
			else{
				mm_x${B}[row]   = adf::kernel::create(mm_kernel1);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel1.cc\";
			}
		}
		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			adf::runtime<ratio>(mm_x${B}[row]) = 1;
			adf::location<kernel>(mm_x${B}[row]) = adf::tile(COL_OFFSET,ROW_OFFSET+row);

			if(row<${NUM_PACK_IN}){
				adf::connect<pktstream, window<h1*w1*${BPE}>> (sp_a0.out[row], mm_x${B}[row].in[0]);
				adf::connect<pktstream, window<w1*w2*${BPE}>> (sp_b0.out[row], mm_x${B}[row].in[1]);
			}
			else{
				adf::connect<pktstream, window<h1*w1*${BPE}>> (sp_a1.out[row-${NUM_PACK_IN}], mm_x${B}[row].in[0]);
				adf::connect<pktstream, window<w1*w2*${BPE}>> (sp_b1.out[row-${NUM_PACK_IN}], mm_x${B}[row].in[1]);
			}
			if(row<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*${BPE}>> (mm_x${B}[row].out[0], mm_x${B}[row+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*${BPE}>>(mm_x${B}[row].out[0], out);
			}
		}
		
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}.h;
fi

if [ ${B} == 3 ]
then
for ((i=0;i<2;i++));
do
if [ ${i} == 0 ]
then
	pos_col0=0;
	pos_row0=1;
	pos_col1=0;
	pos_row1=0;
	pos_col2=1;
	pos_row2=0;
else
	pos_col0=0;
	pos_row0=0;
	pos_col1=0;
	pos_row1=1;
	pos_col2='(-1)';
	pos_row2=1;
fi
echo \
"
#ifndef __GRAPH_TYPE${i}_H__
#define __GRAPH_TYPE${i}_H__
#include <adf.h>
#include \"para.h\"
using namespace adf;
#define NUM_ENGINES_PER_PAC ${B}
template <int COL_OFFSET, int ROW_OFFSET>
class mm_x${B}_graph_type${i} : public adf::graph {
private:
	adf::kernel mm_x${B} [NUM_ENGINES_PER_PAC];
	adf::pktsplit<${NUM_PACK_IN}>  sp_a0;
	adf::pktsplit<${NUM_PACK_IN}>  sp_b0;

public:
	adf::port<input>  in[2];
  	adf::port<output>  out;

	mm_x${B}_graph_type${i}() {
    
		// packet stream to different engines
		sp_a0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		adf::connect< adf::pktstream > (in[0], sp_a0.in[0]);
		adf::connect< adf::pktstream > (in[1], sp_b0.in[0]);

		// create NUM_ENGINES_PER_COL get_particles_i and n-body kernels
		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			if(row==0){
				mm_x${B}[row]   = adf::kernel::create(mm_kernel0);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel0.cc\";
			}
			else{
				mm_x${B}[row]   = adf::kernel::create(mm_kernel1);
				adf::source(mm_x${B}[row])   = \"aie/mm_kernel1.cc\";
			}
		}
		adf::location<kernel>(mm_x${B}[0]) = adf::tile(COL_OFFSET+${pos_col0},ROW_OFFSET+${pos_row0});
		adf::location<kernel>(mm_x${B}[1]) = adf::tile(COL_OFFSET+${pos_col1},ROW_OFFSET+${pos_row1});
		adf::location<kernel>(mm_x${B}[2]) = adf::tile(COL_OFFSET+${pos_col2},ROW_OFFSET+${pos_row2});

		for (int row =0; row<NUM_ENGINES_PER_PAC; row++)  {
			adf::runtime<ratio>(mm_x${B}[row]) = 1;
			adf::connect<pktstream, window<h1*w1*${BPE}>> (sp_a0.out[row], mm_x${B}[row].in[0]);
			adf::connect<pktstream, window<w1*w2*${BPE}>> (sp_b0.out[row], mm_x${B}[row].in[1]);

			if(row<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*${BPE}>> (mm_x${B}[row].out[0], mm_x${B}[row+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*${BPE}>>(mm_x${B}[row].out[0], out);
			}
		}
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}_type${i}.h;
done

echo \
"
#ifndef __GRAPH_COL_H__
#define __GRAPH_COL_H__
#include <adf.h>
#include \"para.h\"
using namespace adf;
#define NUM_ENGINES_PER_PAC ${B}

template <int COL_OFFSET, int ROW_OFFSET>
class mm_x${B}_graph_col : public adf::graph {
private:
	adf::kernel mm_x${B} [NUM_ENGINES_PER_PAC];
	adf::pktsplit<${NUM_PACK_IN}>  sp_a0;
	adf::pktsplit<${NUM_PACK_IN}>  sp_b0;

public:
	adf::port<input>  in[2];
  	adf::port<output>  out;

	mm_x${B}_graph_col() {
    
		// packet stream to different engines
		sp_a0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK_IN}>::create();
		adf::connect< adf::pktstream > (in[0], sp_a0.in[0]);
		adf::connect< adf::pktstream > (in[1], sp_b0.in[0]);

		// create NUM_ENGINES_PER_COL get_particles_i and n-body kernels
		for (int col =0; col<NUM_ENGINES_PER_PAC; col++)  {
			if(col==0){
				mm_x${B}[col]   = adf::kernel::create(mm_kernel0);
				adf::source(mm_x${B}[col])   = \"aie/mm_kernel0.cc\";
			}
			else{
				mm_x${B}[col]   = adf::kernel::create(mm_kernel1);
				adf::source(mm_x${B}[col])   = \"aie/mm_kernel1.cc\";
			}
		}
		for (int col =0; col<NUM_ENGINES_PER_PAC; col++)  {
			adf::runtime<ratio>(mm_x${B}[col]) = 1;
			adf::location<kernel>(mm_x${B}[col]) = adf::tile(COL_OFFSET+col,ROW_OFFSET);


			adf::connect<pktstream, window<h1*w1*${BPE}>> (sp_a0.out[col], mm_x${B}[col].in[0]);
			adf::connect<pktstream, window<w1*w2*${BPE}>> (sp_b0.out[col], mm_x${B}[col].in[1]);

			if(col<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*${BPE}>> (mm_x${B}[col].out[0], mm_x${B}[col+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*${BPE}>>(mm_x${B}[col].out[0], out);
			}
		}
		
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}_col.h;

fi