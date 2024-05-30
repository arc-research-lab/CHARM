
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>
{% if device == "vck190" %}
{% for acc in range(num_layer) -%}
#include "../aie/layer{{acc}}/aie_top_L{{acc}}.h"
{% endfor-%}
{% endif-%}

{% if device == "vck190" %}
// This is used for the PL Kernels
#include "xrt/experimental/xrt_kernel.h"
{% elif device == "vck5000" %}
// This is used for the PL Kernels
#include "xrt.h"
#include "experimental/xrt_kernel.h"
{% endif-%}

{% if device == "vck190" %}
// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
{% for acc in range(num_layer) -%}
{% set A=Host_cfg[acc][3] -%} 
{% set B=Host_cfg[acc][4] -%} 
{% set C=Host_cfg[acc][5] -%} 
mm_x{{A}}_x{{B}}_x{{C}}_graph{{acc}} mm_graph{{acc}};
{% endfor-%}
{% endif-%}

using namespace std;

void post_pro({{data_type}} *Data_hw, std::vector<{{data_type}}> &final_result, const int M1, const int N1, const int M){
    for (int n = 0; n < N1; n++) {
        for (int m = 0; m < M1; m++) { 
            int pos0=m+n*M;
            int pos1=m+n*M1;
            final_result[pos1]=Data_hw[pos0];
        } 
    }
}

