
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
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
mm_x2_x3_x4_graph0 mm_graph0;
mm_x2_x3_x4_graph1 mm_graph1;
mm_x2_x3_x4_graph2 mm_graph2;
mm_x2_x3_x4_graph3 mm_graph3;
mm_x2_x3_x4_graph4 mm_graph4;
mm_x2_x3_x4_graph5 mm_graph5;
mm_x2_x3_x4_graph6 mm_graph6;
mm_x2_x3_x4_graph7 mm_graph7;
using namespace std;

static std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm) {
    if (fnm.empty()) throw std::runtime_error("No xclbin specified");

    // load bit stream
    std::ifstream stream(fnm);
    stream.seekg(0, stream.end);
    size_t size = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<char> header(size);
    stream.read(header.data(), size);

    auto top = reinterpret_cast<const axlf*>(header.data());
    if (xrtDeviceLoadXclbin(device, top)) throw std::runtime_error("Xclbin loading failed");

    return header;
}

void post_pro(int16_t *Data_hw, std::vector<int16_t> final_result, const int M1, const int N1, const int M){
    for (int n = 0; n < N1; n++) {
        for (int m = 0; m < M1; m++) { 
            int pos0=m+n*M;
            int pos1=m+n*M1;
            final_result[pos1]=Data_hw[pos0];
        } 
    }
}

