/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#include <iostream>
#include "../aie/mm_top.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>

#include <vector>
#include <math.h>
#include <string>

// This is used for the PL Kernels
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
mm_x64_x4_graph<0,0> myGraph;
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

const int PACKET_NUM=4; 
const int H1=32;
const int W1=32;
const int W2=32;
const int A=4;
const int B=4;
const int C=16;
const int X=4;
const int Y=2;
const int Z=1;

const int M_acc=H1*A*X;
const int K_acc=W1*B*Y;
const int N_acc=W2*C*Z;

const int NUM_LARYER=8;

const int D0=6;
const int D1=16;
const int batch_size=D0*D1;

const int layer_in[NUM_LARYER][4] =
{
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,4096,1},
    {3072,4096,1024,1},
    {512,512,64,batch_size},
    {512,64,512,batch_size}
};

const int layer[NUM_LARYER][4] =
{
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,4096,1},
    {3072,4096,1024,1},
    {512,512,512,batch_size},
    {512,256,512,batch_size}
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
    
    auto dhdl = xrtDeviceOpen(0); // Open Device the local device
    if (dhdl == nullptr) throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin = load_xclbin(dhdl, xclbinFilename);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);

    //layer0
    std::vector<std::vector<float>>layer0_in0(layer[0][0],std::vector<float>(layer[0][1],1.0));
    std::vector<std::vector<float>>layer0_in1(layer[0][1],std::vector<float>(layer[0][2],1.0));
    std::vector<std::vector<float>>layer0_golden(layer[0][0],std::vector<float>(layer[0][2],1.0));

    //layer1
    std::vector<std::vector<float>>layer1_in0(layer[1][0],std::vector<float>(layer[1][1],1.0));
    std::vector<std::vector<float>>layer1_in1(layer[1][1],std::vector<float>(layer[1][2],1.0));
    std::vector<std::vector<float>>layer1_golden(layer[1][0],std::vector<float>(layer[1][2],1.0));

    //layer2
    std::vector<std::vector<float>>layer2_in0(layer[2][0],std::vector<float>(layer[2][1],1.0));
    std::vector<std::vector<float>>layer2_in1(layer[2][1],std::vector<float>(layer[2][2],1.0));
    std::vector<std::vector<float>>layer2_golden(layer[2][0],std::vector<float>(layer[2][2],1.0));

    //layer3
    std::vector<std::vector<float>>layer3_in0(layer[3][0],std::vector<float>(layer[3][1],1.0));
    std::vector<std::vector<float>>layer3_in1(layer[3][1],std::vector<float>(layer[3][2],1.0));
    std::vector<std::vector<float>>layer3_golden(layer[3][0],std::vector<float>(layer[3][2],1.0));

    //layer4
    std::vector<std::vector<float>>layer4_in0(layer[4][0],std::vector<float>(layer[4][1],1.0));
    std::vector<std::vector<float>>layer4_in1(layer[4][1],std::vector<float>(layer[4][2],1.0));
    std::vector<std::vector<float>>layer4_golden(layer[4][0],std::vector<float>(layer[4][2],1.0));

    //layer5
    std::vector<std::vector<float>>layer5_in0(layer[5][0],std::vector<float>(layer[5][1],1.0));
    std::vector<std::vector<float>>layer5_in1(layer[5][1],std::vector<float>(layer[5][2],1.0));
    std::vector<std::vector<float>>layer5_golden(layer[5][0],std::vector<float>(layer[5][2],1.0));

    //layer6
    std::vector<std::vector<float>>layer6_in0(layer[6][0],std::vector<float>(layer[6][1],1.0));
    std::vector<std::vector<float>>layer6_in1(layer[6][1],std::vector<float>(layer[6][2],1.0));
    std::vector<std::vector<float>>layer6_golden(layer[6][0],std::vector<float>(layer[6][2],1.0));

    //layer7
    std::vector<std::vector<float>>layer7_in0(layer[7][0],std::vector<float>(layer[7][1],1.0));
    std::vector<std::vector<float>>layer7_in1(layer[7][1],std::vector<float>(layer[7][2],1.0));
    std::vector<std::vector<float>>layer7_golden(layer[7][0],std::vector<float>(layer[7][2],1.0));



    //layer0
    srand (time(0));
    for (int m = 0; m < layer[0][0]; m++) {
        for (int k = 0; k < layer[0][1]; k++) {
            layer0_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[0][1]; k++) {
        for (int n = 0; n < layer[0][2]; n++) {
            layer0_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer1
    srand (time(0));
    for (int m = 0; m < layer[1][0]; m++) {
        for (int k = 0; k < layer[1][1]; k++) {
            layer1_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[1][1]; k++) {
        for (int n = 0; n < layer[1][2]; n++) {
            layer1_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer2
    srand (time(0));
    for (int m = 0; m < layer[2][0]; m++) {
        for (int k = 0; k < layer[2][1]; k++) {
            layer2_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[2][1]; k++) {
        for (int n = 0; n < layer[2][2]; n++) {
            layer2_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer3
    srand (time(0));
    for (int m = 0; m < layer[3][0]; m++) {
        for (int k = 0; k < layer[3][1]; k++) {
            layer3_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[3][1]; k++) {
        for (int n = 0; n < layer[3][2]; n++) {
            layer3_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer4
    srand (time(0));
    for (int m = 0; m < layer[4][0]; m++) {
        for (int k = 0; k < layer[4][1]; k++) {
            layer4_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[4][1]; k++) {
        for (int n = 0; n < layer[4][2]; n++) {
            layer4_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer5
    srand (time(0));
    for (int m = 0; m < layer[5][0]; m++) {
        for (int k = 0; k < layer[5][1]; k++) {
            layer5_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[5][1]; k++) {
        for (int n = 0; n < layer[5][2]; n++) {
            layer5_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer6
    srand (time(0));
    for (int m = 0; m < layer[6][0]; m++) {
        for (int k = 0; k < layer[6][1]; k++) {
            layer6_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[6][1]; k++) {
        for (int n = 0; n < layer[6][2]; n++) {
            layer6_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }

    //layer7
    srand (time(0));
    for (int m = 0; m < layer[7][0]; m++) {
        for (int k = 0; k < layer[7][1]; k++) {
            layer7_in0[m][k]= (rand()%5)*(float)1.0;
        }
    }
    srand (time(0));
    for (int k = 0; k < layer[7][1]; k++) {
        for (int n = 0; n < layer[7][2]; n++) {
            layer7_in1[k][n]= (rand()%5)*(float)1.0;
        }
    }
    
    //layer0
    xrtBufferHandle bohdl_layer0_in0 = xrtBOAlloc(dhdl, (layer[0][0]*layer[0][1]) * sizeof(float), 0, 0);
    auto bomapped_layer0_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer0_in0));
    xrtBufferHandle bohdl_layer0_in1 = xrtBOAlloc(dhdl, (layer[0][1]*layer[0][2]) * sizeof(float), 0, 0);
    auto bomapped_layer0_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer0_in1));

    //layer1
    xrtBufferHandle bohdl_layer1_in0 = xrtBOAlloc(dhdl, (layer[1][0]*layer[1][1]) * sizeof(float), 0, 0);
    auto bomapped_layer1_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer1_in0));
    xrtBufferHandle bohdl_layer1_in1 = xrtBOAlloc(dhdl, (layer[1][1]*layer[1][2]) * sizeof(float), 0, 0);
    auto bomapped_layer1_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer1_in1));

    //layer2
    xrtBufferHandle bohdl_layer2_in0 = xrtBOAlloc(dhdl, (layer[2][0]*layer[2][1]) * sizeof(float), 0, 0);
    auto bomapped_layer2_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer2_in0));
    xrtBufferHandle bohdl_layer2_in1 = xrtBOAlloc(dhdl, (layer[2][1]*layer[2][2]) * sizeof(float), 0, 0);
    auto bomapped_layer2_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer2_in1));

    //layer3
    xrtBufferHandle bohdl_layer3_in0 = xrtBOAlloc(dhdl, (layer[3][0]*layer[3][1]) * sizeof(float), 0, 0);
    auto bomapped_layer3_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer3_in0));
    xrtBufferHandle bohdl_layer3_in1 = xrtBOAlloc(dhdl, (layer[3][1]*layer[3][2]) * sizeof(float), 0, 0);
    auto bomapped_layer3_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer3_in1));

    //layer4
    xrtBufferHandle bohdl_layer4_in0 = xrtBOAlloc(dhdl, (layer[4][0]*layer[4][1]) * sizeof(float), 0, 0);
    auto bomapped_layer4_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer4_in0));
    xrtBufferHandle bohdl_layer4_in1 = xrtBOAlloc(dhdl, (layer[4][1]*layer[4][2]) * sizeof(float), 0, 0);
    auto bomapped_layer4_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer4_in1));

    //layer5
    xrtBufferHandle bohdl_layer5_in0 = xrtBOAlloc(dhdl, (layer[5][0]*layer[5][1]) * sizeof(float), 0, 0);
    auto bomapped_layer5_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_in0));
    xrtBufferHandle bohdl_layer5_in1 = xrtBOAlloc(dhdl, (layer[5][1]*layer[5][2]) * sizeof(float), 0, 0);
    auto bomapped_layer5_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_in1));

    //layer6
    xrtBufferHandle bohdl_layer6_in0 = xrtBOAlloc(dhdl, (layer[6][0]*layer[6][1]) * sizeof(float), 0, 0);
    auto bomapped_layer6_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_in0));
    xrtBufferHandle bohdl_layer6_in1 = xrtBOAlloc(dhdl, (layer[6][1]*layer[6][2]) * sizeof(float), 0, 0);
    auto bomapped_layer6_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_in1));

    //layer7
    xrtBufferHandle bohdl_layer7_in0 = xrtBOAlloc(dhdl, (layer[7][0]*layer[7][1]) * sizeof(float), 0, 0);
    auto bomapped_layer7_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer7_in0));
    xrtBufferHandle bohdl_layer7_in1 = xrtBOAlloc(dhdl, (layer[7][1]*layer[7][2]) * sizeof(float), 0, 0);
    auto bomapped_layer7_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer7_in1));
    
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

    //layer4
    for (int m = 0; m < layer[4][0]; m++) {
        for (int k = 0; k < layer[4][1]; k++) {
            bomapped_layer4_in0[m+k*layer[4][0]]=layer4_in0[m][k];
        }
    }
    for (int k = 0; k < layer[4][1]; k++) {
        for (int n = 0; n < layer[4][2]; n++) {
            bomapped_layer4_in1[k+n*layer[4][1]]=layer4_in1[k][n];
        }
    }

    //layer5
    for (int m = 0; m < layer[5][0]; m++) {
        for (int k = 0; k < layer[5][1]; k++) {
            bomapped_layer5_in0[m+k*layer[5][0]]=layer5_in0[m][k];
        }
    }
    for (int k = 0; k < layer[5][1]; k++) {
        for (int n = 0; n < layer[5][2]; n++) {
            bomapped_layer5_in1[k+n*layer[5][1]]=layer5_in1[k][n];
        }
    }

    //layer6
    for (int m = 0; m < layer[6][0]; m++) {
        for (int k = 0; k < layer[6][1]; k++) {
            bomapped_layer6_in0[m+k*layer[6][0]]=layer6_in0[m][k];
        }
    }
    for (int k = 0; k < layer[6][1]; k++) {
        for (int n = 0; n < layer[6][2]; n++) {
            bomapped_layer6_in1[k+n*layer[6][1]]=layer6_in1[k][n];
        }
    }

    //layer7
    for (int m = 0; m < layer[7][0]; m++) {
        for (int k = 0; k < layer[7][1]; k++) {
            bomapped_layer7_in0[m+k*layer[7][0]]=layer7_in0[m][k];
        }
    }
    for (int k = 0; k < layer[7][1]; k++) {
        for (int n = 0; n < layer[7][2]; n++) {
            bomapped_layer7_in1[k+n*layer[7][1]]=layer7_in1[k][n];
        }
    }
    
    

    ////////////////////////////// Input Sync//////////////////////////////
    //layer0
    xrtBOSync(bohdl_layer0_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[0][0]*layer[0][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer0_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[0][1]*layer[0][2])* sizeof(float),0);

    //layer1
    xrtBOSync(bohdl_layer1_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[1][0]*layer[1][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer1_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[1][1]*layer[1][2])* sizeof(float),0);

    //layer2
    xrtBOSync(bohdl_layer2_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[2][0]*layer[2][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer2_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[2][1]*layer[2][2])* sizeof(float),0);

    //layer3
    xrtBOSync(bohdl_layer3_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[3][0]*layer[3][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer3_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[3][1]*layer[3][2])* sizeof(float),0);

    //layer4
    xrtBOSync(bohdl_layer4_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[4][0]*layer[4][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer4_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[4][1]*layer[4][2])* sizeof(float),0);

    //layer5
    xrtBOSync(bohdl_layer5_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[5][0]*layer[5][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer5_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[5][1]*layer[5][2])* sizeof(float),0);

    //layer6
    xrtBOSync(bohdl_layer6_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[6][0]*layer[6][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer6_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[6][1]*layer[6][2])* sizeof(float),0);

    //layer7
    xrtBOSync(bohdl_layer7_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[7][0]*layer[7][1])* sizeof(float),0);
    xrtBOSync(bohdl_layer7_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[7][1]*layer[7][2])* sizeof(float),0);
    
    ////////////////////////////// Output handler//////////////////////////////
    //layer0
    xrtBufferHandle bohdl_layer0_out = xrtBOAlloc(dhdl, (layer[0][0]*layer[0][2]) * sizeof(float), 0, 0);
    auto bomapped_layer0_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer0_out));

    //layer1
    xrtBufferHandle bohdl_layer1_out = xrtBOAlloc(dhdl, (layer[1][0]*layer[1][2]) * sizeof(float), 0, 0);
    auto bomapped_layer1_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer1_out));

    //layer2
    xrtBufferHandle bohdl_layer2_out = xrtBOAlloc(dhdl, (layer[2][0]*layer[2][2]) * sizeof(float), 0, 0);
    auto bomapped_layer2_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer2_out));

    //layer3
    xrtBufferHandle bohdl_layer3_out = xrtBOAlloc(dhdl, (layer[3][0]*layer[3][2]) * sizeof(float), 0, 0);
    auto bomapped_layer3_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer3_out));

    //layer4
    xrtBufferHandle bohdl_layer4_out = xrtBOAlloc(dhdl, (layer[4][0]*layer[4][2]) * sizeof(float), 0, 0);
    auto bomapped_layer4_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer4_out));

    //layer5
    xrtBufferHandle bohdl_layer5_out = xrtBOAlloc(dhdl, (layer[5][0]*layer[5][2]) * sizeof(float), 0, 0);
    auto bomapped_layer5_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_out));

    //layer6
    xrtBufferHandle bohdl_layer6_out = xrtBOAlloc(dhdl, (layer[6][0]*layer[6][2]) * sizeof(float), 0, 0);
    auto bomapped_layer6_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_out));

    //layer7
    xrtBufferHandle bohdl_layer7_out = xrtBOAlloc(dhdl, (layer[7][0]*layer[7][2]) * sizeof(float), 0, 0);
    auto bomapped_layer7_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer7_out));
    
    myGraph.init();
                              
    printf("graph run\n");
    myGraph.run(-1);


    std::cout << "Kernel run\n";
    xrtKernelHandle dma_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma_large");
    xrtRunHandle dma_rhdl;
    std::vector<double>kernel_time_in_sec(NUM_LARYER,0.0);
    
    std::chrono::duration<double> kernel_time0(0);
    std::chrono::duration<double> kernel_time1(0);
    std::chrono::duration<double> kernel_time2(0);
    std::chrono::duration<double> kernel_time3(0);
    std::chrono::duration<double> kernel_time4(0);
    std::chrono::duration<double> kernel_time5(0);
    std::chrono::duration<double> kernel_time6(0);
    std::chrono::duration<double> kernel_time7(0);

    for (int i=0;i<iter;i++){
    
        auto kernel_start0 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer0_in0, bohdl_layer0_in1,bohdl_layer0_out,(layer[0][0]/(M_acc)),(layer[0][1]/K_acc),(layer[0][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end0 = std::chrono::high_resolution_clock::now();
        kernel_time0 = std::chrono::duration<double>(kernel_end0 - kernel_start0);
        kernel_time_in_sec[0] += kernel_time0.count();

        auto kernel_start1 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer1_in0, bohdl_layer1_in1,bohdl_layer1_out,(layer[1][0]/(M_acc)),(layer[1][1]/K_acc),(layer[1][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end1 = std::chrono::high_resolution_clock::now();
        kernel_time1 = std::chrono::duration<double>(kernel_end1 - kernel_start1);
        kernel_time_in_sec[1] += kernel_time1.count();

        auto kernel_start2 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer2_in0, bohdl_layer2_in1,bohdl_layer2_out,(layer[2][0]/(M_acc)),(layer[2][1]/K_acc),(layer[2][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end2 = std::chrono::high_resolution_clock::now();
        kernel_time2 = std::chrono::duration<double>(kernel_end2 - kernel_start2);
        kernel_time_in_sec[2] += kernel_time2.count();

        auto kernel_start3 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer3_in0, bohdl_layer3_in1,bohdl_layer3_out,(layer[3][0]/(M_acc)),(layer[3][1]/K_acc),(layer[3][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end3 = std::chrono::high_resolution_clock::now();
        kernel_time3 = std::chrono::duration<double>(kernel_end3 - kernel_start3);
        kernel_time_in_sec[3] += kernel_time3.count();

        auto kernel_start4 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer4_in0, bohdl_layer4_in1,bohdl_layer4_out,(layer[4][0]/(M_acc)),(layer[4][1]/K_acc),(layer[4][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end4 = std::chrono::high_resolution_clock::now();
        kernel_time4 = std::chrono::duration<double>(kernel_end4 - kernel_start4);
        kernel_time_in_sec[4] += kernel_time4.count();

        auto kernel_start5 = std::chrono::high_resolution_clock::now();
        dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer5_in0, bohdl_layer5_in1,bohdl_layer5_out,(layer[5][0]/(M_acc)),(layer[5][1]/K_acc),(layer[5][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
        xrtRunWait(dma_rhdl);
        auto kernel_end5 = std::chrono::high_resolution_clock::now();
        kernel_time5 = std::chrono::duration<double>(kernel_end5 - kernel_start5);
        kernel_time_in_sec[5] += kernel_time5.count();

        for (int j=0;j<batch_size;j++){
            auto kernel_start6 = std::chrono::high_resolution_clock::now();
            dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer6_in0, bohdl_layer6_in1,bohdl_layer6_out,(layer[6][0]/(M_acc)),(layer[6][1]/K_acc),(layer[6][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
            xrtRunWait(dma_rhdl);
            auto kernel_end6 = std::chrono::high_resolution_clock::now();
            kernel_time6 = std::chrono::duration<double>(kernel_end6 - kernel_start6);
            kernel_time_in_sec[6] += kernel_time6.count();

            auto kernel_start7 = std::chrono::high_resolution_clock::now();
            dma_rhdl = xrtKernelRun(dma_khdl, bohdl_layer7_in0, bohdl_layer7_in1,bohdl_layer7_out,(layer[7][0]/(M_acc)),(layer[7][1]/K_acc),(layer[7][2]/N_acc),
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
                        nullptr, nullptr, nullptr, nullptr);
            xrtRunWait(dma_rhdl);
            auto kernel_end7 = std::chrono::high_resolution_clock::now();
            kernel_time7 = std::chrono::duration<double>(kernel_end7 - kernel_start7);
            kernel_time_in_sec[7] += kernel_time7.count();
        }

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
    std::cout << "Total time is: "<< Time_total <<"s, TOPS_Overall = " << TOPS_Overall << " GOPS/s" << std::endl;

    for (int i=0;i<NUM_LARYER;i++){
        std::cout << "The time of each layer" << i <<" is : " << kernel_time_in_sec[i] <<"s, TOPS_ACC" << i <<" = " << TOPS[i] << " GOPS/s" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << std::endl;


    // sync output memory
    xrtBOSync(bohdl_layer0_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[0][0]*layer[0][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer1_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[1][0]*layer[1][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer2_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[2][0]*layer[2][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer3_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[3][0]*layer[3][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer4_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[4][0]*layer[4][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer5_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[5][0]*layer[5][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer6_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[6][0]*layer[6][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer7_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[7][0]*layer[7][2])* sizeof(float),/*OFFSET=*/ 0);

    xrtRunClose(dma_rhdl);
    xrtKernelClose(dma_khdl);


    
    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////

    if (verify)

    {
        float sum = 0;
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
                if(abs((float)(bomapped_layer0_out[m+n*layer[0][0]])-layer0_golden[m][n])>=1e-3){
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
                if(abs((float)(bomapped_layer1_out[m+n*layer[1][0]])-layer1_golden[m][n])>=1e-3){
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
                if(abs((float)(bomapped_layer2_out[m+n*layer[2][0]])-layer2_golden[m][n])>=1e-3){
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
                if(abs((float)(bomapped_layer3_out[m+n*layer[3][0]])-layer3_golden[m][n])>=1e-3){
                    errorCount3++;
                }
            }
        }
        if (!errorCount3)
            printf("Layer3 Passed\n");
        else
            printf("Layer3 Failed\n");
        
        int errorCount4 = 0;
        for (int m = 0; m < layer[4][0]; m++) {
            for (int n = 0; n < layer[4][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[4][1]; k++) {
                    sum=sum+layer4_in0[m][k]*layer4_in1[k][n];
                }
                layer4_golden[m][n]=sum;
            }
        }

        for (int m = 0; m < layer[4][0]; m++) {
            for (int n = 0; n < layer[4][2]; n++) {
                if(abs((float)(bomapped_layer4_out[m+n*layer[4][0]])-layer4_golden[m][n])>=1e-3){
                    errorCount4++;
                }
            }
        }
        if (!errorCount4)
            printf("Layer4 Passed\n");
        else
            printf("Layer4 Failed\n");

        for (int m = 0; m < layer[5][0]; m++) {
            for (int n = 0; n < layer[5][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[5][1]; k++) {
                    sum=sum+layer5_in0[m][k]*layer5_in1[k][n];
                }
                layer5_golden[m][n]=sum;
            }
        }

        int errorCount5 = 0;
        for (int m = 0; m < layer[5][0]; m++) {
            for (int n = 0; n < layer[5][2]; n++) {
                if(abs((float)(bomapped_layer5_out[m+n*layer[5][0]])-layer5_golden[m][n])>=1e-3){
                    errorCount5++;
                }
            }
        }
        if (!errorCount5)
            printf("Layer5 Passed\n");
        else
            printf("Layer5 Failed\n");

        int errorCount6 = 0;
        for (int m = 0; m < layer[6][0]; m++) {
            for (int n = 0; n < layer[6][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[6][1]; k++) {
                    sum=sum+layer6_in0[m][k]*layer6_in1[k][n];
                }
                layer6_golden[m][n]=sum;
            }
        }

        for (int m = 0; m < layer[6][0]; m++) {
            for (int n = 0; n < layer[6][2]; n++) {
                if(abs((float)(bomapped_layer6_out[m+n*layer[6][0]])-layer6_golden[m][n])>=1e-3){
                    errorCount6++;
                }
            }
        }
        if (!errorCount6)
            printf("Layer6 Passed\n");
        else
            printf("Layer6 Failed\n");

         int errorCount7 = 0;
        for (int m = 0; m < layer[7][0]; m++) {
            for (int n = 0; n < layer[7][2]; n++) {
                sum =0;
                for (int k = 0; k < layer[7][1]; k++) {
                    sum=sum+layer7_in0[m][k]*layer7_in1[k][n];
                }
                layer7_golden[m][n]=sum;
            }
        }

        for (int m = 0; m < layer[7][0]; m++) {
            for (int n = 0; n < layer[7][2]; n++) {
                if(abs((float)(bomapped_layer7_out[m+n*layer[7][0]])-layer7_golden[m][n])>=1e-3){
                    errorCount7++;
                }
            }
        }
        if (!errorCount7)
            printf("Layer7 Passed\n");
        else
            printf("Layer7 Failed\n");
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << "Releasing remaining XRT objects...\n";
    
    xrtBOFree(bohdl_layer0_in0);
    xrtBOFree(bohdl_layer0_in1);
    xrtBOFree(bohdl_layer0_out);

    xrtBOFree(bohdl_layer1_in0);
    xrtBOFree(bohdl_layer1_in1);
    xrtBOFree(bohdl_layer1_out);

    xrtBOFree(bohdl_layer2_in0);
    xrtBOFree(bohdl_layer2_in1);
    xrtBOFree(bohdl_layer2_out);

    xrtBOFree(bohdl_layer3_in0);
    xrtBOFree(bohdl_layer3_in1);
    xrtBOFree(bohdl_layer3_out);

    xrtBOFree(bohdl_layer4_in0);
    xrtBOFree(bohdl_layer4_in1);
    xrtBOFree(bohdl_layer4_out);

    xrtBOFree(bohdl_layer5_in0);
    xrtBOFree(bohdl_layer5_in1);
    xrtBOFree(bohdl_layer5_out);

    xrtBOFree(bohdl_layer6_in0);
    xrtBOFree(bohdl_layer6_in1);
    xrtBOFree(bohdl_layer6_out);

    xrtBOFree(bohdl_layer7_in0);
    xrtBOFree(bohdl_layer7_in1);
    xrtBOFree(bohdl_layer7_out);
    xrtDeviceClose(dhdl);
    return 0;
}