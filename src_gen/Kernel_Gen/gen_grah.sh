if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_grah.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

if [ -f ./${dir_name}/aie/aie_graph.h ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/aie/aie_graph.h exists." 
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
    elif (( ${n} == 8 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		AIEArrGen="${Value[0]}";
    elif (( ${n} == 9 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		NUM_PACK_IN="${Value[0]}";
    elif (( ${n} == 16 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		SysGen="${Value[0]}";
 	fi
done < "$input"

if (( ${SysGen} == 1 )) || (( ${AIEArrGen} == 1 ))
then
    kernel_type=1;
fi

if [ ${data_type} == "fp32" ] || [ ${kernel_type} == "int32" ]
then
	BPE=4;
elif [ ${data_type} == "int16" ]
then
	BPE=2;
fi

mkdir -p ${dir_name}/aie
if [ ${data_type} == "int32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "'"para.h"'"
using namespace adf;

class simpleGraph : public graph {
   private:
    kernel mm;

   public:
    port<input> in0, in1;
    port<output> out;

    simpleGraph() {
        mm = kernel::create(mm_kernel0);

        connect<window<h1*w1*${BPE}>>(in0, mm.in[0]);
        connect<window<w1*w2*${BPE}>>(in1, mm.in[1]);
        connect<window<h1*w2*${BPE}>>(mm.out[0], out);


        source(mm) = "'"mm_kernel0.cc"'";

        runtime<ratio>(mm) = 1;
    };
};

#endif
">> ./${dir_name}/aie/aie_graph.h;

elif [ ${data_type} == "int32" ] && [ ${kernel_type} == 1 ]
then
echo \
"#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "'"para.h"'"
using namespace adf;

class simpleGraph : public graph {
   private:
    kernel mm;
    kernel mm1;

   public:
    port<input> in0, in1, in2, in3;
    port<output> out;

    simpleGraph() {
        mm = kernel::create(mm_kernel0);
        mm1 = kernel::create(mm_kernel1);

        connect<window<h1*w1*${BPE}>>(in0, mm.in[0]);
        connect<window<w1*w2*${BPE}>>(in1, mm.in[1]);
        connect<window<h1*w1*${BPE}>>(in2, mm1.in[0]);
        connect<window<w1*w2*${BPE}>>(in3, mm1.in[1]);
        connect<window<h1*w2*${BPE}>>(mm.out[0], mm1.in[2]);
        connect<window<h1*w2*${BPE}>>(mm1.out[0], out);



        source(mm) = "'"mm_kernel0.cc"'";
        source(mm1) = "'"mm_kernel1.cc"'";
        runtime<ratio>(mm) = 1;
        runtime<ratio>(mm1) = 1;
    };
};

#endif
">> ./${dir_name}/aie/aie_graph.h;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include \"para.h\"
using namespace adf;

class simpleGraph : public graph {
   private:
    kernel mm;

   public:
    port<input> in0, in1;
    port<output> out;

    simpleGraph() {
        mm = kernel::create(mm_kernel0);

        connect<window<h1*w1*${BPE}>>(in0, mm.in[0]);
        connect<window<w1*w2*${BPE}>>(in1, mm.in[1]);
        connect<window<h1*w2*${BPE}>>(mm.out[0], out);


        source(mm) = \"mm_kernel0.cc\";

        runtime<ratio>(mm) = 1;
    };
};

#endif
">> ./${dir_name}/aie/aie_graph.h;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include \"para.h\"
using namespace adf;

class simpleGraph : public graph {
   private:
    kernel mm;

   public:
    port<input> in0, in1;
    port<output> out;

    simpleGraph() {
        mm = kernel::create(mm_kernel0);

        connect<window<h1*w1*${BPE}>>(in0, mm.in[0]);
        connect<window<w1*w2*${BPE}>>(in1, mm.in[1]);
        connect<window<h1*w2*${BPE}>>(mm.out[0], out);


        source(mm) = \"mm_kernel0.cc\";

        runtime<ratio>(mm) = 1;
    };
};

#endif
">> ./${dir_name}/aie/aie_graph.h;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 1 ]
then
echo \
"#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "'"para.h"'"
using namespace adf;

class simpleGraph : public graph {
   private:
    kernel mm;
    kernel mm1;

   public:
    port<input> in0, in1, in2, in3;
    port<output> out;

    simpleGraph() {
        mm = kernel::create(mm_kernel0);
        mm1 = kernel::create(mm_kernel1);

        connect<window<h1*w1*${BPE}>>(in0, mm.in[0]);
        connect<window<w1*w2*${BPE}>>(in1, mm.in[1]);
        connect<window<h1*w1*${BPE}>>(in2, mm1.in[0]);
        connect<window<w1*w2*${BPE}>>(in3, mm1.in[1]);
        connect<window<h1*w2*${BPE}>>(mm.out[0], mm1.in[2]);
        connect<window<h1*w2*${BPE}>>(mm1.out[0], out);



        source(mm) = "'"mm_kernel0.cc"'";
        source(mm1) = "'"mm_kernel1.cc"'";
        runtime<ratio>(mm) = 1;
        runtime<ratio>(mm1) = 1;
    };
};

#endif
">> ./${dir_name}/aie/aie_graph.h;

fi