
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>
#include "../aie/layer0/aie_top_L0.h"

// This is used for the PL Kernels
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
mm_x{{A}}_x{{B}}_x{{C}}_graph0 mm_graph0;
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

void sw_mm(std::vector<float> DataInput0,std::vector<float> DataInput1,std::vector<float> &golden,const int M,const int K,const int N){
    float sum = 0;
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

const int H1={{h1}};
const int W1={{w1}};
const int W2={{w2}};
const int A={{A}};
const int B={{B}};
const int C={{C}};
const int X={{X}};
const int Y={{Y}};
const int Z={{Z}};

int main(int argc, char** argv) {

    int TX,TY,TZ;
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
    
    std::vector<float> DataInput0(sizeIn1,1);
    std::vector<float> DataInput1(sizeIn2,1);
    std::vector<float> golden(sizeOut,1);

    srand (time(0));
    for (int k = 0; k < sizeIn1; k++) {
        DataInput0[k]= (float)(rand()*1.0);
    } 

    for (int k = 0; k < sizeIn2; k++) {
        DataInput1[k]= (float)(rand()*1.0);
    }

    //Allocate input mem
    xrtBufferHandle in_bohdl0 = xrtBOAlloc(dhdl, sizeIn1 * sizeof(float), 0, 0);
    auto in_bomapped0 = reinterpret_cast<float*>(xrtBOMap(in_bohdl0));
    for (int k = 0; k < sizeIn1; k++) {
        in_bomapped0 [k]= DataInput0[k];
    } 

    xrtBufferHandle in_bohdl1 = xrtBOAlloc(dhdl, sizeIn2 * sizeof(float), 0, 0);
    auto in_bomapped1 = reinterpret_cast<float*>(xrtBOMap(in_bohdl1));
    for (int k = 0; k < sizeIn2; k++) {
        in_bomapped1 [k]= DataInput1[k];
    } 

    // sync input memory
    xrtBOSync(in_bohdl0, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn1* sizeof(float),0);
    xrtBOSync(in_bohdl1, XCL_BO_SYNC_BO_TO_DEVICE , sizeIn2* sizeof(float),0);
    
    //Allocate output buffer
    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, sizeOut* sizeof(float), 0, 0);
    auto out_bomapped = reinterpret_cast<float*>(xrtBOMap(out_bohdl));
    memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(float));
    
    //int graph_iter=X*Y*Z*TX*TY*TZ;
    vit_graph0.init();
           
    printf("graph run\n");
    vit_graph0.run(-1);

    std::cout << "Kernel run\n";
    xrtKernelHandle dma_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma");
    xrtRunHandle dma_rhdl;
    
    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    for (int i=0;i<iter;i++){
    // start input kernels run handles
    dma_rhdl = xrtKernelRun(dma_khdl, in_bohdl0, in_bohdl1,out_bohdl,
            {% for k in range(A) -%}
            {% for j in range(NUM_TXL) -%}
            {% for i in range(B) -%} nullptr, {% endfor%}
            {% endfor-%}
            {% endfor-%}

            {% for k in range(C) -%}
            {% for j in range(NUM_TXR) -%}
            {% for i in range(B) -%}  nullptr, {% endfor%}
            {% endfor-%}
            {% endfor-%}

            {% for j in range(A) -%}
            {% for i in range(C) -%} nullptr, {% endfor%}
            {% endfor-%}
            TX,TY,TZ);
        xrtRunWait(dma_rhdl);
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
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE , sizeOut* sizeof(float),/*OFFSET=*/ 0);
        
    xrtRunClose(dma_rhdl);
    xrtKernelClose(dma_khdl);

    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        
        sw_mm(DataInput0,DataInput1,golden,M,K,N);
    
        int errorCount = 0;  
        for (int len = 0; len < sizeOut; len++) {

            if(abs((float)(out_bomapped[len])-golden[len])>=1e-4){
                printf("Error found out_bomapped[%d]!=golden[%d], %f!=%f \n", len,len,out_bomapped[len],golden[len]);
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
    
    xrtBOFree(out_bohdl);
    xrtBOFree(in_bohdl0);
    xrtBOFree(in_bohdl1);
    xrtDeviceClose(dhdl);
    return 0;
}

