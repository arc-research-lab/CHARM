if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_host.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

if [ -f ./${dir_name}/host/host.cpp ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/host/host.cpp; exists." 
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

if [ ${data_type} == "int32" ]
then
    D_TYPE="int";
    PRINT_F="%d"
elif [ ${data_type} == "fp32" ]
then
    D_TYPE="float";
    PRINT_F="%f"
elif [ ${data_type} == "int16" ]
then
    D_TYPE="uint16_t";
    PRINT_F="%d"
fi


let temp=${A}*${C};
let AIE_NUM=${A}*${B}*${C};

if [ ${B} == 4 ]
then
	let left_aie=400-${AIE_NUM};
elif [ ${B} == 3 ]
then
	if [ ${AIE_NUM} -gt 300 ]
	then
		let left_aie=$(((50%${C})/2*16));
	else	
		let left_aie=400-${AIE_NUM}/3*4;
	fi
fi
let start_pos=${left_aie}/16;
let port_row_in=${A}*${B}*${C}/${NUM_PACK_IN}/${R_BRO};
let port_col_in=${C}*${B}*${A}/${NUM_PACK_IN}/${C_BRO};
let port_out=${A}*${C}/${NUM_PACK_OUT};

mkdir -p ${dir_name}/host
echo \
"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>
#include \"../aie/mm_top.h\"

// This is used for the PL Kernels
#include \"xrt/xrt.h\"
#include \"xrt/experimental/xrt_kernel.h\"

// Using the ADF API that call XRT API
#include \"adf/adf_api/XRTConfig.h\"
mm_x${temp}_x${B}_graph<${start_pos},0> myGraph;
using namespace std;

static std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm) {
    if (fnm.empty()) throw std::runtime_error(\"No xclbin specified\");

    // load bit stream
    std::ifstream stream(fnm);
    stream.seekg(0, stream.end);
    size_t size = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<char> header(size);
    stream.read(header.data(), size);

    auto top = reinterpret_cast<const axlf*>(header.data());
    if (xrtDeviceLoadXclbin(device, top)) throw std::runtime_error(\"Xclbin loading failed\");

    return header;
}

void sw_mm(std::vector<${D_TYPE}> DataInput0,std::vector<${D_TYPE}> DataInput1,std::vector<${D_TYPE}> &golden,const int M,const int K,const int N){
    ${D_TYPE} sum = 0;
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            sum =0;
            for (int k = 0; k < K; k++) {
                sum=sum+DataInput0[m+k*M]*DataInput1[k+n*K];
            }
            golden[m+n*M]=sum;
        } 
    }
}
 
const int H1=${i};
const int W1=${k};
const int W2=${j};
const int A=${A};
const int B=${B};
const int C=${C};
const int X=${X};
const int Y=${Y};
const int Z=${Z};

