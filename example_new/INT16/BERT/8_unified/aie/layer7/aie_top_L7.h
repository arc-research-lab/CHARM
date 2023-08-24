#include "aie_graph_L7.h"

using namespace adf;

const int L7_A=2;
const int L7_B=3;
const int L7_C=4;
const int L7_A_BRO=2;
const int L7_C_BRO=2;

const int L7_NUM_INSTANCES=L7_A*L7_C;
const int L7_NUM_PACK_OUT=L7_NUM_INSTANCES/2;

class mm_x2_x3_x4_graph7 : public adf::graph {
public:
    input_port in_lhs[(L7_A*L7_C/L7_A_BRO)][L7_NUM_PACK_IN];
    input_port in_rhs[(L7_A*L7_C/L7_C_BRO)][L7_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L7_NUM_PACK_OUT];
	output_port out[L7_NUM_PACK_OUT];

    
    mm_k1_B3_L7 <43, 0>  mm_0_0;
    mm_k1_B3_L7 <45, 1>  mm_0_2;
    mm_k1_B3_L7 <43, 2>  mm_0_1;
    mm_k1_B3_L7 <45, 3>  mm_0_3;
    mm_k1_B3_L7 <46, 0>  mm_1_0;
    mm_k1_B3_L7 <48, 1>  mm_1_2;
    mm_k1_B3_L7 <46, 2>  mm_1_1;
    mm_k1_B3_L7 <48, 3>  mm_1_3;
    



    mm_x2_x3_x4_graph7 () {
        for (int i =0; i<L7_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[1][0],mm_0_2.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[1][0],mm_0_3.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[2][0],mm_1_0.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[2][0],mm_1_1.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[3][0],mm_1_2.in0[0]);
        connect<pktstream,window<L7_h1*L7_w1*L7_Byte>>(in_lhs[3][0],mm_1_3.in0[0]);
        

        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[1][0],mm_0_1.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[1][0],mm_1_1.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[2][0],mm_0_2.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[2][0],mm_1_2.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[3][0],mm_0_3.in1[0]);
        connect<pktstream,window<L7_w1*L7_w2*L7_Byte>>(in_rhs[3][0],mm_1_3.in1[0]);
        

        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_0_2.out,mg_out[1].in[0]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_0_3.out,mg_out[1].in[1]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_1_0.out,mg_out[2].in[0]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_1_1.out,mg_out[2].in[1]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_1_2.out,mg_out[3].in[0]);
        connect<window<L7_h1*L7_w2*L7_Byte>,pktstream>(mm_1_3.out,mg_out[3].in[1]);
        

        for (int i=0; i<L7_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};