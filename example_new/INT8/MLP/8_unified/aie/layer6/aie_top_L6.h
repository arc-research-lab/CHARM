#include "aie_graph_L6.h"

using namespace adf;

const int L6_A=3;
const int L6_B=2;
const int L6_C=2;
const int L6_A_BRO=2;
const int L6_C_BRO=3;

const int L6_NUM_INSTANCES=L6_A*L6_C;
const int L6_NUM_PACK_OUT=L6_NUM_INSTANCES/2;

class mm_x3_x2_x2_graph6 : public adf::graph {
public:
    input_port in_lhs[(L6_A*L6_C/L6_A_BRO)][L6_NUM_PACK_IN];
    input_port in_rhs[(L6_A*L6_C/L6_C_BRO)][L6_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L6_NUM_PACK_OUT];
	output_port out[L6_NUM_PACK_OUT];

    
    mm_k1_B2_L6 <30, 0>  mm_0_0;
    mm_k1_B2_L6 <31, 1>  mm_0_1;
    mm_k1_B2_L6 <30, 2>  mm_1_0;
    mm_k1_B2_L6 <31, 3>  mm_1_1;
    mm_k1_B2_L6 <30, 4>  mm_2_0;
    mm_k1_B2_L6 <31, 5>  mm_2_1;
    



    mm_x3_x2_x2_graph6 () {
        for (int i =0; i<L6_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[1][0],mm_1_0.in0[0]);
        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[1][0],mm_1_1.in0[0]);
        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[2][0],mm_2_0.in0[0]);
        connect<pktstream,window<L6_h1*L6_w1*L6_Byte>>(in_lhs[2][0],mm_2_1.in0[0]);
        

        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[0][0],mm_2_0.in1[0]);
        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[1][0],mm_0_1.in1[0]);
        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[1][0],mm_1_1.in1[0]);
        connect<pktstream,window<L6_w1*L6_w2*L6_Byte>>(in_rhs[1][0],mm_2_1.in1[0]);
        

        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_1_0.out,mg_out[1].in[0]);
        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_1_1.out,mg_out[1].in[1]);
        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_2_0.out,mg_out[2].in[0]);
        connect<window<L6_h1*L6_w2*L6_Byte>,pktstream>(mm_2_1.out,mg_out[2].in[1]);
        

        for (int i=0; i<L6_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};