#include "layer0/aie_top_L0.h"
#include "layer1/aie_top_L1.h"
#include "layer2/aie_top_L2.h"
#include "layer3/aie_top_L3.h"
#include "layer4/aie_top_L4.h"
#include "layer5/aie_top_L5.h"
#include "layer6/aie_top_L6.h"
#include "layer7/aie_top_L7.h"
using namespace adf;

PLIO* LHS_in0_L0 = new PLIO("LHS_in0_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L0 = new PLIO("LHS_in1_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L0 = new PLIO("LHS_in2_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L0 = new PLIO("LHS_in3_L0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L1 = new PLIO("LHS_in0_L1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L1 = new PLIO("LHS_in1_L1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L1 = new PLIO("LHS_in2_L1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L1 = new PLIO("LHS_in3_L1", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L2 = new PLIO("LHS_in0_L2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L2 = new PLIO("LHS_in1_L2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L2 = new PLIO("LHS_in2_L2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L2 = new PLIO("LHS_in3_L2", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L3 = new PLIO("LHS_in0_L3", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L3 = new PLIO("LHS_in1_L3", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L3 = new PLIO("LHS_in2_L3", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L3 = new PLIO("LHS_in3_L3", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L4 = new PLIO("LHS_in0_L4", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L4 = new PLIO("LHS_in1_L4", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L4 = new PLIO("LHS_in2_L4", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L4 = new PLIO("LHS_in3_L4", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L5 = new PLIO("LHS_in0_L5", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L5 = new PLIO("LHS_in1_L5", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L5 = new PLIO("LHS_in2_L5", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L5 = new PLIO("LHS_in3_L5", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L6 = new PLIO("LHS_in0_L6", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L6 = new PLIO("LHS_in1_L6", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L6 = new PLIO("LHS_in2_L6", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L6 = new PLIO("LHS_in3_L6", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L7 = new PLIO("LHS_in0_L7", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L7 = new PLIO("LHS_in1_L7", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L7 = new PLIO("LHS_in2_L7", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L7 = new PLIO("LHS_in3_L7", adf::plio_128_bits, "data/input0.txt",230);

PLIO* RHS_in0_L0 = new PLIO("RHS_in0_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L0 = new PLIO("RHS_in1_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L0 = new PLIO("RHS_in2_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L0 = new PLIO("RHS_in3_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L1 = new PLIO("RHS_in0_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L1 = new PLIO("RHS_in1_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L1 = new PLIO("RHS_in2_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L1 = new PLIO("RHS_in3_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L2 = new PLIO("RHS_in0_L2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L2 = new PLIO("RHS_in1_L2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L2 = new PLIO("RHS_in2_L2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L2 = new PLIO("RHS_in3_L2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L3 = new PLIO("RHS_in0_L3", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L3 = new PLIO("RHS_in1_L3", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L3 = new PLIO("RHS_in2_L3", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L3 = new PLIO("RHS_in3_L3", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L4 = new PLIO("RHS_in0_L4", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L4 = new PLIO("RHS_in1_L4", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L4 = new PLIO("RHS_in2_L4", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L4 = new PLIO("RHS_in3_L4", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L5 = new PLIO("RHS_in0_L5", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L5 = new PLIO("RHS_in1_L5", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L5 = new PLIO("RHS_in2_L5", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L5 = new PLIO("RHS_in3_L5", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L6 = new PLIO("RHS_in0_L6", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L6 = new PLIO("RHS_in1_L6", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L6 = new PLIO("RHS_in2_L6", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L6 = new PLIO("RHS_in3_L6", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L7 = new PLIO("RHS_in0_L7", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L7 = new PLIO("RHS_in1_L7", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L7 = new PLIO("RHS_in2_L7", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L7 = new PLIO("RHS_in3_L7", adf::plio_128_bits, "data/input1.txt",230);
PLIO* out0_L0 = new PLIO("out0_L0", adf::plio_128_bits, "data/output0.txt",230);
PLIO* out1_L0 = new PLIO("out1_L0", adf::plio_128_bits, "data/output1.txt",230);
PLIO* out2_L0 = new PLIO("out2_L0", adf::plio_128_bits, "data/output2.txt",230);
PLIO* out3_L0 = new PLIO("out3_L0", adf::plio_128_bits, "data/output3.txt",230);

PLIO* out0_L1 = new PLIO("out0_L1", adf::plio_128_bits, "data/output4.txt",230);
PLIO* out1_L1 = new PLIO("out1_L1", adf::plio_128_bits, "data/output5.txt",230);
PLIO* out2_L1 = new PLIO("out2_L1", adf::plio_128_bits, "data/output6.txt",230);
PLIO* out3_L1 = new PLIO("out3_L1", adf::plio_128_bits, "data/output7.txt",230);

PLIO* out0_L2 = new PLIO("out0_L2", adf::plio_128_bits, "data/output8.txt",230);
PLIO* out1_L2 = new PLIO("out1_L2", adf::plio_128_bits, "data/output9.txt",230);
PLIO* out2_L2 = new PLIO("out2_L2", adf::plio_128_bits, "data/output10.txt",230);
PLIO* out3_L2 = new PLIO("out3_L2", adf::plio_128_bits, "data/output11.txt",230);

PLIO* out0_L3 = new PLIO("out0_L3", adf::plio_128_bits, "data/output12.txt",230);
PLIO* out1_L3 = new PLIO("out1_L3", adf::plio_128_bits, "data/output13.txt",230);
PLIO* out2_L3 = new PLIO("out2_L3", adf::plio_128_bits, "data/output14.txt",230);
PLIO* out3_L3 = new PLIO("out3_L3", adf::plio_128_bits, "data/output15.txt",230);

PLIO* out0_L4 = new PLIO("out0_L4", adf::plio_128_bits, "data/output16.txt",230);
PLIO* out1_L4 = new PLIO("out1_L4", adf::plio_128_bits, "data/output17.txt",230);
PLIO* out2_L4 = new PLIO("out2_L4", adf::plio_128_bits, "data/output18.txt",230);
PLIO* out3_L4 = new PLIO("out3_L4", adf::plio_128_bits, "data/output19.txt",230);

PLIO* out0_L5 = new PLIO("out0_L5", adf::plio_128_bits, "data/output20.txt",230);
PLIO* out1_L5 = new PLIO("out1_L5", adf::plio_128_bits, "data/output21.txt",230);
PLIO* out2_L5 = new PLIO("out2_L5", adf::plio_128_bits, "data/output22.txt",230);
PLIO* out3_L5 = new PLIO("out3_L5", adf::plio_128_bits, "data/output23.txt",230);

PLIO* out0_L6 = new PLIO("out0_L6", adf::plio_128_bits, "data/output24.txt",230);
PLIO* out1_L6 = new PLIO("out1_L6", adf::plio_128_bits, "data/output25.txt",230);
PLIO* out2_L6 = new PLIO("out2_L6", adf::plio_128_bits, "data/output26.txt",230);
PLIO* out3_L6 = new PLIO("out3_L6", adf::plio_128_bits, "data/output27.txt",230);

PLIO* out0_L7 = new PLIO("out0_L7", adf::plio_128_bits, "data/output28.txt",230);
PLIO* out1_L7 = new PLIO("out1_L7", adf::plio_128_bits, "data/output29.txt",230);
PLIO* out2_L7 = new PLIO("out2_L7", adf::plio_128_bits, "data/output30.txt",230);
PLIO* out3_L7 = new PLIO("out3_L7", adf::plio_128_bits, "data/output31.txt",230);

simulation::platform<64, 32> platform(
LHS_in0_L0,
LHS_in1_L0,
LHS_in2_L0,
LHS_in3_L0,
LHS_in0_L1,
LHS_in1_L1,
LHS_in2_L1,
LHS_in3_L1,
LHS_in0_L2,
LHS_in1_L2,
LHS_in2_L2,
LHS_in3_L2,
LHS_in0_L3,
LHS_in1_L3,
LHS_in2_L3,
LHS_in3_L3,
LHS_in0_L4,
LHS_in1_L4,
LHS_in2_L4,
LHS_in3_L4,
LHS_in0_L5,
LHS_in1_L5,
LHS_in2_L5,
LHS_in3_L5,
LHS_in0_L6,
LHS_in1_L6,
LHS_in2_L6,
LHS_in3_L6,
LHS_in0_L7,
LHS_in1_L7,
LHS_in2_L7,
LHS_in3_L7,
RHS_in0_L0, 
RHS_in1_L0, 
RHS_in2_L0, 
RHS_in3_L0, 
RHS_in0_L1, 
RHS_in1_L1, 
RHS_in2_L1, 
RHS_in3_L1, 
RHS_in0_L2, 
RHS_in1_L2, 
RHS_in2_L2, 
RHS_in3_L2, 
RHS_in0_L3, 
RHS_in1_L3, 
RHS_in2_L3, 
RHS_in3_L3, 
RHS_in0_L4, 
RHS_in1_L4, 
RHS_in2_L4, 
RHS_in3_L4, 
RHS_in0_L5, 
RHS_in1_L5, 
RHS_in2_L5, 
RHS_in3_L5, 
RHS_in0_L6, 
RHS_in1_L6, 
RHS_in2_L6, 
RHS_in3_L6, 
RHS_in0_L7, 
RHS_in1_L7, 
RHS_in2_L7, 
RHS_in3_L7, 
out0_L0,
out1_L0,
out2_L0,
out3_L0,
out0_L1,
out1_L1,
out2_L1,
out3_L1,
out0_L2,
out1_L2,
out2_L2,
out3_L2,
out0_L3,
out1_L3,
out2_L3,
out3_L3,
out0_L4,
out1_L4,
out2_L4,
out3_L4,
out0_L5,
out1_L5,
out2_L5,
out3_L5,
out0_L6,
out1_L6,
out2_L6,
out3_L6,
out0_L7,
out1_L7,
out2_L7,
out3_L7 
);


mm_x2_x3_x4_graph0  mm_graph0;
mm_x2_x3_x4_graph1  mm_graph1;
mm_x2_x3_x4_graph2  mm_graph2;
mm_x2_x3_x4_graph3  mm_graph3;
mm_x2_x3_x4_graph4  mm_graph4;
mm_x2_x3_x4_graph5  mm_graph5;
mm_x2_x3_x4_graph6  mm_graph6;
mm_x2_x3_x4_graph7  mm_graph7;


connect<> net_lhs_in0_L0 (platform.src[0], mm_graph0.in_lhs[0][0]);
connect<> net_lhs_in1_L0 (platform.src[1], mm_graph0.in_lhs[1][0]);
connect<> net_lhs_in2_L0 (platform.src[2], mm_graph0.in_lhs[2][0]);
connect<> net_lhs_in3_L0 (platform.src[3], mm_graph0.in_lhs[3][0]);

connect<> net_lhs_in0_L1 (platform.src[4], mm_graph1.in_lhs[0][0]);
connect<> net_lhs_in1_L1 (platform.src[5], mm_graph1.in_lhs[1][0]);
connect<> net_lhs_in2_L1 (platform.src[6], mm_graph1.in_lhs[2][0]);
connect<> net_lhs_in3_L1 (platform.src[7], mm_graph1.in_lhs[3][0]);

connect<> net_lhs_in0_L2 (platform.src[8], mm_graph2.in_lhs[0][0]);
connect<> net_lhs_in1_L2 (platform.src[9], mm_graph2.in_lhs[1][0]);
connect<> net_lhs_in2_L2 (platform.src[10], mm_graph2.in_lhs[2][0]);
connect<> net_lhs_in3_L2 (platform.src[11], mm_graph2.in_lhs[3][0]);

connect<> net_lhs_in0_L3 (platform.src[12], mm_graph3.in_lhs[0][0]);
connect<> net_lhs_in1_L3 (platform.src[13], mm_graph3.in_lhs[1][0]);
connect<> net_lhs_in2_L3 (platform.src[14], mm_graph3.in_lhs[2][0]);
connect<> net_lhs_in3_L3 (platform.src[15], mm_graph3.in_lhs[3][0]);

connect<> net_lhs_in0_L4 (platform.src[16], mm_graph4.in_lhs[0][0]);
connect<> net_lhs_in1_L4 (platform.src[17], mm_graph4.in_lhs[1][0]);
connect<> net_lhs_in2_L4 (platform.src[18], mm_graph4.in_lhs[2][0]);
connect<> net_lhs_in3_L4 (platform.src[19], mm_graph4.in_lhs[3][0]);

connect<> net_lhs_in0_L5 (platform.src[20], mm_graph5.in_lhs[0][0]);
connect<> net_lhs_in1_L5 (platform.src[21], mm_graph5.in_lhs[1][0]);
connect<> net_lhs_in2_L5 (platform.src[22], mm_graph5.in_lhs[2][0]);
connect<> net_lhs_in3_L5 (platform.src[23], mm_graph5.in_lhs[3][0]);

connect<> net_lhs_in0_L6 (platform.src[24], mm_graph6.in_lhs[0][0]);
connect<> net_lhs_in1_L6 (platform.src[25], mm_graph6.in_lhs[1][0]);
connect<> net_lhs_in2_L6 (platform.src[26], mm_graph6.in_lhs[2][0]);
connect<> net_lhs_in3_L6 (platform.src[27], mm_graph6.in_lhs[3][0]);

connect<> net_lhs_in0_L7 (platform.src[28], mm_graph7.in_lhs[0][0]);
connect<> net_lhs_in1_L7 (platform.src[29], mm_graph7.in_lhs[1][0]);
connect<> net_lhs_in2_L7 (platform.src[30], mm_graph7.in_lhs[2][0]);
connect<> net_lhs_in3_L7 (platform.src[31], mm_graph7.in_lhs[3][0]);

connect<> net_rhs_in0_L0 (platform.src[32], mm_graph0.in_rhs[0][0]);
connect<> net_rhs_in1_L0 (platform.src[33], mm_graph0.in_rhs[1][0]);
connect<> net_rhs_in2_L0 (platform.src[34], mm_graph0.in_rhs[2][0]);
connect<> net_rhs_in3_L0 (platform.src[35], mm_graph0.in_rhs[3][0]);
connect<> net_rhs_in0_L1 (platform.src[36], mm_graph1.in_rhs[0][0]);
connect<> net_rhs_in1_L1 (platform.src[37], mm_graph1.in_rhs[1][0]);
connect<> net_rhs_in2_L1 (platform.src[38], mm_graph1.in_rhs[2][0]);
connect<> net_rhs_in3_L1 (platform.src[39], mm_graph1.in_rhs[3][0]);
connect<> net_rhs_in0_L2 (platform.src[40], mm_graph2.in_rhs[0][0]);
connect<> net_rhs_in1_L2 (platform.src[41], mm_graph2.in_rhs[1][0]);
connect<> net_rhs_in2_L2 (platform.src[42], mm_graph2.in_rhs[2][0]);
connect<> net_rhs_in3_L2 (platform.src[43], mm_graph2.in_rhs[3][0]);
connect<> net_rhs_in0_L3 (platform.src[44], mm_graph3.in_rhs[0][0]);
connect<> net_rhs_in1_L3 (platform.src[45], mm_graph3.in_rhs[1][0]);
connect<> net_rhs_in2_L3 (platform.src[46], mm_graph3.in_rhs[2][0]);
connect<> net_rhs_in3_L3 (platform.src[47], mm_graph3.in_rhs[3][0]);
connect<> net_rhs_in0_L4 (platform.src[48], mm_graph4.in_rhs[0][0]);
connect<> net_rhs_in1_L4 (platform.src[49], mm_graph4.in_rhs[1][0]);
connect<> net_rhs_in2_L4 (platform.src[50], mm_graph4.in_rhs[2][0]);
connect<> net_rhs_in3_L4 (platform.src[51], mm_graph4.in_rhs[3][0]);
connect<> net_rhs_in0_L5 (platform.src[52], mm_graph5.in_rhs[0][0]);
connect<> net_rhs_in1_L5 (platform.src[53], mm_graph5.in_rhs[1][0]);
connect<> net_rhs_in2_L5 (platform.src[54], mm_graph5.in_rhs[2][0]);
connect<> net_rhs_in3_L5 (platform.src[55], mm_graph5.in_rhs[3][0]);
connect<> net_rhs_in0_L6 (platform.src[56], mm_graph6.in_rhs[0][0]);
connect<> net_rhs_in1_L6 (platform.src[57], mm_graph6.in_rhs[1][0]);
connect<> net_rhs_in2_L6 (platform.src[58], mm_graph6.in_rhs[2][0]);
connect<> net_rhs_in3_L6 (platform.src[59], mm_graph6.in_rhs[3][0]);
connect<> net_rhs_in0_L7 (platform.src[60], mm_graph7.in_rhs[0][0]);
connect<> net_rhs_in1_L7 (platform.src[61], mm_graph7.in_rhs[1][0]);
connect<> net_rhs_in2_L7 (platform.src[62], mm_graph7.in_rhs[2][0]);
connect<> net_rhs_in3_L7 (platform.src[63], mm_graph7.in_rhs[3][0]);
connect<> net_out0_L0 (mm_graph0.out[0], platform.sink[0]);
connect<> net_out1_L0 (mm_graph0.out[1], platform.sink[1]);
connect<> net_out2_L0 (mm_graph0.out[2], platform.sink[2]);
connect<> net_out3_L0 (mm_graph0.out[3], platform.sink[3]);
connect<> net_out0_L1 (mm_graph1.out[0], platform.sink[4]);
connect<> net_out1_L1 (mm_graph1.out[1], platform.sink[5]);
connect<> net_out2_L1 (mm_graph1.out[2], platform.sink[6]);
connect<> net_out3_L1 (mm_graph1.out[3], platform.sink[7]);
connect<> net_out0_L2 (mm_graph2.out[0], platform.sink[8]);
connect<> net_out1_L2 (mm_graph2.out[1], platform.sink[9]);
connect<> net_out2_L2 (mm_graph2.out[2], platform.sink[10]);
connect<> net_out3_L2 (mm_graph2.out[3], platform.sink[11]);
connect<> net_out0_L3 (mm_graph3.out[0], platform.sink[12]);
connect<> net_out1_L3 (mm_graph3.out[1], platform.sink[13]);
connect<> net_out2_L3 (mm_graph3.out[2], platform.sink[14]);
connect<> net_out3_L3 (mm_graph3.out[3], platform.sink[15]);
connect<> net_out0_L4 (mm_graph4.out[0], platform.sink[16]);
connect<> net_out1_L4 (mm_graph4.out[1], platform.sink[17]);
connect<> net_out2_L4 (mm_graph4.out[2], platform.sink[18]);
connect<> net_out3_L4 (mm_graph4.out[3], platform.sink[19]);
connect<> net_out0_L5 (mm_graph5.out[0], platform.sink[20]);
connect<> net_out1_L5 (mm_graph5.out[1], platform.sink[21]);
connect<> net_out2_L5 (mm_graph5.out[2], platform.sink[22]);
connect<> net_out3_L5 (mm_graph5.out[3], platform.sink[23]);
connect<> net_out0_L6 (mm_graph6.out[0], platform.sink[24]);
connect<> net_out1_L6 (mm_graph6.out[1], platform.sink[25]);
connect<> net_out2_L6 (mm_graph6.out[2], platform.sink[26]);
connect<> net_out3_L6 (mm_graph6.out[3], platform.sink[27]);
connect<> net_out0_L7 (mm_graph7.out[0], platform.sink[28]);
connect<> net_out1_L7 (mm_graph7.out[1], platform.sink[29]);
connect<> net_out2_L7 (mm_graph7.out[2], platform.sink[30]);
connect<> net_out3_L7 (mm_graph7.out[3], platform.sink[31]);
#ifdef __AIESIM__
int main(int argc, char** argv) {
    mm_graph0.init();
    mm_graph1.init();
    mm_graph2.init();
    mm_graph3.init();
    mm_graph4.init();
    mm_graph5.init();
    mm_graph6.init();
    mm_graph7.init();
    mm_graph0.run(4);
    mm_graph1.run(4);
    mm_graph2.run(4);
    mm_graph3.run(4);
    mm_graph4.run(4);
    mm_graph5.run(4);
    mm_graph6.run(4);
    mm_graph7.run(4);
    mm_graph0.end();
    mm_graph1.end();
    mm_graph2.end();
    mm_graph3.end();
    mm_graph4.end();
    mm_graph5.end();
    mm_graph6.end();
    mm_graph7.end();
    return 0;
}
#endif