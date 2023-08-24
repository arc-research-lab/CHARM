#include "aie_graph_L0.h"

using namespace adf;

const int L0_A=8;
const int L0_B=3;
const int L0_C=8;
const int L0_A_BRO=4;
const int L0_C_BRO=2;

const int L0_NUM_INSTANCES=L0_A*L0_C;
const int L0_NUM_PACK_OUT=L0_NUM_INSTANCES/2;

class mm_x8_x3_x8_graph0 : public adf::graph {
public:
    input_port in_lhs[(L0_A*L0_C/L0_A_BRO)][L0_NUM_PACK_IN];
    input_port in_rhs[(L0_A*L0_C/L0_C_BRO)][L0_NUM_PACK_IN];
    adf::pktmerge<2>  mg_out[L0_NUM_PACK_OUT];
	output_port out[L0_NUM_PACK_OUT];

    
    mm_k1_B3_L0 <6, 0>  mm_0_0;
    mm_k1_B3_L0 <8, 1>  mm_0_4;
    mm_k1_B3_L0 <6, 2>  mm_0_1;
    mm_k1_B3_L0 <8, 3>  mm_0_5;
    mm_k1_B3_L0 <6, 4>  mm_0_2;
    mm_k1_B3_L0 <8, 5>  mm_0_6;
    mm_k1_B3_L0 <6, 6>  mm_0_3;
    mm_k1_B3_L0 <8, 7>  mm_0_7;
    mm_k1_B3_L0 <9, 0>  mm_1_0;
    mm_k1_B3_L0 <11, 1>  mm_1_4;
    mm_k1_B3_L0 <9, 2>  mm_1_1;
    mm_k1_B3_L0 <11, 3>  mm_1_5;
    mm_k1_B3_L0 <9, 4>  mm_1_2;
    mm_k1_B3_L0 <11, 5>  mm_1_6;
    mm_k1_B3_L0 <9, 6>  mm_1_3;
    mm_k1_B3_L0 <11, 7>  mm_1_7;
    mm_k1_B3_L0 <12, 0>  mm_2_0;
    mm_k1_B3_L0 <14, 1>  mm_2_4;
    mm_k1_B3_L0 <12, 2>  mm_2_1;
    mm_k1_B3_L0 <14, 3>  mm_2_5;
    mm_k1_B3_L0 <12, 4>  mm_2_2;
    mm_k1_B3_L0 <14, 5>  mm_2_6;
    mm_k1_B3_L0 <12, 6>  mm_2_3;
    mm_k1_B3_L0 <14, 7>  mm_2_7;
    mm_k1_B3_L0 <15, 0>  mm_3_0;
    mm_k1_B3_L0 <17, 1>  mm_3_4;
    mm_k1_B3_L0 <15, 2>  mm_3_1;
    mm_k1_B3_L0 <17, 3>  mm_3_5;
    mm_k1_B3_L0 <15, 4>  mm_3_2;
    mm_k1_B3_L0 <17, 5>  mm_3_6;
    mm_k1_B3_L0 <15, 6>  mm_3_3;
    mm_k1_B3_L0 <17, 7>  mm_3_7;
    mm_k1_B3_L0 <18, 0>  mm_4_0;
    mm_k1_B3_L0 <20, 1>  mm_4_4;
    mm_k1_B3_L0 <18, 2>  mm_4_1;
    mm_k1_B3_L0 <20, 3>  mm_4_5;
    mm_k1_B3_L0 <18, 4>  mm_4_2;
    mm_k1_B3_L0 <20, 5>  mm_4_6;
    mm_k1_B3_L0 <18, 6>  mm_4_3;
    mm_k1_B3_L0 <20, 7>  mm_4_7;
    mm_k1_B3_L0 <21, 0>  mm_5_0;
    mm_k1_B3_L0 <23, 1>  mm_5_4;
    mm_k1_B3_L0 <21, 2>  mm_5_1;
    mm_k1_B3_L0 <23, 3>  mm_5_5;
    mm_k1_B3_L0 <21, 4>  mm_5_2;
    mm_k1_B3_L0 <23, 5>  mm_5_6;
    mm_k1_B3_L0 <21, 6>  mm_5_3;
    mm_k1_B3_L0 <23, 7>  mm_5_7;
    mm_k1_B3_L0 <24, 0>  mm_6_0;
    mm_k1_B3_L0 <26, 1>  mm_6_4;
    mm_k1_B3_L0 <24, 2>  mm_6_1;
    mm_k1_B3_L0 <26, 3>  mm_6_5;
    mm_k1_B3_L0 <24, 4>  mm_6_2;
    mm_k1_B3_L0 <26, 5>  mm_6_6;
    mm_k1_B3_L0 <24, 6>  mm_6_3;
    mm_k1_B3_L0 <26, 7>  mm_6_7;
    mm_k1_B3_L0 <27, 0>  mm_7_0;
    mm_k1_B3_L0 <29, 1>  mm_7_4;
    mm_k1_B3_L0 <27, 2>  mm_7_1;
    mm_k1_B3_L0 <29, 3>  mm_7_5;
    mm_k1_B3_L0 <27, 4>  mm_7_2;
    mm_k1_B3_L0 <29, 5>  mm_7_6;
    mm_k1_B3_L0 <27, 6>  mm_7_3;
    mm_k1_B3_L0 <29, 7>  mm_7_7;
    



