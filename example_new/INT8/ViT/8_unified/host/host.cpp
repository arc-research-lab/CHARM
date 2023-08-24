
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>

#include "../aie/layer0/aie_top_L0.h"
#include "../aie/layer1/aie_top_L1.h"
#include "../aie/layer2/aie_top_L2.h"
#include "../aie/layer3/aie_top_L3.h"
#include "../aie/layer4/aie_top_L4.h"
#include "../aie/layer5/aie_top_L5.h"
#include "../aie/layer6/aie_top_L6.h"
#include "../aie/layer7/aie_top_L7.h"

// This is used for the PL Kernels
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
mm_x3_x2_x2_graph0 mm_graph0;
mm_x3_x2_x2_graph1 mm_graph1;
mm_x3_x2_x2_graph2 mm_graph2;
mm_x3_x2_x2_graph3 mm_graph3;
mm_x3_x2_x2_graph4 mm_graph4;
mm_x3_x2_x2_graph5 mm_graph5;
mm_x3_x2_x2_graph6 mm_graph6;
mm_x3_x2_x2_graph7 mm_graph7;
using namespace std;

void post_pro(int8_t *Data_hw, std::vector<int8_t> final_result, const int M1, const int N1, const int M){
    for (int n = 0; n < N1; n++) {
        for (int m = 0; m < M1; m++) { 
            int pos0=m+n*M;
            int pos1=m+n*M1;
            final_result[pos1]=Data_hw[pos0];
        } 
    }
}

