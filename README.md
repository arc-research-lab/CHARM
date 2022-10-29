# CHARM

## Overview
In this repo, we use general-purpose Matrix-Matrix Multiplication (GEMM) applications as an example and provide a detailed description of how to build a system-level design on AMD Versal VCK190 Platform. By going through this repo, users can get knowledge on:

+ How to design a high efficient single AIE kernel by leveraging the 7-way very long instruction words (VLIW)?
+ How to sustain 400 AIEs with the limited I/O interfaces between AIE and PL by using broadcast-packet mechanism?
+ How to transfer data from PL/AIE to AIE/PL by using bubble-free pipeline strategy?

We provide an automatic code generation and compilation flow that users can build the system on Versal step by step by changing the configuration files.

## Dependencies 
To play with the CHARM MM Accelerators, following software and hardware dependencies are required:
+ AMD/Xilinx Vitis 2021.1
+ AMD/Xilinx XRT Library
+ AMD/Xilinx VCK190 Evaluation Kit
+ Linux System with "tar" installed

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
```
## Step by step Tutorial
In this part, we first introduce the overall MM tiling strategy including four levels of tilings. Then in the later parts, we illustrate the methodology of how we handle each of these level of tilings.<br>
### Overall MM tiling strategy:
Given a large Matrix Multiplication(MM) with size (M\*K) * (K\*N) refer as M\*K\*N, the listing bellow shows four level of tilings to handle this MM (from innermost to outermost):<br>
+ Line 16-20: MM calculated on a **single AIE core**. 
+ Line 12-14: The spatial distribution unrolled across different AIE cores in **AIE Array**.
+ Line 7-9: The sequential processing of data stored in **PL on-chip memories**. 
+ Line 2-4: The temporal processing of data stored in off-chip memory.<br>

We visualize the on-chip buffer level tiling in the right figure. We refer the MM calculated in single AIE as  **"Tile"**  level and refer the MM unrolled on AIE array level as  **"Batch"**  level. The strtegy of mapping the tiled MM on AIE array will be illustrated later. 

<img src="https://user-images.githubusercontent.com/77606152/198852392-beb5d876-56c7-4486-8b14-f3ea0021ab72.png" width="400" height="300">         <img src="https://user-images.githubusercontent.com/77606152/198853940-ebdd1006-4807-42eb-9595-ce4f6fe2cc18.png" width="500" height="300">


