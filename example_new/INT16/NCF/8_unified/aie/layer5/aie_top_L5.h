#include "aie_graph_L5.h"

using namespace adf;

const int L5_A=2;
const int L5_B=3;
const int L5_C=4;
const int L5_A_BRO=2;
const int L5_C_BRO=2;

const int L5_NUM_INSTANCES=L5_A*L5_C;
const int L5_NUM_PACK_OUT=L5_NUM_INSTANCES/2;

class mm_x2_x3_x4_graph5 : public adf::graph {
public:
    input_port in_lhs[(L5_A*L5_C/L5_A_BRO)][L5_NUM_PACK_IN];
    input_port in_rhs[(L5_A*L5_C/L5_C_BRO)][L5_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L5_NUM_PACK_OUT];
	output_port out[L5_NUM_PACK_OUT];

    
    mm_k1_B3_L5 <31, 0>  mm_0_0;
    mm_k1_B3_L5 <33, 1>  mm_0_2;
    mm_k1_B3_L5 <31, 2>  mm_0_1;
    mm_k1_B3_L5 <33, 3>  mm_0_3;
    mm_k1_B3_L5 <34, 0>  mm_1_0;
    mm_k1_B3_L5 <36, 1>  mm_1_2;
    mm_k1_B3_L5 <34, 2>  mm_1_1;
    mm_k1_B3_L5 <36, 3>  mm_1_3;
    



    mm_x2_x3_x4_graph5 () {
        for (int i =0; i<L5_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[1][0],mm_0_2.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[1][0],mm_0_3.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[2][0],mm_1_0.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[2][0],mm_1_1.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[3][0],mm_1_2.in0[0]);
        connect<pktstream,window<L5_h1*L5_w1*L5_Byte>>(in_lhs[3][0],mm_1_3.in0[0]);
        

        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[1][0],mm_0_1.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[1][0],mm_1_1.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[2][0],mm_0_2.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[2][0],mm_1_2.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[3][0],mm_0_3.in1[0]);
        connect<pktstream,window<L5_w1*L5_w2*L5_Byte>>(in_rhs[3][0],mm_1_3.in1[0]);
        

        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_0_2.out,mg_out[1].in[0]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_0_3.out,mg_out[1].in[1]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_1_0.out,mg_out[2].in[0]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_1_1.out,mg_out[2].in[1]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_1_2.out,mg_out[3].in[0]);
        connect<window<L5_h1*L5_w2*L5_Byte>,pktstream>(mm_1_3.out,mg_out[3].in[1]);
        

        for (int i=0; i<L5_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};