void sw_mm(std::vector<int8_t> DataInput0,std::vector<int8_t> DataInput1,int8_t *golden,const int M,const int K,const int N){
    int8_t sum = 0;
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

const int H1_0=64;
const int W1_0=64;
const int W2_0=64;
const int A0=3;
const int B0=2;
const int C0=2;
const int X0=2;
const int Y0=4;
const int Z0=4;
const int M_ACC0=H1_0*A0*X0;
const int K_ACC0=W1_0*B0*Y0;
const int N_ACC0=W2_0*C0*Z0;



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

    auto dma0 = xrt::kernel(device, uuid, "dma0");
    auto dma1 = xrt::kernel(device, uuid, "dma1");
    auto dma2 = xrt::kernel(device, uuid, "dma2");
    auto dma3 = xrt::kernel(device, uuid, "dma3");
    auto dma4 = xrt::kernel(device, uuid, "dma4");
    auto dma5 = xrt::kernel(device, uuid, "dma5");
    auto dma6 = xrt::kernel(device, uuid, "dma6");
    auto dma7 = xrt::kernel(device, uuid, "dma7");
    
    
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
    
    std::vector<int8_t> DataInput0(sizeIn1,1);
    std::vector<int8_t> DataInput1(sizeIn2,1);
    int8_t* golden = new int8_t[sizeOut];
    std::vector<int8_t> final_result_sw(sizeOut1,1);
    std::vector<int8_t> final_result_hw(sizeOut1,1);

    srand (time(0));
    for (int k = 0; k < K; k++) {
        for (int m = 0; m < M; m++) {
            int pos = m + k*M;
            if (m>=M1||k>=K1){
                DataInput0[pos]= 0;
            }
            else{
                DataInput0[pos]= (int8_t)(rand());
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
                DataInput1[pos]= (int8_t)(rand());
            }
            
        }
    }

    //Allocate input mem
    auto in_bohdl0 = xrt::bo(device, sizeIn1 * sizeof(int8_t), dma0.group_id(0));
    auto in_bomapped0 = in_bohdl0.map<int8_t*>();
    for (int k = 0; k < sizeIn1; k++) {
        in_bomapped0 [k]= DataInput0[k];
    } 

    auto in_bohdl1 = xrt::bo(device, sizeIn2 * sizeof(int8_t), dma0.group_id(0));
    auto in_bomapped1 = in_bohdl1.map<int8_t*>();
    for (int k = 0; k < sizeIn2; k++) {
        in_bomapped1 [k]= DataInput1[k];
    } 

    // sync input memory
    in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn1* sizeof(int8_t),0);
    in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn2* sizeof(int8_t),0);
    
    //Allocate output buffer
    auto out_bohdl0 = xrt::bo(device, sizeOut* sizeof(int8_t), dma0.group_id(0));
    auto out_bomapped0 = out_bohdl0.map<int8_t*>();

    auto out_bohdl1 = xrt::bo(device, sizeOut* sizeof(int8_t), dma1.group_id(0));
    auto out_bomapped1 = out_bohdl1.map<int8_t*>();

    auto out_bohdl2 = xrt::bo(device, sizeOut* sizeof(int8_t), dma2.group_id(0));
    auto out_bomapped2 = out_bohdl2.map<int8_t*>();

    auto out_bohdl3 = xrt::bo(device, sizeOut* sizeof(int8_t), dma3.group_id(0));
    auto out_bomapped3 = out_bohdl3.map<int8_t*>();

    auto out_bohdl4 = xrt::bo(device, sizeOut* sizeof(int8_t), dma4.group_id(0));
    auto out_bomapped4 = out_bohdl4.map<int8_t*>();

    auto out_bohdl5 = xrt::bo(device, sizeOut* sizeof(int8_t), dma5.group_id(0));
    auto out_bomapped5 = out_bohdl5.map<int8_t*>();

    auto out_bohdl6 = xrt::bo(device, sizeOut* sizeof(int8_t), dma6.group_id(0));
    auto out_bomapped6 = out_bohdl6.map<int8_t*>();

    auto out_bohdl7 = xrt::bo(device, sizeOut* sizeof(int8_t), dma7.group_id(0));
    auto out_bomapped7 = out_bohdl7.map<int8_t*>();
    
    
    auto ghdl0=xrt::graph(device,uuid,"mm_graph0");
    auto ghdl1=xrt::graph(device,uuid,"mm_graph1");
    auto ghdl2=xrt::graph(device,uuid,"mm_graph2");
    auto ghdl3=xrt::graph(device,uuid,"mm_graph3");
    auto ghdl4=xrt::graph(device,uuid,"mm_graph4");
    auto ghdl5=xrt::graph(device,uuid,"mm_graph5");
    auto ghdl6=xrt::graph(device,uuid,"mm_graph6");
    auto ghdl7=xrt::graph(device,uuid,"mm_graph7");
    printf("graph run\n");
    ghdl0.run(-1);
    ghdl1.run(-1);
    ghdl2.run(-1);
    ghdl3.run(-1);
    ghdl4.run(-1);
    ghdl5.run(-1);
    ghdl6.run(-1);
    ghdl7.run(-1);
    

    std::cout << "Kernel run\n";
    xrt::run dma_run0;
    xrt::run dma_run1;
    xrt::run dma_run2;
    xrt::run dma_run3;
    xrt::run dma_run4;
    xrt::run dma_run5;
    xrt::run dma_run6;
    xrt::run dma_run7;

    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    for (int i=0;i<iter;i++){
        // start input kernels run handles
        dma_run0 = dma0(in_bohdl0, in_bohdl1,out_bohdl0,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run1 = dma1(in_bohdl0, in_bohdl1,out_bohdl1,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run2 = dma2(in_bohdl0, in_bohdl1,out_bohdl2,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run3 = dma3(in_bohdl0, in_bohdl1,out_bohdl3,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run4 = dma4(in_bohdl0, in_bohdl1,out_bohdl4,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run5 = dma5(in_bohdl0, in_bohdl1,out_bohdl5,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run6 = dma6(in_bohdl0, in_bohdl1,out_bohdl6,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_run7 = dma7(in_bohdl0, in_bohdl1,out_bohdl7,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        

        dma_run0.wait();
        dma_run1.wait();
        dma_run2.wait();
        dma_run3.wait();
        dma_run4.wait();
        dma_run5.wait();
        dma_run6.wait();
        dma_run7.wait();
        }
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    kernel_time_in_sec = kernel_time.count();
    double TOPS = (double)(M1 * K1) * (double) (N1 * 8 * 2 *iter* 1e-9) / kernel_time_in_sec;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "MM Size:"<< M1 << "*" << K1 << "*" << N1 << std::endl;
    std::cout << "Total time is (s): " << std::endl;
    std::cout << kernel_time_in_sec << std::endl;
    std::cout << "TOPS = " << TOPS << " GOPS/s" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // sync output memory
    out_bohdl0.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl1.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl2.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl3.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl4.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl5.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl6.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);
    out_bohdl7.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int8_t),/*OFFSET=*/ 0);

    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        
        post_pro(out_bomapped0, final_result_hw, M1, N1, M);
        sw_mm(DataInput0,DataInput1,golden,M,K,N);
        post_pro(golden, final_result_sw, M1, N1, M);
        int errorCount = 0;  
        for (int len = 0; len < sizeOut1; len++) {

            if(abs((int8_t)(final_result_hw[len])-final_result_sw[len])>=1e-4){
                printf("Error found final_result_hw[%d]!=final_result_sw[%d], %d!=%d \n", len,len,final_result_hw[len],final_result_sw[len]);
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