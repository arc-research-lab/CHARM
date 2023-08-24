
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
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
mm_x16_x3_x6_graph0 mm_graph0;
using namespace std;

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
const int A0=16;
const int B0=3;
const int C0=6;
const int X0=2;
const int Y0=1;
const int Z0=5;
const int M_ACC0=H1_0*A0*X0;
const int K_ACC0=W1_0*B0*Y0;
const int N_ACC0=W2_0*C0*Z0;

const int NUM_LARYER=4;

const int layer_in[NUM_LARYER][4] =
{
    {3072,2048,4096,1},
    {3072,4096,4096,1},
    {3072,4096,4096,1},
    {3072,4096,1024,1}
};


int main(int argc, char** argv) {

    int iter=500,verify=0;
    char* xclbinFilename;
    if(argc == 4) {
        xclbinFilename = argv[1];
        if (sscanf (argv[2], "%i", &iter) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[3], "%i", &verify) != 1) {
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

    
    int layer[NUM_LARYER][4];

    for (int i=0; i<NUM_LARYER; i++){
        for (int j=0; j<4; j++){
            float temp;
            if (j==0){
                temp = (float)(layer_in[i][j])/(float)(M_ACC0);
                layer[i][j]=ceil(temp)*M_ACC0;
            }
            else if(j==1){
                temp = (float)(layer_in[i][j])/(float)(K_ACC0);
                layer[i][j]=ceil(temp)*K_ACC0;
            }
            else if(j==2){
                temp = (float)(layer_in[i][j])/(float)(N_ACC0);
                layer[i][j]=ceil(temp)*N_ACC0;
            }
            else{
                layer[i][j]=layer_in[i][j];
            }
        }
    }

    //layer0
    std::vector<std::vector<int16_t>>layer0_in0(layer[0][0],std::vector<int16_t>(layer[0][1],1));
    std::vector<std::vector<int16_t>>layer0_in1(layer[0][1],std::vector<int16_t>(layer[0][2],1));
    std::vector<std::vector<int16_t>>layer0_golden(layer[0][0],std::vector<int16_t>(layer[0][2],1));

    //layer1
    std::vector<std::vector<int16_t>>layer1_in0(layer[1][0],std::vector<int16_t>(layer[1][1],1));
    std::vector<std::vector<int16_t>>layer1_in1(layer[1][1],std::vector<int16_t>(layer[1][2],1));
    std::vector<std::vector<int16_t>>layer1_golden(layer[1][0],std::vector<int16_t>(layer[1][2],1));

    //layer2
    std::vector<std::vector<int16_t>>layer2_in0(layer[2][0],std::vector<int16_t>(layer[2][1],1));
    std::vector<std::vector<int16_t>>layer2_in1(layer[2][1],std::vector<int16_t>(layer[2][2],1));
    std::vector<std::vector<int16_t>>layer2_golden(layer[2][0],std::vector<int16_t>(layer[2][2],1));

    //layer3
    std::vector<std::vector<int16_t>>layer3_in0(layer[3][0],std::vector<int16_t>(layer[3][1],1));
    std::vector<std::vector<int16_t>>layer3_in1(layer[3][1],std::vector<int16_t>(layer[3][2],1));
    std::vector<std::vector<int16_t>>layer3_golden(layer[3][0],std::vector<int16_t>(layer[3][2],1));

    //layer0
    srand (time(0));
    for (int m = 0; m < layer[0][0]; m++) {
        for (int k = 0; k < layer[0][1]; k++) {
            layer0_in0[m][k]= (int16_t)rand();
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[0][1]; k++) {
        for (int n = 0; n < layer[0][2]; n++) {
            layer0_in1[k][n]= (int16_t)rand();
        }
    }

    //layer1
    srand (time(0));
    for (int m = 0; m < layer[1][0]; m++) {
        for (int k = 0; k < layer[1][1]; k++) {
            layer1_in0[m][k]= (int16_t)rand();
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[1][1]; k++) {
        for (int n = 0; n < layer[1][2]; n++) {
            layer1_in1[k][n]= (int16_t)rand();
        }
    }

    //layer2
    srand (time(0));
    for (int m = 0; m < layer[2][0]; m++) {
        for (int k = 0; k < layer[2][1]; k++) {
            layer2_in0[m][k]= (int16_t)rand();
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[2][1]; k++) {
        for (int n = 0; n < layer[2][2]; n++) {
            layer2_in1[k][n]= (int16_t)rand();
        }
    }

    //layer3
    srand (time(0));
    for (int m = 0; m < layer[3][0]; m++) {
        for (int k = 0; k < layer[3][1]; k++) {
            layer3_in0[m][k]= (int16_t)rand();
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[3][1]; k++) {
        for (int n = 0; n < layer[3][2]; n++) {
            layer3_in1[k][n]= (int16_t)rand();
        }
    }

    //Allocate input mem
    //layer0
    auto bohdl_layer0_in0 = xrt::bo(device, (layer[0][0]*layer[0][1]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer0_in0 = bohdl_layer0_in0.map<int16_t*>();
    auto bohdl_layer0_in1 = xrt::bo(device, (layer[0][1]*layer[0][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer0_in1 = bohdl_layer0_in1.map<int16_t*>();

    //layer1
    auto bohdl_layer1_in0 = xrt::bo(device, (layer[1][0]*layer[1][1]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer1_in0 = bohdl_layer1_in0.map<int16_t*>();
    auto bohdl_layer1_in1 = xrt::bo(device, (layer[1][1]*layer[1][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer1_in1 = bohdl_layer1_in1.map<int16_t*>();

    //layer2
    auto bohdl_layer2_in0 = xrt::bo(device, (layer[2][0]*layer[2][1]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer2_in0 = bohdl_layer2_in0.map<int16_t*>();
    auto bohdl_layer2_in1 = xrt::bo(device, (layer[2][1]*layer[2][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer2_in1 = bohdl_layer2_in1.map<int16_t*>();

    //layer3
    auto bohdl_layer3_in0 = xrt::bo(device, (layer[3][0]*layer[3][1]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer3_in0 = bohdl_layer3_in0.map<int16_t*>();
    auto bohdl_layer3_in1 = xrt::bo(device, (layer[3][1]*layer[3][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer3_in1 = bohdl_layer3_in1.map<int16_t*>();

    //layer0
    for (int m = 0; m < layer[0][0]; m++) {
        for (int k = 0; k < layer[0][1]; k++) {
            bomapped_layer0_in0[m+k*layer[0][0]]=layer0_in0[m][k];
        }
    }
    for (int k = 0; k < layer[0][1]; k++) {
        for (int n = 0; n < layer[0][2]; n++) {
            bomapped_layer0_in1[k+n*layer[0][1]]=layer0_in1[k][n];
        }
    }

    //layer1
    for (int m = 0; m < layer[1][0]; m++) {
        for (int k = 0; k < layer[1][1]; k++) {
            bomapped_layer1_in0[m+k*layer[1][0]]=layer1_in0[m][k];
        }
    }
    for (int k = 0; k < layer[1][1]; k++) {
        for (int n = 0; n < layer[1][2]; n++) {
            bomapped_layer1_in1[k+n*layer[1][1]]=layer1_in1[k][n];
        }
    }

    //layer2
    for (int m = 0; m < layer[2][0]; m++) {
        for (int k = 0; k < layer[2][1]; k++) {
            bomapped_layer2_in0[m+k*layer[2][0]]=layer2_in0[m][k];
        }
    }
    for (int k = 0; k < layer[2][1]; k++) {
        for (int n = 0; n < layer[2][2]; n++) {
            bomapped_layer2_in1[k+n*layer[2][1]]=layer2_in1[k][n];
        }
    }

    //layer3
    for (int m = 0; m < layer[3][0]; m++) {
        for (int k = 0; k < layer[3][1]; k++) {
            bomapped_layer3_in0[m+k*layer[3][0]]=layer3_in0[m][k];
        }
    }
    for (int k = 0; k < layer[3][1]; k++) {
        for (int n = 0; n < layer[3][2]; n++) {
            bomapped_layer3_in1[k+n*layer[3][1]]=layer3_in1[k][n];
        }
    }

    // sync input memory
    //layer0
    bohdl_layer0_in0.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[0][0]*layer[0][1])* sizeof(int16_t),0);
    bohdl_layer0_in1.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[0][1]*layer[0][2])* sizeof(int16_t),0);

    //layer1
    bohdl_layer1_in0.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[1][0]*layer[1][1])* sizeof(int16_t),0);
    bohdl_layer1_in1.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[1][1]*layer[1][2])* sizeof(int16_t),0);

    //layer2
    bohdl_layer2_in0.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[2][0]*layer[2][1])* sizeof(int16_t),0);
    bohdl_layer2_in1.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[2][1]*layer[2][2])* sizeof(int16_t),0);

    //layer3
    bohdl_layer3_in0.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[3][0]*layer[3][1])* sizeof(int16_t),0);
    bohdl_layer3_in1.sync(XCL_BO_SYNC_BO_TO_DEVICE , (layer[3][1]*layer[3][2])* sizeof(int16_t),0);
    
    ////////////////////////////// Output handler//////////////////////////////
    //layer0
    auto bohdl_layer0_out = xrt::bo(device, (layer[0][0]*layer[0][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer0_out = bohdl_layer0_out.map<int16_t*>();

    //layer1
    auto bohdl_layer1_out = xrt::bo(device, (layer[1][0]*layer[1][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer1_out = bohdl_layer1_out.map<int16_t*>();

    //layer2
    auto bohdl_layer2_out = xrt::bo(device, (layer[2][0]*layer[2][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer2_out = bohdl_layer2_out.map<int16_t*>();

    //layer3
    auto bohdl_layer3_out = xrt::bo(device, (layer[3][0]*layer[3][2]) * sizeof(int16_t), dma0.group_id(0));
    auto bomapped_layer3_out = bohdl_layer3_out.map<int16_t*>();
    
    xrt::run dma_run;
    auto ghdl0=xrt::graph(device,uuid,"mm_graph0");
    printf("graph run\n");
    ghdl0.run(-1);
    
    std::cout << "Kernel run\n";
    
    //profile aie mm 
    std::vector<double>kernel_time_in_sec(NUM_LARYER,0.0);
    std::chrono::duration<double> kernel_time0(0);
    std::chrono::duration<double> kernel_time1(0);
    std::chrono::duration<double> kernel_time2(0);
    std::chrono::duration<double> kernel_time3(0);

    for (int i=0;i<iter;i++){
    // start input kernels run handles

        auto kernel_start0 = std::chrono::high_resolution_clock::now();
        dma_run = dma0(bohdl_layer0_in0, bohdl_layer0_in1,bohdl_layer0_out,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                (layer[0][0]/(M_ACC0)),(layer[0][1]/K_ACC0),(layer[0][2]/N_ACC0));
        dma_run.wait();
        auto kernel_end0 = std::chrono::high_resolution_clock::now();
        kernel_time0 = std::chrono::duration<double>(kernel_end0 - kernel_start0);
        kernel_time_in_sec[0] += kernel_time0.count();

        auto kernel_start1 = std::chrono::high_resolution_clock::now();
        dma_run = dma0(bohdl_layer1_in0, bohdl_layer1_in1,bohdl_layer1_out,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                (layer[1][0]/(M_ACC0)),(layer[1][1]/K_ACC0),(layer[1][2]/N_ACC0));
        dma_run.wait();
        auto kernel_end1 = std::chrono::high_resolution_clock::now();
        kernel_time1 = std::chrono::duration<double>(kernel_end1 - kernel_start1);
        kernel_time_in_sec[1] += kernel_time1.count();

        auto kernel_start2 = std::chrono::high_resolution_clock::now();
        dma_run = dma0(bohdl_layer2_in0, bohdl_layer2_in1,bohdl_layer2_out,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                (layer[2][0]/(M_ACC0)),(layer[2][1]/K_ACC0),(layer[2][2]/N_ACC0));
        dma_run.wait();
        auto kernel_end2 = std::chrono::high_resolution_clock::now();
        kernel_time2 = std::chrono::duration<double>(kernel_end2 - kernel_start2);
        kernel_time_in_sec[2] += kernel_time2.count();

        auto kernel_start3 = std::chrono::high_resolution_clock::now();
        dma_run = dma0(bohdl_layer3_in0, bohdl_layer3_in1,bohdl_layer3_out,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr,
                (layer[3][0]/(M_ACC0)),(layer[3][1]/K_ACC0),(layer[3][2]/N_ACC0));
        dma_run.wait();
        auto kernel_end3 = std::chrono::high_resolution_clock::now();
        kernel_time3 = std::chrono::duration<double>(kernel_end3 - kernel_start3);
        kernel_time_in_sec[3] += kernel_time3.count();

    }
    std::vector<double>TOPS(NUM_LARYER,0.0);
    std::vector<double>OPS(NUM_LARYER,0.0);
    
    for (int i=0;i<NUM_LARYER;i++){
        OPS[i]=(double)layer_in[i][0]*(double)layer_in[i][1]*(double)layer_in[i][2]*(double)layer_in[i][3]*2*(1e-9);
    }

    for (int i=0;i<NUM_LARYER;i++){
        std::cout << "The Ops of layer" << i <<" is : " << OPS[i] << "GOP" << std::endl;
    }
    
    for (int i=0;i<NUM_LARYER;i++){
        kernel_time_in_sec[i]=kernel_time_in_sec[i];
    }

    for (int i=0;i<NUM_LARYER;i++){
        TOPS[i]=OPS[i]*iter/kernel_time_in_sec[i];
    }

    

    double TOPS_total=0;
    for (int i=0;i<NUM_LARYER;i++){
        TOPS_total+=OPS[i];
    }

    double Time_total=0;
    for (int i=0;i<NUM_LARYER;i++){
        Time_total+=kernel_time_in_sec[i];
    }
 
    double TOPS_Overall= (TOPS_total)*iter/Time_total;
    std::cout << std::endl;
    std::cout << std::endl;
    
    for (int i=0;i<NUM_LARYER;i++){
        std::cout << "The time of each layer" << i <<" is : " << kernel_time_in_sec[i] <<"s, TOPS_ACC" << i <<" = " << TOPS[i] << " GOPS/s" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Total time is: "<< Time_total <<"s, TOPS_Overall = " << TOPS_Overall << " GOPS/s" << std::endl;

    // sync output memory
    bohdl_layer0_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE, (layer[0][0]*layer[0][2])* sizeof(int16_t),/*OFFSET=*/ 0);
    bohdl_layer1_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE, (layer[1][0]*layer[1][2])* sizeof(int16_t),/*OFFSET=*/ 0);
    bohdl_layer2_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE, (layer[2][0]*layer[2][2])* sizeof(int16_t),/*OFFSET=*/ 0);
    bohdl_layer3_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE, (layer[3][0]*layer[3][2])* sizeof(int16_t),/*OFFSET=*/ 0);
        
    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if (verify)

    {
        int16_t sum = 0;
        for (int m = 0; m < layer[0][0]; m++) {
            for (int n = 0; n < layer[0][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[0][1]; k++) {
                    sum=sum+layer0_in0[m][k]*layer0_in1[k][n];
                }
                layer0_golden[m][n]=sum;
            }
        }

        int errorCount0 = 0;
        for (int m = 0; m < layer[0][0]; m++) {
            for (int n = 0; n < layer[0][2]; n++) {
                if(abs((int16_t)(bomapped_layer0_out[m+n*layer[0][0]])-layer0_golden[m][n])>=1e-3){
                    errorCount0++;
                }
            }
        }
        if (!errorCount0)
            printf("Layer0 Passed\n");
        else
            printf("Layer0 Failed\n");

        int errorCount1 = 0;
        for (int m = 0; m < layer[1][0]; m++) {
            for (int n = 0; n < layer[1][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[1][1]; k++) {
                    sum=sum+layer1_in0[m][k]*layer1_in1[k][n];
                }
                layer1_golden[m][n]=sum;
            }
        }

        for (int m = 0; m < layer[1][0]; m++) {
            for (int n = 0; n < layer[1][2]; n++) {
                if(abs((int16_t)(bomapped_layer1_out[m+n*layer[1][0]])-layer1_golden[m][n])>=1e-3){
                    errorCount1++;
                }
            }
        }
        if (!errorCount1)
            printf("Layer1 Passed\n");
        else
            printf("Layer1 Failed\n");

        int errorCount2 = 0;
        for (int m = 0; m < layer[2][0]; m++) {
            for (int n = 0; n < layer[2][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[2][1]; k++) {
                    sum=sum+layer2_in0[m][k]*layer2_in1[k][n];
                }
                layer2_golden[m][n]=sum;
            }
        }

        for (int m = 0; m < layer[2][0]; m++) {
            for (int n = 0; n < layer[2][2]; n++) {
                if(abs((int16_t)(bomapped_layer2_out[m+n*layer[2][0]])-layer2_golden[m][n])>=1e-3){
                    errorCount2++;
                }
            }
        }
        if (!errorCount2)
            printf("Layer2 Passed\n");
        else
            printf("Layer2 Failed\n");

        int errorCount3 = 0;
        for (int m = 0; m < layer[3][0]; m++) {
            for (int n = 0; n < layer[3][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[3][1]; k++) {
                    sum=sum+layer3_in0[m][k]*layer3_in1[k][n];
                }
                layer3_golden[m][n]=sum;
            }
        }

         for (int m = 0; m < layer[3][0]; m++) {
            for (int n = 0; n < layer[3][2]; n++) {
                if(abs((int16_t)(bomapped_layer3_out[m+n*layer[3][0]])-layer3_golden[m][n])>=1e-3){
                    errorCount3++;
                }
            }
        }
        if (!errorCount3)
            printf("Layer3 Passed\n");
        else
            printf("Layer3 Failed\n");
        
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << "Releasing remaining XRT objects...\n";
    
    return 0;
}
