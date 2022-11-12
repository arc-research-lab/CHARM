if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
	echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_gracpp.sh ../../config_files/input.cfg ."
    echo "******************************************"
	echo ""
    exit
fi

if [ -f ./${dir_name}/aie/aie_graph.cpp ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/aie/aie_graph.cpp exists." 
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

mkdir -p ${dir_name}/aie
if [ ${data_type} == "int32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#include "'"aie_graph.h"'"
using namespace adf;

PLIO* in0 = new PLIO("'"DataIn0"'", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in1 = new PLIO("'"DataIn1"'", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* out = new PLIO("'"DataOut"'", adf::plio_32_bits, "'"data/output.txt"'",1000);

simulation::platform<2, 1> platform(in0, in1, out);

simpleGraph addergraph;

connect<> net0(platform.src[0], addergraph.in0);
connect<> net1(platform.src[1], addergraph.in1);

connect<> net2(addergraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(1);
    addergraph.end();
    return 0;
}
#endif
">> ./${dir_name}/aie/aie_graph.cpp;

elif [ ${data_type} == "int32" ] && [ ${kernel_type} == 1 ]
then
echo \
"#include "'"aie_graph.h"'"
using namespace adf;

PLIO* in0 = new PLIO("'"DataIn0"'", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in1 = new PLIO("'"DataIn1"'", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* in2 = new PLIO("'"DataIn2"'", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in3 = new PLIO("'"DataIn3"'", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* out = new PLIO("'"DataOut"'", adf::plio_32_bits, "'"data/output.txt"'",1000);

simulation::platform<4, 1> platform(in0, in1, in2, in3, out);

simpleGraph addergraph;

connect<> net0(platform.src[0], addergraph.in0);
connect<> net1(platform.src[1], addergraph.in1);
connect<> net2(platform.src[2], addergraph.in2);
connect<> net3(platform.src[3], addergraph.in3);

connect<> net4(addergraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(1);
    addergraph.end();
    return 0;
}
#endif
">> ./${dir_name}/aie/aie_graph.cpp;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#include \"aie_graph.h\"
using namespace adf;

PLIO* in0 = new PLIO(\"DataIn0\", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in1 = new PLIO(\"DataIn1\", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* out = new PLIO(\"DataOut\", adf::plio_32_bits, \"data/output.txt\",1000);

simulation::platform<2, 1> platform(in0, in1, out);

simpleGraph addergraph;

connect<> net0(platform.src[0], addergraph.in0);
connect<> net1(platform.src[1], addergraph.in1);

connect<> net2(addergraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(1);
    addergraph.end();
    return 0;
}
#endif
">> ./${dir_name}/aie/aie_graph.cpp;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 0 ]
then
echo \
"#include \"aie_graph.h\"
using namespace adf;

PLIO* in0 = new PLIO(\"DataIn0\", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in1 = new PLIO(\"DataIn1\", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* out = new PLIO(\"DataOut\", adf::plio_32_bits, \"data/output.txt\",1000);

simulation::platform<2, 1> platform(in0, in1, out);

simpleGraph addergraph;

connect<> net0(platform.src[0], addergraph.in0);
connect<> net1(platform.src[1], addergraph.in1);

connect<> net2(addergraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(1);
    addergraph.end();
    return 0;
}
#endif
">> ./${dir_name}/aie/aie_graph.cpp;

elif [ ${data_type} == "fp32" ] && [ ${kernel_type} == 1 ]
then
echo \
"#include "'"aie_graph.h"'"
using namespace adf;

PLIO* in0 = new PLIO("'"DataIn0"'", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in1 = new PLIO("'"DataIn1"'", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* in2 = new PLIO("'"DataIn2"'", adf::plio_32_bits, \"../${din_name}input0.txt\",1000);
PLIO* in3 = new PLIO("'"DataIn3"'", adf::plio_32_bits, \"../${din_name}input1.txt\",1000);
PLIO* out = new PLIO("'"DataOut"'", adf::plio_32_bits, "'"data/output.txt"'",1000);

simulation::platform<4, 1> platform(in0, in1, in2, in3, out);

simpleGraph addergraph;

connect<> net0(platform.src[0], addergraph.in0);
connect<> net1(platform.src[1], addergraph.in1);
connect<> net2(platform.src[2], addergraph.in2);
connect<> net3(platform.src[3], addergraph.in3);

connect<> net4(addergraph.out, platform.sink[0]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    addergraph.init();
    addergraph.run(1);
    addergraph.end();
    return 0;
}
#endif
">> ./${dir_name}/aie/aie_graph.cpp;

fi