int main(int argc, char** argv) {

    int TX,TY,TZ;
    int M1=4096,K1=4096,N1=4096;
    int iter=500,verify=0;
    char* xclbinFilename;
    if(argc == 7) {
        xclbinFilename = argv[1];
        if (sscanf (argv[2], \"%i\", &M1) != 1) {
            fprintf(stderr, \"error - not an integer\");
        }
        if (sscanf (argv[3], \"%i\", &K1) != 1) {
            fprintf(stderr, \"error - not an integer\");
        }
        if (sscanf (argv[4], \"%i\", &N1) != 1) {
            fprintf(stderr, \"error - not an integer\");
        }
        if (sscanf (argv[5], \"%i\", &iter) != 1) {
            fprintf(stderr, \"error - not an integer\");
        }
        if (sscanf (argv[6], \"%i\", &verify) != 1) {
            fprintf(stderr, \"error - not an integer\");
        }   
    }

    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////
    
    auto dhdl = xrtDeviceOpen(0); // Open Device the local device
    if (dhdl == nullptr) throw std::runtime_error(\"No valid device handle found. Make sure using right xclOpen index.\");
    auto xclbin = load_xclbin(dhdl, xclbinFilename);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);

    float temp_m=(float)(M1)/(float)(X*A*H1);
    float temp_k=(float)(K1)/(float)(Y*B*W1);
    float temp_n=(float)(N1)/(float)(Z*C*W2);
    TX=ceil(temp_m);
    TY=ceil(temp_k);
    TZ=ceil(temp_n);
    std::cout << TX << TY << TZ << std::endl;
    int M =TX * X * A * H1;
    int K =TY * Y * B * W1;
    int N =TZ * Z * C * W2;
    int sizeIn1 = M * K;
    int sizeIn2 = K * N;
    int sizeOut = M * N;
    
    std::vector<${D_TYPE}> DataInput0(sizeIn1,1);
    std::vector<${D_TYPE}> DataInput1(sizeIn2,1);
    std::vector<${D_TYPE}> golden(sizeOut,1);

    srand (time(0));
    for (int k = 0; k < sizeIn1; k++) {
        DataInput0[k]= (rand()%5)*(${D_TYPE})(1);
    } 


    srand (time(0));
    for (int k = 0; k < sizeIn2; k++) {
        DataInput1[k]= (rand()%5)*(${D_TYPE})(1);
    }

    //Allocate input mem
    xrtBufferHandle in_bohdl0 = xrtBOAlloc(dhdl, sizeIn1 * sizeof(${D_TYPE}), 0, 0);
    auto in_bomapped0 = reinterpret_cast<${D_TYPE}*>(xrtBOMap(in_bohdl0));
    for (int k = 0; k < sizeIn1; k++) {
        in_bomapped0 [k]= DataInput0[k];
    } 

    xrtBufferHandle in_bohdl1 = xrtBOAlloc(dhdl, sizeIn2 * sizeof(${D_TYPE}), 0, 0);
    auto in_bomapped1 = reinterpret_cast<${D_TYPE}*>(xrtBOMap(in_bohdl1));
    for (int k = 0; k < sizeIn2; k++) {
        in_bomapped1 [k]= DataInput1[k];
    } 

    // sync input memory
    xrtBOSync(in_bohdl0, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn1* sizeof(${D_TYPE}),0);
    xrtBOSync(in_bohdl1, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn2* sizeof(${D_TYPE}),0);
    
    //Allocate output buffer
    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, sizeOut* sizeof(${D_TYPE}), 0, 0);
    auto out_bomapped = reinterpret_cast<${D_TYPE}*>(xrtBOMap(out_bohdl));
    memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(${D_TYPE}));
    
    //int graph_iter=X*Y*Z*TX*TY*TZ;
    myGraph.init();
           
    printf(\"graph run\n\");
    myGraph.run(-1);

    std::cout << \"Kernel run\n\";
    xrtKernelHandle dma_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, \"dma\");
    xrtRunHandle dma_rhdl;
    
    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    for (int i=0;i<iter;i++){
    // start input kernels run handles
    dma_rhdl = xrtKernelRun(dma_khdl, in_bohdl0, in_bohdl1,out_bohdl,TX,TY,TZ,">> ./${dir_name}/host/host.cpp;

echo -n "            ">> ./${dir_name}/host/host.cpp;
for ((i=0;i<${port_row_in};i++));
do  
    echo -n "nullptr, ">> ./${dir_name}/host/host.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/host/host.cpp;
        echo -n "            ">> ./${dir_name}/host/host.cpp;
    fi
done

for ((i=0;i<${port_col_in};i++));
do  
    echo -n "nullptr, ">> ./${dir_name}/host/host.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/host/host.cpp;
        echo -n "            ">> ./${dir_name}/host/host.cpp;
    fi
done

for ((i=0;i<${port_out};i++));
do  
    if (( ${i} != ${port_out}-1 ))
    then
        echo -n "nullptr, ">> ./${dir_name}/host/host.cpp;
        if (( ${i}%4 == 3 ))
        then
            echo "">> ./${dir_name}/host/host.cpp;
            echo -n "            ">> ./${dir_name}/host/host.cpp;
        fi
    else
        echo -n "nullptr);">> ./${dir_name}/host/host.cpp;
    fi
done

echo \
"
        xrtRunWait(dma_rhdl);
    }
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    kernel_time_in_sec = kernel_time.count();
    double TOPS = (double)(M1 * K1) * (double) (N1 * 2 *iter* 1e-9) / kernel_time_in_sec;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << \"MM Size:\"<< M1 << \"*\" << K1 << \"*\" << N1 << std::endl;
    std::cout << \"Total time is: \"<< kernel_time_in_sec <<\"s, TOPS = \" << TOPS << \" GOPS/s\" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // sync output memory
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(${D_TYPE}),/*OFFSET=*/ 0);
        
    xrtRunClose(dma_rhdl);
    xrtKernelClose(dma_khdl);

    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        
        sw_mm(DataInput0,DataInput1,golden,M,K,N);
    
        int errorCount = 0;  
        for (int m = 0; m < M; m++) {
            for (int n = 0; n < N; n++) {
                if(abs((${D_TYPE})(out_bomapped[m+n*M])-golden[m+n*M])>=1e-4){
                    printf(\"Error found out_bomapped[%d][%d]!=golden[%d][%d], ${PRINT_F}!=${PRINT_F} \n\", m,n,m,n,out_bomapped[m+n*M],golden[m+n*M]);
                    errorCount++;
                }

            } 
        }
        if (errorCount)
            printf(\"Test failed with %d errors\n\", errorCount);
        else
            printf(\"TEST PASSED\n\");
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << \"Releasing remaining XRT objects...\n\";
    
    xrtBOFree(out_bohdl);
    xrtBOFree(in_bohdl0);
    xrtBOFree(in_bohdl1);
    //myGraph.end();
    xrtDeviceClose(dhdl);
    return 0;
}
">> ./${dir_name}/host/host.cpp;