void sw_mm(std::vector<int16_t> DataInput0,std::vector<int16_t> DataInput1,int16_t *golden,const int M,const int K,const int N){
    int16_t sum = 0;
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

const int H1_0=48;
const int W1_0=32;
const int W2_0=48;
const int A0=2;
const int B0=3;
const int C0=4;
const int X0=4;
const int Y0=1;
const int Z0=3;
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
    
    auto dhdl = xrtDeviceOpen(0); // Open Device the local device
    if (dhdl == nullptr) throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin = load_xclbin(dhdl, xclbinFilename);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    
    adf::registerXRT(dhdl, top->m_header.uuid);
    
    
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
    
    std::vector<int16_t> DataInput0(sizeIn1,1);
    std::vector<int16_t> DataInput1(sizeIn2,1);
    int16_t* golden = new int16_t[sizeOut];
    std::vector<int16_t> final_result_sw(sizeOut1,1);
    std::vector<int16_t> final_result_hw(sizeOut1,1);

    srand (time(0));
    for (int k = 0; k < K; k++) {
        for (int m = 0; m < M; m++) {
            int pos = m + k*M;
            if (m>=M1||k>=K1){
                DataInput0[pos]= 0;
            }
            else{
                DataInput0[pos]= (int16_t)(rand());
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
                DataInput1[pos]= (int16_t)(rand());
            }
            
        }
    }

    //Allocate input mem
    xrtBufferHandle in_bohdl0 = xrtBOAlloc(dhdl, sizeIn1 * sizeof(int16_t), 0, 0);
    auto in_bomapped0 = reinterpret_cast<int16_t*>(xrtBOMap(in_bohdl0));
    for (int k = 0; k < sizeIn1; k++) {
        in_bomapped0 [k]= DataInput0[k];
    } 

    xrtBufferHandle in_bohdl1 = xrtBOAlloc(dhdl, sizeIn2 * sizeof(int16_t), 0, 0);
    auto in_bomapped1 = reinterpret_cast<int16_t*>(xrtBOMap(in_bohdl1));
    for (int k = 0; k < sizeIn2; k++) {
        in_bomapped1 [k]= DataInput1[k];
    } 

    // sync input memory
    xrtBOSync(in_bohdl0, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn1* sizeof(int16_t),0);
    xrtBOSync(in_bohdl1, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn2* sizeof(int16_t),0);
    
    //Allocate output buffer
    //layer0
    xrtBufferHandle out_bohdl0 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped0 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl0));

    //layer1
    xrtBufferHandle out_bohdl1 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped1 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl1));

    //layer2
    xrtBufferHandle out_bohdl2 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped2 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl2));

    //layer3
    xrtBufferHandle out_bohdl3 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped3 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl3));

    //layer4
    xrtBufferHandle out_bohdl4 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped4 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl4));

    //layer5
    xrtBufferHandle out_bohdl5 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped5 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl5));

    //layer6
    xrtBufferHandle out_bohdl6 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped6 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl6));

    //layer7
    xrtBufferHandle out_bohdl7 = xrtBOAlloc(dhdl, sizeOut * sizeof(int16_t), 0, 0);
    auto out_bomapped7 = reinterpret_cast<int16_t*>(xrtBOMap(out_bohdl7));
    
    
    mm_graph0.init();
    mm_graph1.init();
    mm_graph2.init();
    mm_graph3.init();
    mm_graph4.init();
    mm_graph5.init();
    mm_graph6.init();
    mm_graph7.init();
    printf("graph run\n");
    mm_graph0.run(-1);
    mm_graph1.run(-1);
    mm_graph2.run(-1);
    mm_graph3.run(-1);
    mm_graph4.run(-1);
    mm_graph5.run(-1);
    mm_graph6.run(-1);
    mm_graph7.run(-1);
    

    std::cout << "Kernel run\n";
    xrtKernelHandle dma_khdl0 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma0");
    xrtKernelHandle dma_khdl1 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma1");
    xrtKernelHandle dma_khdl2 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma2");
    xrtKernelHandle dma_khdl3 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma3");
    xrtKernelHandle dma_khdl4 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma4");
    xrtKernelHandle dma_khdl5 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma5");
    xrtKernelHandle dma_khdl6 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma6");
    xrtKernelHandle dma_khdl7 = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma7");
    xrtRunHandle dma_rhdl0;
    xrtRunHandle dma_rhdl1;
    xrtRunHandle dma_rhdl2;
    xrtRunHandle dma_rhdl3;
    xrtRunHandle dma_rhdl4;
    xrtRunHandle dma_rhdl5;
    xrtRunHandle dma_rhdl6;
    xrtRunHandle dma_rhdl7;
    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    for (int i=0;i<iter;i++){
        // start input kernels run handles
        dma_rhdl0 = xrtKernelRun(dma_khdl0, in_bohdl0, in_bohdl1,out_bohdl0,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl1 = xrtKernelRun(dma_khdl1, in_bohdl0, in_bohdl1,out_bohdl1,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl2 = xrtKernelRun(dma_khdl2, in_bohdl0, in_bohdl1,out_bohdl2,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl3 = xrtKernelRun(dma_khdl3, in_bohdl0, in_bohdl1,out_bohdl3,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl4 = xrtKernelRun(dma_khdl4, in_bohdl0, in_bohdl1,out_bohdl4,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl5 = xrtKernelRun(dma_khdl5, in_bohdl0, in_bohdl1,out_bohdl5,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl6 = xrtKernelRun(dma_khdl6, in_bohdl0, in_bohdl1,out_bohdl6,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        dma_rhdl7 = xrtKernelRun(dma_khdl7, in_bohdl0, in_bohdl1,out_bohdl7,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                TX0,TY0,TZ0);
        

        xrtRunWait(dma_rhdl0);
        xrtRunWait(dma_rhdl1);
        xrtRunWait(dma_rhdl2);
        xrtRunWait(dma_rhdl3);
        xrtRunWait(dma_rhdl4);
        xrtRunWait(dma_rhdl5);
        xrtRunWait(dma_rhdl6);
        xrtRunWait(dma_rhdl7);
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
    xrtBOSync(out_bohdl0, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl1, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl2, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl3, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl4, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl5, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl6, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);
    xrtBOSync(out_bohdl7, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(int16_t),/*OFFSET=*/ 0);

    
    xrtRunClose(dma_rhdl0);
    xrtRunClose(dma_rhdl1);
    xrtRunClose(dma_rhdl2);
    xrtRunClose(dma_rhdl3);
    xrtRunClose(dma_rhdl4);
    xrtRunClose(dma_rhdl5);
    xrtRunClose(dma_rhdl6);
    xrtRunClose(dma_rhdl7);
    xrtKernelClose(dma_khdl0);
    xrtKernelClose(dma_khdl1);
    xrtKernelClose(dma_khdl2);
    xrtKernelClose(dma_khdl3);
    xrtKernelClose(dma_khdl4);
    xrtKernelClose(dma_khdl5);
    xrtKernelClose(dma_khdl6);
    xrtKernelClose(dma_khdl7);
    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        
        post_pro(out_bomapped0, final_result_hw, M1, N1, M);
        sw_mm(DataInput0,DataInput1,golden,M,K,N);
        post_pro(golden, final_result_sw, M1, N1, M);
        int errorCount = 0;  
        for (int len = 0; len < sizeOut1; len++) {

            if(abs((int16_t)(final_result_hw[len])-final_result_sw[len])>=1e-4){
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
    
    xrtBOFree(out_bohdl0);
    xrtBOFree(out_bohdl1);
    xrtBOFree(out_bohdl2);
    xrtBOFree(out_bohdl3);
    xrtBOFree(out_bohdl4);
    xrtBOFree(out_bohdl5);
    xrtBOFree(out_bohdl6);
    xrtBOFree(out_bohdl7);
    xrtBOFree(in_bohdl0);
    xrtBOFree(in_bohdl1);

    mm_graph0.wait();
    mm_graph1.wait();
    mm_graph2.wait();
    mm_graph3.wait();
    mm_graph4.wait();
    mm_graph5.wait();
    mm_graph6.wait();
    mm_graph7.wait();
    xrtDeviceClose(dhdl);
    return 0;
}
