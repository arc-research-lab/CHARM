#include "layer0/aie_top_L0.h"
using namespace adf;

PLIO* LHS_in0_L0 = new PLIO("LHS_in0_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L0 = new PLIO("LHS_in1_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L0 = new PLIO("LHS_in2_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L0 = new PLIO("LHS_in3_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in4_L0 = new PLIO("LHS_in4_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in5_L0 = new PLIO("LHS_in5_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in6_L0 = new PLIO("LHS_in6_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in7_L0 = new PLIO("LHS_in7_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in8_L0 = new PLIO("LHS_in8_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in9_L0 = new PLIO("LHS_in9_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in10_L0 = new PLIO("LHS_in10_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in11_L0 = new PLIO("LHS_in11_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in12_L0 = new PLIO("LHS_in12_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in13_L0 = new PLIO("LHS_in13_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in14_L0 = new PLIO("LHS_in14_L0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in15_L0 = new PLIO("LHS_in15_L0", adf::plio_128_bits, "data/input0.txt",230);

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
PLIO* out0_L0 = new PLIO("out0_L0", adf::plio_128_bits, "data/output0.txt",230);
PLIO* out1_L0 = new PLIO("out1_L0", adf::plio_128_bits, "data/output1.txt",230);
PLIO* out2_L0 = new PLIO("out2_L0", adf::plio_128_bits, "data/output2.txt",230);
PLIO* out3_L0 = new PLIO("out3_L0", adf::plio_128_bits, "data/output3.txt",230);
PLIO* out4_L0 = new PLIO("out4_L0", adf::plio_128_bits, "data/output4.txt",230);
PLIO* out5_L0 = new PLIO("out5_L0", adf::plio_128_bits, "data/output5.txt",230);
PLIO* out6_L0 = new PLIO("out6_L0", adf::plio_128_bits, "data/output6.txt",230);
PLIO* out7_L0 = new PLIO("out7_L0", adf::plio_128_bits, "data/output7.txt",230);

simulation::platform<32, 8> platform(
LHS_in0_L0,
LHS_in1_L0,
LHS_in2_L0,
LHS_in3_L0,
LHS_in4_L0,
LHS_in5_L0,
LHS_in6_L0,
LHS_in7_L0,
LHS_in8_L0,
LHS_in9_L0,
LHS_in10_L0,
LHS_in11_L0,
LHS_in12_L0,
LHS_in13_L0,
LHS_in14_L0,
LHS_in15_L0,
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
out0_L0,
out1_L0,
out2_L0,
out3_L0,
out4_L0,
out5_L0,
out6_L0,
out7_L0 
);


mm_x4_x4_x4_graph0  mm_graph0;


connect<> net_lhs_in0_L0 (platform.src[0], mm_graph0.in_lhs[0][0]);
connect<> net_lhs_in1_L0 (platform.src[1], mm_graph0.in_lhs[0][1]);
connect<> net_lhs_in2_L0 (platform.src[2], mm_graph0.in_lhs[1][0]);
connect<> net_lhs_in3_L0 (platform.src[3], mm_graph0.in_lhs[1][1]);
connect<> net_lhs_in4_L0 (platform.src[4], mm_graph0.in_lhs[2][0]);
connect<> net_lhs_in5_L0 (platform.src[5], mm_graph0.in_lhs[2][1]);
connect<> net_lhs_in6_L0 (platform.src[6], mm_graph0.in_lhs[3][0]);
connect<> net_lhs_in7_L0 (platform.src[7], mm_graph0.in_lhs[3][1]);
connect<> net_lhs_in8_L0 (platform.src[8], mm_graph0.in_lhs[4][0]);
connect<> net_lhs_in9_L0 (platform.src[9], mm_graph0.in_lhs[4][1]);
connect<> net_lhs_in10_L0 (platform.src[10], mm_graph0.in_lhs[5][0]);
connect<> net_lhs_in11_L0 (platform.src[11], mm_graph0.in_lhs[5][1]);
connect<> net_lhs_in12_L0 (platform.src[12], mm_graph0.in_lhs[6][0]);
connect<> net_lhs_in13_L0 (platform.src[13], mm_graph0.in_lhs[6][1]);
connect<> net_lhs_in14_L0 (platform.src[14], mm_graph0.in_lhs[7][0]);
connect<> net_lhs_in15_L0 (platform.src[15], mm_graph0.in_lhs[7][1]);

connect<> net_rhs_in0_L0 (platform.src[16], mm_graph0.in_rhs[0][0]);
connect<> net_rhs_in1_L0 (platform.src[17], mm_graph0.in_rhs[0][1]);
connect<> net_rhs_in2_L0 (platform.src[18], mm_graph0.in_rhs[1][0]);
connect<> net_rhs_in3_L0 (platform.src[19], mm_graph0.in_rhs[1][1]);
connect<> net_rhs_in4_L0 (platform.src[20], mm_graph0.in_rhs[2][0]);
connect<> net_rhs_in5_L0 (platform.src[21], mm_graph0.in_rhs[2][1]);
connect<> net_rhs_in6_L0 (platform.src[22], mm_graph0.in_rhs[3][0]);
connect<> net_rhs_in7_L0 (platform.src[23], mm_graph0.in_rhs[3][1]);
connect<> net_rhs_in8_L0 (platform.src[24], mm_graph0.in_rhs[4][0]);
connect<> net_rhs_in9_L0 (platform.src[25], mm_graph0.in_rhs[4][1]);
connect<> net_rhs_in10_L0 (platform.src[26], mm_graph0.in_rhs[5][0]);
connect<> net_rhs_in11_L0 (platform.src[27], mm_graph0.in_rhs[5][1]);
connect<> net_rhs_in12_L0 (platform.src[28], mm_graph0.in_rhs[6][0]);
connect<> net_rhs_in13_L0 (platform.src[29], mm_graph0.in_rhs[6][1]);
connect<> net_rhs_in14_L0 (platform.src[30], mm_graph0.in_rhs[7][0]);
connect<> net_rhs_in15_L0 (platform.src[31], mm_graph0.in_rhs[7][1]);
connect<> net_out0_L0 (mm_graph0.out[0], platform.sink[0]);
connect<> net_out1_L0 (mm_graph0.out[1], platform.sink[1]);
connect<> net_out2_L0 (mm_graph0.out[2], platform.sink[2]);
connect<> net_out3_L0 (mm_graph0.out[3], platform.sink[3]);
connect<> net_out4_L0 (mm_graph0.out[4], platform.sink[4]);
connect<> net_out5_L0 (mm_graph0.out[5], platform.sink[5]);
connect<> net_out6_L0 (mm_graph0.out[6], platform.sink[6]);
connect<> net_out7_L0 (mm_graph0.out[7], platform.sink[7]);
#ifdef __AIESIM__
int main(int argc, char** argv) {
    mm_graph0.init();
    mm_graph0.run(4);
    mm_graph0.end();
    return 0;
}
#endif