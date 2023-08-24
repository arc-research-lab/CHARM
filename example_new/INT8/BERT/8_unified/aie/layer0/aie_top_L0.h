#include "aie_graph_L0.h"

using namespace adf;

const int L0_A=3;
const int L0_B=2;
const int L0_C=2;
const int L0_A_BRO=2;
const int L0_C_BRO=3;

const int L0_NUM_INSTANCES=L0_A*L0_C;
const int L0_NUM_PACK_OUT=L0_NUM_INSTANCES/2;

class mm_x3_x2_x2_graph0 : public adf::graph {
public:
    input_port in_lhs[(L0_A*L0_C/L0_A_BRO)][L0_NUM_PACK_IN];
    input_port in_rhs[(L0_A*L0_C/L0_C_BRO)][L0_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L0_NUM_PACK_OUT];
	output_port out[L0_NUM_PACK_OUT];

    
    mm_k1_B2_L0 <6, 0>  mm_0_0;
    mm_k1_B2_L0 <7, 1>  mm_0_1;
    mm_k1_B2_L0 <6, 2>  mm_1_0;
    mm_k1_B2_L0 <7, 3>  mm_1_1;
    mm_k1_B2_L0 <6, 4>  mm_2_0;
    mm_k1_B2_L0 <7, 5>  mm_2_1;
    



    mm_x3_x2_x2_graph0 () {
        for (int i =0; i<L0_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_1_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_1_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_2_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_2_1.in0[0]);
        

        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[0][0],mm_2_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[1][0],mm_0_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[1][0],mm_1_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[1][0],mm_2_1.in1[0]);
        

        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_0.out,mg_out[1].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_1.out,mg_out[1].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_0.out,mg_out[2].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_1.out,mg_out[2].in[1]);
        

        for (int i=0; i<L0_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};