# CHARM

## Overview
In this repo, we use general-purpose Matrix-Matrix Multiplication(GEMM) applications as an example and provide a detailed description of how to build a system-level design on AMD Versal VCK190 Platform. By going through this repo, users can get knowledge on how to design a high efficient single AIE kernel, how to sustain 400 AIEs with the limited I/O interfaces between AIE and PL as well as the data transfer methodology from PL/AIE to AIE/PL.

We provide an automatic code generation and compilation flow that users can build the system on Versal step by step by changing the configuration files.

## Dependencies 
To play with the Charming Accelerators, following software and hardware dependencies are required:
+ Linux System with "tar" installed
+ AMD/Xilinx Vitis 2021.1
+ AMD/Xilinx XRT Library
+ AMD/Xilinx VCK190 Evaluation Kit

## Environment Setup
1. To quickly boost and run experiments on the board instead of building the platform and Linux from scratch, users can download the platform package (VCK190 Base 2021.1) and petalinux common image(Versal common image) from the following link:<br/>
https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2021-1.html

2. VCK190 Base 2021.1: It contains the pre-built Versal extensible embedded platform. During compilation users need to specify the platofrm path in the following format.<br/> 
```
PLATFORM = ${PATH}/xilinx_vck190_base_202110_1/xilinx_vck190_base_202110_1.xpfm
```

3. Versal common image: It includes the petalinux system boot files and the cross compilation environment needed for ARM CPU. Users can install the petalinux by running ``./sdk.sh``. During compilation, users need to point the path to SYSROOT and EDGE_COMMON_SW.<br/>
```
SYSROOT = ${PATH}/sysroots/cortexa72-cortexa53-xilinx-linux
EDGE_COMMON_SW=${PATH}/xilinx-versal-common-v2021.1
```

4. Vitis and Cross-compilation Environment Setup<br/>
```
source /opt/tools/xilinx/Vitis/2021.1/settings64.sh
source /opt/xilinx/xrt/setup.sh
unset LD_LIBRARY_PATH (If needed)
source ${PATH}/environment-setup-cortexa72-cortexa53-xilinx-linux
