/**
* Copyright (C_large) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A_large copy of the
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
#include "../aie/mm_top_small.h"
#include "../aie/mm_top_large.h"
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
mm_x64_x4_graph<0,0> myGraph_large;
mm_x4_x2_graph<33,0> myGraph_small;
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

const int H1=32;
const int W1=32;
const int W2=32;
const int A_large=4;
const int B_large=4;
const int C_large=16;
const int X_large=8;
const int Y_large=2;
const int Z_large=2;
const int A_small=2;
const int B_small=2;
const int C_small=2;
const int X_small=1;
const int Y_small=1;
const int Z_small=1;

const int M_acc0=H1*A_small*X_small;
const int K_acc0=W1*B_small*Y_small;
const int N_acc0=W2*C_small*Z_small;
const int M_acc1=H1*A_large*X_large;
const int K_acc1=W1*B_large*Y_large;
const int N_acc1=W2*C_large*Z_large;


const int DATA_SIZE1=H1*W1;
const int DATA_SIZE2=W1*W2;
const int OUT_SIZE=H1*W2;
const int D0=48;
const int D1=16;
const int NUM_LARYER=7;
const int batch_size = D0 * D1;

const int layer_in[NUM_LARYER][4] =
{
    {3072,3024,1024,1},
    {3072,1024,1024,1},
    {3072,1024,4096,1},
    {3072,4096,1024,1},
    {3072,1024,3048,1},
    {64,64,64,batch_size},
    {64,64,64,batch_size}
};

const int layer[NUM_LARYER][4] =
{
    {3072,3072,1024,1},
    {3072,1024,1024,1},
    {3072,1024,4096,1},
    {3072,4096,1024,1},
    {3072,1024,3072,1},
    {64,64,64,batch_size},
    {64,64,64,batch_size}
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
    std::vector<std::vector<std::vector<std::vector<float>>>> layer5_in0(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[5][0],std::vector<float>(layer[5][1], 1.0))));
    std::vector<std::vector<std::vector<std::vector<float>>>> layer5_in1(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[5][1],std::vector<float>(layer[5][2], 1.0))));
    std::vector<std::vector<std::vector<std::vector<float>>>> layer5_golden(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[5][0],std::vector<float>(layer[5][2], 1.0))));

    //layer6
    std::vector<std::vector<std::vector<std::vector<float>>>> layer6_in0(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[6][0],std::vector<float>(layer[6][1], 1.0))));
    std::vector<std::vector<std::vector<std::vector<float>>>> layer6_in1(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[6][1],std::vector<float>(layer[6][2], 1.0))));
    std::vector<std::vector<std::vector<std::vector<float>>>> layer6_golden(D0, std::vector<std::vector<std::vector<float>>>(D1,std::vector<std::vector<float>>(layer[6][0],std::vector<float>(layer[6][2], 1.0))));

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
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int m = 0; m < layer[5][0]; m++) {
                for (int k = 0; k < layer[5][1]; k++) {
                    layer5_in0[dot_x][dot_y][m][k]= (rand()%5)*(float)1.0;
                }
            }
        }
    }

    srand (time(0));
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int k = 0; k < layer[5][1]; k++) {
                for (int n = 0; n < layer[5][2]; n++) {
                    layer5_in1[dot_x][dot_y][k][n]= (rand()%5)*(float)1.0;
                }
            }
        }
    }

    //layer6
    srand (time(0));
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int m = 0; m < layer[6][0]; m++) {
                for (int k = 0; k < layer[6][1]; k++) {
                    layer6_in0[dot_x][dot_y][m][k]= (rand()%5)*(float)1.0;
                }
            }
        }
    }

    srand (time(0));
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int k = 0; k < layer[6][1]; k++) {
                for (int n = 0; n < layer[6][2]; n++) {
                    layer6_in1[dot_x][dot_y][k][n]= (rand()%5)*(float)1.0;
                }
            }
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
    xrtBufferHandle bohdl_layer5_in0 = xrtBOAlloc(dhdl, (layer[5][0]*layer[5][1]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer5_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_in0));
    xrtBufferHandle bohdl_layer5_in1 = xrtBOAlloc(dhdl, (layer[5][1]*layer[5][2]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer5_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_in1));

    //layer6
    xrtBufferHandle bohdl_layer6_in0 = xrtBOAlloc(dhdl, (layer[6][0]*layer[6][1]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer6_in0 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_in0));
    xrtBufferHandle bohdl_layer6_in1 = xrtBOAlloc(dhdl, (layer[6][1]*layer[6][2]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer6_in1 = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_in1));
    
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
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int m = 0; m < layer[5][0]; m++) {
                for (int k = 0; k < layer[5][1]; k++) {
                    bomapped_layer5_in0[m+k*layer[5][0]*D0+dot_y*layer[5][1]*layer[5][0]*D0+dot_x*layer[5][0]]=layer5_in0[dot_x][dot_y][m][k];
                }
            }
        }
    }
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int k = 0; k < layer[5][1]; k++) {
                for (int n = 0; n < layer[5][2]; n++) {
                    bomapped_layer5_in1[k+n*layer[5][1]*D0+dot_y*layer[5][2]*layer[5][1]*D0+dot_x*layer[5][1]]=layer5_in1[dot_x][dot_y][k][n];
                }
            }
        }
    }

    //layer6
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int m = 0; m < layer[6][0]; m++) {
                for (int k = 0; k < layer[6][1]; k++) {
                    bomapped_layer6_in0[m+k*layer[6][0]*D0+dot_y*layer[6][1]*layer[6][0]*D0+dot_x*layer[6][0]]=layer6_in0[dot_x][dot_y][m][k];
                }
            }
        }
    }
    for(int dot_x=0;dot_x<D0;dot_x++){
        for(int dot_y=0;dot_y<D1;dot_y++){
            for (int k = 0; k < layer[6][1]; k++) {
                for (int n = 0; n < layer[6][2]; n++) {
                    bomapped_layer6_in1[k+n*layer[6][1]*D0+dot_y*layer[6][2]*layer[6][1]*D0+dot_x*layer[6][1]]=layer6_in1[dot_x][dot_y][k][n];
                }
            }
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
    xrtBOSync(bohdl_layer5_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[5][0]*layer[5][1]*batch_size)* sizeof(float),0);
    xrtBOSync(bohdl_layer5_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[5][1]*layer[5][2]*batch_size)* sizeof(float),0);

    //layer6
    xrtBOSync(bohdl_layer6_in0, XCL_BO_SYNC_BO_TO_DEVICE , (layer[6][0]*layer[6][1]*batch_size)* sizeof(float),0);
    xrtBOSync(bohdl_layer6_in1, XCL_BO_SYNC_BO_TO_DEVICE , (layer[6][1]*layer[6][2]*batch_size)* sizeof(float),0);



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
    xrtBufferHandle bohdl_layer5_out = xrtBOAlloc(dhdl, (layer[5][0]*layer[5][2]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer5_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer5_out));

    //layer6
    xrtBufferHandle bohdl_layer6_out = xrtBOAlloc(dhdl, (layer[6][0]*layer[6][2]*batch_size) * sizeof(float), 0, 0);
    auto bomapped_layer6_out = reinterpret_cast<float*>(xrtBOMap(bohdl_layer6_out));

    myGraph_large.init();
    myGraph_small.init();                         
    
    myGraph_large.run(-1);
    printf("graph_large run\n");
    myGraph_small.run(-1); 
    printf("graph_small run\n");


    std::cout << "Kernel run\n";
    xrtKernelHandle dma_khdl_large = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma_large");
    xrtRunHandle dma_rhdl_large;

    xrtKernelHandle dma_khdl_small = xrtPLKernelOpen(dhdl, top->m_header.uuid, "dma_small");
    xrtRunHandle dma_rhdl_small;

    //profile aie mm 
    double kernel_time_in_sec_total = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    std::vector<double>kernel_time_in_sec(NUM_LARYER,0.0);
    for (int i=0;i<iter;i++){
    std::chrono::duration<double> kernel_time0(0);
    std::chrono::duration<double> kernel_time1(0);
    std::chrono::duration<double> kernel_time2(0);
    std::chrono::duration<double> kernel_time3(0);
    std::chrono::duration<double> kernel_time4(0);
    std::chrono::duration<double> kernel_time5(0);
    std::chrono::duration<double> kernel_time6(0);
    auto kernel_start5 = std::chrono::high_resolution_clock::now();
    dma_rhdl_small = xrtKernelRun(dma_khdl_small, bohdl_layer5_in0, bohdl_layer5_in1,bohdl_layer5_out,(layer[5][0]/(M_acc0)),(layer[5][1]/K_acc0),(layer[5][2]/N_acc0),D0,D1,
                        nullptr, nullptr, nullptr, nullptr, //A_small 2, B_small 2
                        nullptr);// C_small 1
    auto kernel_start3 = std::chrono::high_resolution_clock::now();
    dma_rhdl_large = xrtKernelRun(dma_khdl_large, bohdl_layer3_in0, bohdl_layer3_in1,bohdl_layer3_out,(layer[3][0]/(M_acc1)),(layer[3][1]/K_acc1),(layer[3][2]/N_acc1),
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//A_large 16
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//B_large 32
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr);// C_large 16
    xrtRunWait(dma_rhdl_large);
    auto kernel_end3 = std::chrono::high_resolution_clock::now();
    kernel_time3 = std::chrono::duration<double>(kernel_end3 - kernel_start3);
    kernel_time_in_sec[3] += kernel_time3.count();

    auto kernel_start0 = std::chrono::high_resolution_clock::now();
    dma_rhdl_large = xrtKernelRun(dma_khdl_large, bohdl_layer0_in0, bohdl_layer0_in1,bohdl_layer0_out,(layer[0][0]/(M_acc1)),(layer[0][1]/K_acc1),(layer[0][2]/N_acc1),
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//A_large 16
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//B_large 32
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr);// C_large 16

    xrtRunWait(dma_rhdl_small);
    auto kernel_end5 = std::chrono::high_resolution_clock::now();
    kernel_time5 = std::chrono::duration<double>(kernel_end5 - kernel_start5);
    kernel_time_in_sec[5] += kernel_time5.count();

    auto kernel_start6 = std::chrono::high_resolution_clock::now();
    dma_rhdl_small = xrtKernelRun(dma_khdl_small, bohdl_layer6_in0, bohdl_layer6_in1,bohdl_layer6_out,(layer[6][0]/(M_acc0)),(layer[6][1]/K_acc0),(layer[6][2]/N_acc0),D0,D1,
                        nullptr, nullptr, nullptr, nullptr, //A_small 2, B_small 2
                        nullptr);// C_small 1
    
    xrtRunWait(dma_rhdl_large);
    auto kernel_end0 = std::chrono::high_resolution_clock::now();
    kernel_time0 = std::chrono::duration<double>(kernel_end0 - kernel_start0);
    kernel_time_in_sec[0] += kernel_time0.count();

    auto kernel_start1 = std::chrono::high_resolution_clock::now();
    dma_rhdl_large = xrtKernelRun(dma_khdl_large, bohdl_layer1_in0, bohdl_layer1_in1,bohdl_layer1_out,(layer[1][0]/(M_acc1)),(layer[1][1]/K_acc1),(layer[1][2]/N_acc1),
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//A_large 16
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//B_large 32
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr);// C_large 16
    
    xrtRunWait(dma_rhdl_large);
    auto kernel_end1 = std::chrono::high_resolution_clock::now();
    kernel_time1 = std::chrono::duration<double>(kernel_end1 - kernel_start1);
    kernel_time_in_sec[1] += kernel_time1.count();

    auto kernel_start2 = std::chrono::high_resolution_clock::now();
    dma_rhdl_large = xrtKernelRun(dma_khdl_large, bohdl_layer2_in0, bohdl_layer2_in1,bohdl_layer2_out,(layer[2][0]/(M_acc1)),(layer[2][1]/K_acc1),(layer[2][2]/N_acc1),
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//A_large 16
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//B_large 32
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr);// C_large 16
    xrtRunWait(dma_rhdl_large);
    auto kernel_end2 = std::chrono::high_resolution_clock::now();
    kernel_time2 = std::chrono::duration<double>(kernel_end2 - kernel_start2);
    kernel_time_in_sec[2] += kernel_time2.count();

    
    auto kernel_start4 = std::chrono::high_resolution_clock::now();
    dma_rhdl_large = xrtKernelRun(dma_khdl_large, bohdl_layer4_in0, bohdl_layer4_in1,bohdl_layer4_out,(layer[4][0]/(M_acc1)),(layer[4][1]/K_acc1),(layer[4][2]/N_acc1),
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//A_large 16
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,//B_large 32
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr);// C_large 16

    xrtRunWait(dma_rhdl_small);
    auto kernel_end6 = std::chrono::high_resolution_clock::now();
    kernel_time6 = std::chrono::duration<double>(kernel_end6 - kernel_start6);
    kernel_time_in_sec[6] += kernel_time6.count();

    xrtRunWait(dma_rhdl_large);
    auto kernel_end4 = std::chrono::high_resolution_clock::now();
    kernel_time4 = std::chrono::duration<double>(kernel_end4 - kernel_start4);
    kernel_time_in_sec[4] += kernel_time4.count();
    }
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    kernel_time_in_sec_total = kernel_time.count();

    std::vector<double>TOPS(NUM_LARYER,0.0);
    std::vector<double>OPS(NUM_LARYER,0.0);
    
    for (int i=0;i<NUM_LARYER;i++){
        OPS[i]=(double)layer_in[i][0]*(double)layer_in[i][1]*(double)layer_in[i][2]*(double)layer_in[i][3]*2*(1e-9);
    }

    for (int i=0;i<NUM_LARYER;i++){
        std::cout << "The Ops of layer" << i <<" is : " << OPS[i] << "GOP" << std::endl;
    }

    for (int i=0;i<NUM_LARYER;i++){
        TOPS[i]=OPS[i]*iter/kernel_time_in_sec[i];
    }

    double TOPS_total=0;
    for (int i=0;i<NUM_LARYER;i++){
        TOPS_total+=OPS[i];
    }

    double TOPS_Overall= (TOPS_total)*iter/kernel_time_in_sec_total;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Total time is: "<< kernel_time_in_sec_total <<"s, TOPS_Overall = " << TOPS_Overall << " GOPS/s" << std::endl;

    for (int i=0;i<NUM_LARYER;i++){
        std::cout << "The time of layer" << i <<" is : " << kernel_time_in_sec[i] <<"s, TOPS_ACC" << i <<" = " << TOPS[i] << " GOPS/s" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << std::endl;

    // sync output memory
    xrtBOSync(bohdl_layer0_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[0][0]*layer[0][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer1_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[1][0]*layer[1][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer2_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[2][0]*layer[2][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer3_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[3][0]*layer[3][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer4_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[4][0]*layer[4][2])* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer5_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[5][0]*layer[5][2]*batch_size)* sizeof(float),/*OFFSET=*/ 0);
    xrtBOSync(bohdl_layer6_out, XCL_BO_SYNC_BO_FROM_DEVICE , (layer[6][0]*layer[6][2]*batch_size)* sizeof(float),/*OFFSET=*/ 0);

    xrtRunClose(dma_rhdl_large);
    xrtRunClose(dma_rhdl_small);

    xrtKernelClose(dma_khdl_large);
    xrtKernelClose(dma_khdl_small);
    ////////////////////////////////////////////
    //// Comparing the execution data to the golden_large data
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

        for(int dot_x=0;dot_x<D0;dot_x++){
            for(int dot_y=0;dot_y<D1;dot_y++){
                for (int m = 0; m < layer[5][0]; m++) { 
                    for (int n = 0; n < layer[5][2]; n++) {
                        sum =0;
                        for (int k = 0; k < layer[5][1]; k++) {
                            sum=sum+layer5_in0[dot_x][dot_y][m][k]*layer5_in1[dot_x][dot_y][k][n];
                        }
                        layer5_golden[dot_x][dot_y][m][n]=sum;
                    }
                }
            }
        }

        int errorCount5 = 0;
        for(int dot_x=0;dot_x<D0;dot_x++){
            for(int dot_y=0;dot_y<D1;dot_y++){  
                for (int m = 0; m < layer[5][0]; m++) {
                    for (int n = 0; n < layer[5][2]; n++) {
                        if(abs((float)(bomapped_layer5_out[m+n*layer[5][0]*D0+dot_y*layer[5][2]*layer[5][0]*D0+dot_x*layer[5][0]])-layer5_golden[dot_x][dot_y][m][n])>=1e-3){
                            errorCount5++;
                        }
                    }
                }
            }
        }
        if (!errorCount5)
            printf("Layer5 Passed\n");
        else
            printf("Layer5 Failed\n");

        int errorCount6 = 0;
        for(int dot_x=0;dot_x<D0;dot_x++){
            for(int dot_y=0;dot_y<D1;dot_y++){
                for (int m = 0; m < layer[6][0]; m++) {
                    for (int n = 0; n < layer[6][2]; n++) {
                        sum =0;
                        for (int k = 0; k < layer[6][1]; k++) {
                            sum=sum+layer6_in0[dot_x][dot_y][m][k]*layer6_in1[dot_x][dot_y][k][n];
                        }
                        layer6_golden[dot_x][dot_y][m][n]=sum;
                    }
                }
            }
        }

        for(int dot_x=0;dot_x<D0;dot_x++){
            for(int dot_y=0;dot_y<D1;dot_y++){  
                for (int m = 0; m < layer[6][0]; m++) {
                    for (int n = 0; n < layer[6][2]; n++) {
                        if(abs((float)(bomapped_layer6_out[m+n*layer[6][0]*D0+dot_y*layer[6][2]*layer[6][0]*D0+dot_x*layer[6][0]])-layer6_golden[dot_x][dot_y][m][n])>=1e-3){
                            errorCount6++;
                        }
                    }
                }
            }
        }
        if (!errorCount6)
            printf("Layer6 Passed\n");
        else
            printf("Layer6 Failed\n");
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

    xrtDeviceClose(dhdl);
    return 0;
}