void sw_mm(std::vector<{{data_type}}> const &DataInput0,std::vector<{{data_type}}> const &DataInput1,{{data_type}} *golden,const int M,const int K,const int N){
    {{data_type}} sum = 0;
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

{% for acc in range(num_layer) -%}
{% set h1=Host_cfg[acc][0] -%} 
{% set w1=Host_cfg[acc][1] -%} 
{% set w2=Host_cfg[acc][2] -%} 
{% set A=Host_cfg[acc][3] -%} 
{% set B=Host_cfg[acc][4] -%} 
{% set C=Host_cfg[acc][5] -%} 
{% set X=Host_cfg[acc][10] -%} 
{% set Y=Host_cfg[acc][11] -%} 
{% set Z=Host_cfg[acc][12] -%} 
const int H1_{{acc}}={{h1}};
const int W1_{{acc}}={{w1}};
const int W2_{{acc}}={{w2}};
const int A{{acc}}={{A}};
const int B{{acc}}={{B}};
const int C{{acc}}={{C}};
const int X{{acc}}={{X}};
const int Y{{acc}}={{Y}};
const int Z{{acc}}={{Z}};
const int M_ACC{{acc}}=H1_{{acc}}*A{{acc}}*X{{acc}};
const int K_ACC{{acc}}=W1_{{acc}}*B{{acc}}*Y{{acc}};
const int N_ACC{{acc}}=W2_{{acc}}*C{{acc}}*Z{{acc}};
{% endfor -%}


int main(int argc, char** argv) {

    int M1=4096,K1=4096,N1=4096;
    int iter=500,verify=0;
    char* xclbinFilename;
    if(argc == 7) {
        xclbinFilename = argv[1];
        if (sscanf (argv[2], "%i", &M1) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[3], "%i", &K1) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[4], "%i", &N1) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[5], "%i", &iter) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[6], "%i", &verify) != 1) {
            fprintf(stderr, "error - not an integer");
        }   
    }

    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////
    
    auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);
	auto dhdl = xrtDeviceOpenFromXcl(device);

    {% for acc in range(num_layer) -%}
    auto dma{{acc}} = xrt::kernel(device, uuid, "dma{{acc}}");
    {% endfor-%}
    
    float temp_m0=(float)(M1)/(float)(M_ACC0);
    float temp_k0=(float)(K1)/(float)(K_ACC0);
    float temp_n0=(float)(N1)/(float)(N_ACC0);
    int TX0=ceil(temp_m0);
    int TY0=ceil(temp_k0);
    int TZ0=ceil(temp_n0);
    
    int M =TX0 * M_ACC0;
    int K =TY0 * K_ACC0;
    int N =TZ0 * N_ACC0;
    int sizeIn1 = M * K;
    int sizeIn2 = K * N;
    int sizeOut = M * N;
    int sizeOut1 = M1 * N1;
    
    std::vector<{{data_type}}> DataInput0(sizeIn1,1);
    std::vector<{{data_type}}> DataInput1(sizeIn2,1);
    {{data_type}}* golden = new {{data_type}}[sizeOut];
    std::vector<{{data_type}}> final_result_sw(sizeOut1,1);
    std::vector<{{data_type}}> final_result_hw(sizeOut1,1);

    srand (time(0));
    for (int k = 0; k < K; k++) {
        for (int m = 0; m < M; m++) {
            int pos = m + k*M;
            if (m>=M1||k>=K1){
                DataInput0[pos]= 0;
            }
            else{
                DataInput0[pos]= ({{data_type}})(rand()%5);
            }
            
        }
    } 

    for (int n = 0; n < N; n++) {
        for (int k = 0; k < K; k++) {
            int pos = k + n * K;
            if (k>=K1||n>=N1){
                DataInput1[pos]= 0;
            }
            else{
                DataInput1[pos]= ({{data_type}})(rand()%5);
            }
            
        }
    }

    //Allocate input mem
    auto in_bohdl0 = xrt::bo(device, sizeIn1 * sizeof({{data_type}}), dma0.group_id(0));
    auto in_bomapped0 = in_bohdl0.map<{{data_type}}*>();
    for (int k = 0; k < sizeIn1; k++) {
        in_bomapped0 [k]= DataInput0[k];
    } 

    auto in_bohdl1 = xrt::bo(device, sizeIn2 * sizeof({{data_type}}), dma0.group_id(0));
    auto in_bomapped1 = in_bohdl1.map<{{data_type}}*>();
    for (int k = 0; k < sizeIn2; k++) {
        in_bomapped1 [k]= DataInput1[k];
    } 

    // sync input memory
    in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn1* sizeof({{data_type}}),0);
    in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn2* sizeof({{data_type}}),0);
    
    //Allocate output buffer
    auto out_bohdl = xrt::bo(device, sizeOut* sizeof({{data_type}}), dma0.group_id(0));
    auto out_bomapped = out_bohdl.map<{{data_type}}*>();
    
    {% if device == "vck190" %}
    {% for acc in range(num_layer) -%}
    auto ghdl{{acc}}=xrt::graph(device,uuid,"mm_graph{{acc}}");
    {% endfor-%}
    printf("graph run\n");
    {% for acc in range(num_layer) -%}
    ghdl{{acc}}.run(-1);
    {% endfor-%}
    {% endif %}

    std::cout << "Kernel run\n";
    {% for acc in range(num_layer) -%}
    xrt::run dma_run{{acc}};
    {% endfor-%}
    
    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    for (int i=0;i<iter;i++){
        // start input kernels run handles
        {% for acc in range(num_layer) -%}
        {% set A=Host_cfg[acc][3] -%} 
        {% set B=Host_cfg[acc][4] -%} 
        {% set C=Host_cfg[acc][5] -%}
        {% set NUM_TXL=Host_cfg[acc][6] -%} 
        {% set NUM_TXR=Host_cfg[acc][7] -%} 
        {% set PACK_IN=Host_cfg[acc][8] -%}
        {% set PACK_OUT=Host_cfg[acc][9] -%}
        dma_run{{acc}} = dma{{acc}}(in_bohdl0, in_bohdl1,out_bohdl,
                {% set num_lhs=A*NUM_TXL*(B//PACK_IN) -%}
                {% set div_lhs=num_lhs//4 -%}
                {% set left_lhs=num_lhs%4 -%}
                {% for i in range(div_lhs) -%}
                nullptr, nullptr, nullptr, nullptr,
                {% endfor-%}
                {% for i in range(left_lhs)%} nullptr,{% endfor-%}

                {% set num_rhs=C*NUM_TXR*(B//PACK_IN) -%}
                {% set div_rhs=num_rhs//4 -%}
                {% set left_rhs=num_rhs%4 -%}
                {% for i in range(div_rhs) -%}
                nullptr, nullptr, nullptr, nullptr,
                {% endfor-%}
                {% for i in range(left_rhs)%} nullptr,{% endfor-%}

                {% set num_out=A*(C//PACK_OUT) -%}
                {% set div_out=num_out//4 -%}
                {% set left_out=num_out%4 -%}
                {% for i in range(div_out) -%}
                nullptr, nullptr, nullptr, nullptr,
                {% endfor-%}
                {% for i in range(left_out)%} nullptr,{% endfor-%}
                TX{{acc}},TY{{acc}},TZ{{acc}});
        {% endfor %}

        {% for acc in range(num_layer) -%}
        dma_run{{acc}}.wait();
        {% endfor-%}
    }
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    kernel_time_in_sec = kernel_time.count();
    double TOPS = (double)(M1 * K1) * (double) (N1 * 2 *iter* 1e-9) / kernel_time_in_sec;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "MM Size:"<< M1 << "*" << K1 << "*" << N1 << std::endl;
    std::cout << "Total time is: "<< kernel_time_in_sec <<"s, TOPS = " << TOPS << " GOPS/s" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // sync output memory
    out_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof({{data_type}}),/*OFFSET=*/ 0);

    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        
        post_pro(out_bomapped, final_result_hw, M1, N1, M);
        sw_mm(DataInput0,DataInput1,golden,M,K,N);
        post_pro(golden, final_result_sw, M1, N1, M);
        int errorCount = 0;  
        for (int len = 0; len < sizeOut1; len++) {

            if(abs(({{data_type}})(final_result_hw[len])-final_result_sw[len])>=1e-4){
                printf("Error found final_result_hw[%d]!=final_result_sw[%d], {{format}}!={{format}} \n", len,len,final_result_hw[len],final_result_sw[len]);
                errorCount++;
            }
   
        }
        if (errorCount)
            printf("Test failed with %d errors\n", errorCount);
        else
            printf("TEST PASSED\n");
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << "Releasing remaining XRT objects...\n";
    
    return 0;
}