    mm_x8_x3_x8_graph0 () {
        for (int i =0; i<L0_NUM_PACK_OUT; i++)  {
            mg_out[i] = adf::pktmerge<2>::create();
        }

        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[0][0],mm_0_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_0_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_0_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_0_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[1][0],mm_0_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_1_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_1_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_1_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[2][0],mm_1_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[3][0],mm_1_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[3][0],mm_1_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[3][0],mm_1_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[3][0],mm_1_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[4][0],mm_2_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[4][0],mm_2_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[4][0],mm_2_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[4][0],mm_2_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[5][0],mm_2_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[5][0],mm_2_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[5][0],mm_2_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[5][0],mm_2_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[6][0],mm_3_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[6][0],mm_3_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[6][0],mm_3_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[6][0],mm_3_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[7][0],mm_3_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[7][0],mm_3_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[7][0],mm_3_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[7][0],mm_3_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[8][0],mm_4_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[8][0],mm_4_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[8][0],mm_4_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[8][0],mm_4_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[9][0],mm_4_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[9][0],mm_4_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[9][0],mm_4_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[9][0],mm_4_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[10][0],mm_5_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[10][0],mm_5_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[10][0],mm_5_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[10][0],mm_5_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[11][0],mm_5_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[11][0],mm_5_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[11][0],mm_5_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[11][0],mm_5_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[12][0],mm_6_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[12][0],mm_6_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[12][0],mm_6_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[12][0],mm_6_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[13][0],mm_6_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[13][0],mm_6_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[13][0],mm_6_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[13][0],mm_6_7.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[14][0],mm_7_0.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[14][0],mm_7_1.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[14][0],mm_7_2.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[14][0],mm_7_3.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[15][0],mm_7_4.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[15][0],mm_7_5.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[15][0],mm_7_6.in0[0]);
        connect<pktstream,window<L0_h1*L0_w1*L0_Byte>>(in_lhs[15][0],mm_7_7.in0[0]);
        

        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[0][0],mm_0_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[0][0],mm_1_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[1][0],mm_2_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[1][0],mm_3_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[2][0],mm_4_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[2][0],mm_5_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[3][0],mm_6_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[3][0],mm_7_0.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[4][0],mm_0_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[4][0],mm_1_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[5][0],mm_2_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[5][0],mm_3_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[6][0],mm_4_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[6][0],mm_5_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[7][0],mm_6_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[7][0],mm_7_1.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[8][0],mm_0_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[8][0],mm_1_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[9][0],mm_2_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[9][0],mm_3_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[10][0],mm_4_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[10][0],mm_5_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[11][0],mm_6_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[11][0],mm_7_2.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[12][0],mm_0_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[12][0],mm_1_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[13][0],mm_2_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[13][0],mm_3_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[14][0],mm_4_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[14][0],mm_5_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[15][0],mm_6_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[15][0],mm_7_3.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[16][0],mm_0_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[16][0],mm_1_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[17][0],mm_2_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[17][0],mm_3_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[18][0],mm_4_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[18][0],mm_5_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[19][0],mm_6_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[19][0],mm_7_4.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[20][0],mm_0_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[20][0],mm_1_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[21][0],mm_2_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[21][0],mm_3_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[22][0],mm_4_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[22][0],mm_5_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[23][0],mm_6_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[23][0],mm_7_5.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[24][0],mm_0_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[24][0],mm_1_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[25][0],mm_2_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[25][0],mm_3_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[26][0],mm_4_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[26][0],mm_5_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[27][0],mm_6_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[27][0],mm_7_6.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[28][0],mm_0_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[28][0],mm_1_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[29][0],mm_2_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[29][0],mm_3_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[30][0],mm_4_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[30][0],mm_5_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[31][0],mm_6_7.in1[0]);
        connect<pktstream,window<L0_w1*L0_w2*L0_Byte>>(in_rhs[31][0],mm_7_7.in1[0]);
        

        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_0.out,mg_out[0].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_1.out,mg_out[0].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_2.out,mg_out[1].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_3.out,mg_out[1].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_4.out,mg_out[2].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_5.out,mg_out[2].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_6.out,mg_out[3].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_0_7.out,mg_out[3].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_0.out,mg_out[4].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_1.out,mg_out[4].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_2.out,mg_out[5].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_3.out,mg_out[5].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_4.out,mg_out[6].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_5.out,mg_out[6].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_6.out,mg_out[7].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_1_7.out,mg_out[7].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_0.out,mg_out[8].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_1.out,mg_out[8].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_2.out,mg_out[9].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_3.out,mg_out[9].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_4.out,mg_out[10].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_5.out,mg_out[10].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_6.out,mg_out[11].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_2_7.out,mg_out[11].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_0.out,mg_out[12].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_1.out,mg_out[12].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_2.out,mg_out[13].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_3.out,mg_out[13].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_4.out,mg_out[14].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_5.out,mg_out[14].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_6.out,mg_out[15].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_3_7.out,mg_out[15].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_0.out,mg_out[16].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_1.out,mg_out[16].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_2.out,mg_out[17].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_3.out,mg_out[17].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_4.out,mg_out[18].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_5.out,mg_out[18].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_6.out,mg_out[19].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_4_7.out,mg_out[19].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_0.out,mg_out[20].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_1.out,mg_out[20].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_2.out,mg_out[21].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_3.out,mg_out[21].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_4.out,mg_out[22].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_5.out,mg_out[22].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_6.out,mg_out[23].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_5_7.out,mg_out[23].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_0.out,mg_out[24].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_1.out,mg_out[24].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_2.out,mg_out[25].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_3.out,mg_out[25].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_4.out,mg_out[26].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_5.out,mg_out[26].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_6.out,mg_out[27].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_6_7.out,mg_out[27].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_0.out,mg_out[28].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_1.out,mg_out[28].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_2.out,mg_out[29].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_3.out,mg_out[29].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_4.out,mg_out[30].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_5.out,mg_out[30].in[1]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_6.out,mg_out[31].in[0]);
        connect<window<L0_h1*L0_w2*L0_Byte>,pktstream>(mm_7_7.out,mg_out[31].in[1]);
        

        for (int i=0; i<L0_NUM_PACK_OUT; i++)  {
            adf::connect<adf::pktstream> (mg_out[i].out[0], out[i]);
        }
    }
};