#include "layer0/aie_top_L0.h"
#include "layer1/aie_top_L1.h"
using namespace adf;

PLIO* LHS_in0_L0 = new PLIO("LHS_in0_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L0 = new PLIO("LHS_in1_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L0 = new PLIO("LHS_in2_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L0 = new PLIO("LHS_in3_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in4_L0 = new PLIO("LHS_in4_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in5_L0 = new PLIO("LHS_in5_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in6_L0 = new PLIO("LHS_in6_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in7_L0 = new PLIO("LHS_in7_L0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L1 = new PLIO("LHS_in0_L1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L1 = new PLIO("LHS_in1_L1", adf::plio_128_bits, "data/input0.txt",230);

PLIO* RHS_in0_L0 = new PLIO("RHS_in0_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L0 = new PLIO("RHS_in1_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L0 = new PLIO("RHS_in2_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L0 = new PLIO("RHS_in3_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in4_L0 = new PLIO("RHS_in4_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in5_L0 = new PLIO("RHS_in5_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in6_L0 = new PLIO("RHS_in6_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in7_L0 = new PLIO("RHS_in7_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in8_L0 = new PLIO("RHS_in8_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in9_L0 = new PLIO("RHS_in9_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in10_L0 = new PLIO("RHS_in10_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in11_L0 = new PLIO("RHS_in11_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in12_L0 = new PLIO("RHS_in12_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in13_L0 = new PLIO("RHS_in13_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in14_L0 = new PLIO("RHS_in14_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in15_L0 = new PLIO("RHS_in15_L0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L1 = new PLIO("RHS_in0_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L1 = new PLIO("RHS_in1_L1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* out0_L0 = new PLIO("out0_L0", adf::plio_128_bits, "data/output0.txt",230);
PLIO* out1_L0 = new PLIO("out1_L0", adf::plio_128_bits, "data/output1.txt",230);
PLIO* out2_L0 = new PLIO("out2_L0", adf::plio_128_bits, "data/output2.txt",230);
PLIO* out3_L0 = new PLIO("out3_L0", adf::plio_128_bits, "data/output3.txt",230);
PLIO* out4_L0 = new PLIO("out4_L0", adf::plio_128_bits, "data/output4.txt",230);
PLIO* out5_L0 = new PLIO("out5_L0", adf::plio_128_bits, "data/output5.txt",230);
PLIO* out6_L0 = new PLIO("out6_L0", adf::plio_128_bits, "data/output6.txt",230);
PLIO* out7_L0 = new PLIO("out7_L0", adf::plio_128_bits, "data/output7.txt",230);
PLIO* out8_L0 = new PLIO("out8_L0", adf::plio_128_bits, "data/output8.txt",230);
PLIO* out9_L0 = new PLIO("out9_L0", adf::plio_128_bits, "data/output9.txt",230);
PLIO* out10_L0 = new PLIO("out10_L0", adf::plio_128_bits, "data/output10.txt",230);
PLIO* out11_L0 = new PLIO("out11_L0", adf::plio_128_bits, "data/output11.txt",230);
PLIO* out12_L0 = new PLIO("out12_L0", adf::plio_128_bits, "data/output12.txt",230);
PLIO* out13_L0 = new PLIO("out13_L0", adf::plio_128_bits, "data/output13.txt",230);
PLIO* out14_L0 = new PLIO("out14_L0", adf::plio_128_bits, "data/output14.txt",230);
PLIO* out15_L0 = new PLIO("out15_L0", adf::plio_128_bits, "data/output15.txt",230);

PLIO* out0_L1 = new PLIO("out0_L1", adf::plio_128_bits, "data/output16.txt",230);
PLIO* out1_L1 = new PLIO("out1_L1", adf::plio_128_bits, "data/output17.txt",230);

simulation::platform<28, 18> platform(
LHS_in0_L0,
LHS_in1_L0,
LHS_in2_L0,
LHS_in3_L0,
LHS_in4_L0,
LHS_in5_L0,
LHS_in6_L0,
LHS_in7_L0,
LHS_in0_L1,
LHS_in1_L1,
RHS_in0_L0, 
RHS_in1_L0, 
RHS_in2_L0, 
RHS_in3_L0, 
RHS_in4_L0, 
RHS_in5_L0, 
RHS_in6_L0, 
RHS_in7_L0, 
RHS_in8_L0, 
RHS_in9_L0, 
RHS_in10_L0, 
RHS_in11_L0, 
RHS_in12_L0, 
RHS_in13_L0, 
RHS_in14_L0, 
RHS_in15_L0, 
RHS_in0_L1, 
RHS_in1_L1, 
out0_L0,
out1_L0,
out2_L0,
out3_L0,
out4_L0,
out5_L0,
out6_L0,
out7_L0,
out8_L0,
out9_L0,
out10_L0,
out11_L0,
out12_L0,
out13_L0,
out14_L0,
out15_L0,
out0_L1,
out1_L1 
);


mm_x4_x3_x8_graph0  mm_graph0;
mm_x2_x3_x2_graph1  mm_graph1;


connect<> net_lhs_in0_L0 (platform.src[0], mm_graph0.in_lhs[0][0]);
connect<> net_lhs_in1_L0 (platform.src[1], mm_graph0.in_lhs[1][0]);
connect<> net_lhs_in2_L0 (platform.src[2], mm_graph0.in_lhs[2][0]);
connect<> net_lhs_in3_L0 (platform.src[3], mm_graph0.in_lhs[3][0]);
connect<> net_lhs_in4_L0 (platform.src[4], mm_graph0.in_lhs[4][0]);
connect<> net_lhs_in5_L0 (platform.src[5], mm_graph0.in_lhs[5][0]);
connect<> net_lhs_in6_L0 (platform.src[6], mm_graph0.in_lhs[6][0]);
connect<> net_lhs_in7_L0 (platform.src[7], mm_graph0.in_lhs[7][0]);

connect<> net_lhs_in0_L1 (platform.src[8], mm_graph1.in_lhs[0][0]);
connect<> net_lhs_in1_L1 (platform.src[9], mm_graph1.in_lhs[1][0]);

connect<> net_rhs_in0_L0 (platform.src[10], mm_graph0.in_rhs[0][0]);
connect<> net_rhs_in1_L0 (platform.src[11], mm_graph0.in_rhs[1][0]);
connect<> net_rhs_in2_L0 (platform.src[12], mm_graph0.in_rhs[2][0]);
connect<> net_rhs_in3_L0 (platform.src[13], mm_graph0.in_rhs[3][0]);
connect<> net_rhs_in4_L0 (platform.src[14], mm_graph0.in_rhs[4][0]);
connect<> net_rhs_in5_L0 (platform.src[15], mm_graph0.in_rhs[5][0]);
connect<> net_rhs_in6_L0 (platform.src[16], mm_graph0.in_rhs[6][0]);
connect<> net_rhs_in7_L0 (platform.src[17], mm_graph0.in_rhs[7][0]);
connect<> net_rhs_in8_L0 (platform.src[18], mm_graph0.in_rhs[8][0]);
connect<> net_rhs_in9_L0 (platform.src[19], mm_graph0.in_rhs[9][0]);
connect<> net_rhs_in10_L0 (platform.src[20], mm_graph0.in_rhs[10][0]);
connect<> net_rhs_in11_L0 (platform.src[21], mm_graph0.in_rhs[11][0]);
connect<> net_rhs_in12_L0 (platform.src[22], mm_graph0.in_rhs[12][0]);
connect<> net_rhs_in13_L0 (platform.src[23], mm_graph0.in_rhs[13][0]);
connect<> net_rhs_in14_L0 (platform.src[24], mm_graph0.in_rhs[14][0]);
connect<> net_rhs_in15_L0 (platform.src[25], mm_graph0.in_rhs[15][0]);
connect<> net_rhs_in0_L1 (platform.src[26], mm_graph1.in_rhs[0][0]);
connect<> net_rhs_in1_L1 (platform.src[27], mm_graph1.in_rhs[1][0]);
connect<> net_out0_L0 (mm_graph0.out[0], platform.sink[0]);
connect<> net_out1_L0 (mm_graph0.out[1], platform.sink[1]);
connect<> net_out2_L0 (mm_graph0.out[2], platform.sink[2]);
connect<> net_out3_L0 (mm_graph0.out[3], platform.sink[3]);
connect<> net_out4_L0 (mm_graph0.out[4], platform.sink[4]);
connect<> net_out5_L0 (mm_graph0.out[5], platform.sink[5]);
connect<> net_out6_L0 (mm_graph0.out[6], platform.sink[6]);
connect<> net_out7_L0 (mm_graph0.out[7], platform.sink[7]);
connect<> net_out8_L0 (mm_graph0.out[8], platform.sink[8]);
connect<> net_out9_L0 (mm_graph0.out[9], platform.sink[9]);
connect<> net_out10_L0 (mm_graph0.out[10], platform.sink[10]);
connect<> net_out11_L0 (mm_graph0.out[11], platform.sink[11]);
connect<> net_out12_L0 (mm_graph0.out[12], platform.sink[12]);
connect<> net_out13_L0 (mm_graph0.out[13], platform.sink[13]);
connect<> net_out14_L0 (mm_graph0.out[14], platform.sink[14]);
connect<> net_out15_L0 (mm_graph0.out[15], platform.sink[15]);
connect<> net_out0_L1 (mm_graph1.out[0], platform.sink[16]);
connect<> net_out1_L1 (mm_graph1.out[1], platform.sink[17]);
#ifdef __AIESIM__
int main(int argc, char** argv) {
    mm_graph0.init();
    mm_graph1.init();
    mm_graph0.run(4);
    mm_graph1.run(4);
    mm_graph0.end();
    mm_graph1.end();
    return 0;
}
#endif