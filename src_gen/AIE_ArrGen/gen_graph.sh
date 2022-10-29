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
 	fi
done < "$input"

if [ ${B} != 4 ] && [ ${B} != 8 ]
then
    echo ""
    echo "******************************************"
    echo "Currently, CHARM supports B=4 or B=8"
    echo "******************************************"
    echo ""
    exit
fi

mkdir -p ${dir_name}/aie
if [ ${B} == 4 ] 
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
	adf::pktsplit<${NUM_PACK}>  sp_a0;
	adf::pktsplit<${NUM_PACK}>  sp_b0;

public:
	adf::port<input>  in[2];
  	adf::port<output>  out;

	mm_x${B}_graph() {
    
		// packet stream to different engines
		sp_a0  = adf::pktsplit<${NUM_PACK}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK}>::create();
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


			adf::connect<pktstream, window<h1*w1*4>> (sp_a0.out[row], mm_x${B}[row].in[0]);
			adf::connect<pktstream, window<w1*w2*4>> (sp_b0.out[row], mm_x${B}[row].in[1]);

			if(row<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*4>> (mm_x${B}[row].out[0], mm_x${B}[row+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*4>>(mm_x${B}[row].out[0], out);
			}
		}
		
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}.h;

elif [ ${B} == 8 ] 
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
	adf::pktsplit<${NUM_PACK}>  sp_a0;
	adf::pktsplit<${NUM_PACK}>  sp_a1;
	adf::pktsplit<${NUM_PACK}>  sp_b0;
	adf::pktsplit<${NUM_PACK}>  sp_b1;

public:
	adf::port<input>  in[4];
  	adf::port<output>  out;

	mm_x${B}_graph() {
    
		sp_a0  = adf::pktsplit<${NUM_PACK}>::create();
		sp_a1  = adf::pktsplit<${NUM_PACK}>::create();
		sp_b0  = adf::pktsplit<${NUM_PACK}>::create();
		sp_b1  = adf::pktsplit<${NUM_PACK}>::create();
		adf::connect< adf::pktstream > (in[0], sp_a0.in[0]);
		adf::connect< adf::pktstream > (in[1], sp_a1.in[0]);
		adf::connect< adf::pktstream > (in[2], sp_b0.in[0]);
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

			if(row<4){
				adf::connect<pktstream, window<h1*w1*4>> (sp_a0.out[row], mm_x${B}[row].in[0]);
				adf::connect<pktstream, window<w1*w2*4>> (sp_b0.out[row], mm_x${B}[row].in[1]);
			}
			else{
				adf::connect<pktstream, window<h1*w1*4>> (sp_a1.out[row-4], mm_x${B}[row].in[0]);
				adf::connect<pktstream, window<w1*w2*4>> (sp_b1.out[row-4], mm_x${B}[row].in[1]);
			}
			if(row<NUM_ENGINES_PER_PAC-1){
				adf::connect<window<h1*w2*4>> (mm_x${B}[row].out[0], mm_x${B}[row+1].in[2]);
			}
			else{
				adf::connect<window<h1*w2*4>>(mm_x${B}[row].out[0], out);
			}
		}
		
	};
};

#endif
">> ./${dir_name}/aie/mm_graph_x${B}.h;
fi