#include "aie_graph_L1.h"

using namespace adf;

const int L1_A=4;
const int L1_B=2;
const int L1_C=4;
const int L1_A_BRO=2;
const int L1_C_BRO=2;

const int L1_NUM_INSTANCES=L1_A*L1_C;
const int L1_NUM_PACK_OUT=L1_NUM_INSTANCES/2;

class mm_x4_x2_x4_graph1 : public adf::graph {
public:
    input_port in_lhs[(L1_A*L1_C/L1_A_BRO)][L1_NUM_PACK_IN];
    input_port in_rhs[(L1_A*L1_C/L1_C_BRO)][L1_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L1_NUM_PACK_OUT];
	output_port out[L1_NUM_PACK_OUT];

    
    mm_k1_B2_L1 <39, 4>  mm_0_0;
    mm_k1_B2_L1 <40, 5>  mm_0_2;
    mm_k1_B2_L1 <39, 6>  mm_0_1;
    mm_k1_B2_L1 <40, 7>  mm_0_3;
    mm_k1_B2_L1 <41, 4>  mm_1_0;
    mm_k1_B2_L1 <42, 5>  mm_1_2;
    mm_k1_B2_L1 <41, 6>  mm_1_1;
    mm_k1_B2_L1 <42, 7>  mm_1_3;
    mm_k1_B2_L1 <43, 4>  mm_2_0;
    mm_k1_B2_L1 <44, 5>  mm_2_2;
    mm_k1_B2_L1 <43, 6>  mm_2_1;
    mm_k1_B2_L1 <44, 7>  mm_2_3;
    mm_k1_B2_L1 <45, 4>  mm_3_0;
    mm_k1_B2_L1 <46, 5>  mm_3_2;
    mm_k1_B2_L1 <45, 6>  mm_3_1;
    mm_k1_B2_L1 <46, 7>  mm_3_3;
    



    mm_x4_x2_x4_graph1 () {
        for (int i =0; i<L1_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[1][0],mm_0_2.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[1][0],mm_0_3.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[2][0],mm_1_0.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[2][0],mm_1_1.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[3][0],mm_1_2.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[3][0],mm_1_3.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[4][0],mm_2_0.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[4][0],mm_2_1.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[5][0],mm_2_2.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[5][0],mm_2_3.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[6][0],mm_3_0.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[6][0],mm_3_1.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[7][0],mm_3_2.in0[0]);
        connect<pktstream,window<L1_h1*L1_w1*L1_Byte>>(in_lhs[7][0],mm_3_3.in0[0]);
        

        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[1][0],mm_2_0.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[1][0],mm_3_0.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[2][0],mm_0_1.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[2][0],mm_1_1.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[3][0],mm_2_1.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[3][0],mm_3_1.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[4][0],mm_0_2.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[4][0],mm_1_2.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[5][0],mm_2_2.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[5][0],mm_3_2.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[6][0],mm_0_3.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[6][0],mm_1_3.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[7][0],mm_2_3.in1[0]);
        connect<pktstream,window<L1_w1*L1_w2*L1_Byte>>(in_rhs[7][0],mm_3_3.in1[0]);
        

        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_0_2.out,mg_out[1].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_0_3.out,mg_out[1].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_1_0.out,mg_out[2].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_1_1.out,mg_out[2].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_1_2.out,mg_out[3].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_1_3.out,mg_out[3].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_2_0.out,mg_out[4].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_2_1.out,mg_out[4].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_2_2.out,mg_out[5].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_2_3.out,mg_out[5].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_3_0.out,mg_out[6].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_3_1.out,mg_out[6].in[1]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_3_2.out,mg_out[7].in[0]);
        connect<window<L1_h1*L1_w2*L1_Byte>,pktstream>(mm_3_3.out,mg_out[7].in[1]);
        

        for (int i=0; i<L1_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};