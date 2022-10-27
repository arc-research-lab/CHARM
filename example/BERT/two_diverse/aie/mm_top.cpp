#include "mm_top_large.h"
#include "mm_top_small.h"
using namespace adf;
#define COL_OFFSET_LARGE 0
#define ROW_OFFSET_LARGE 0
#define COL_OFFSET_SMALL 33
#define ROW_OFFSET_SMALL 0


PLIO *in_r0_large  = new PLIO("in_r0_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r1_large  = new PLIO("in_r1_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r2_large  = new PLIO("in_r2_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r3_large  = new PLIO("in_r3_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r4_large  = new PLIO("in_r4_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r5_large  = new PLIO("in_r5_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r6_large  = new PLIO("in_r6_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r7_large  = new PLIO("in_r7_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r8_large  = new PLIO("in_r8_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r9_large  = new PLIO("in_r9_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r10_large  = new PLIO("in_r10_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r11_large  = new PLIO("in_r11_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r12_large  = new PLIO("in_r12_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r13_large  = new PLIO("in_r13_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r14_large  = new PLIO("in_r14_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r15_large  = new PLIO("in_r15_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c0_large  = new PLIO("in_c0_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c1_large  = new PLIO("in_c1_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c2_large  = new PLIO("in_c2_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c3_large  = new PLIO("in_c3_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c4_large  = new PLIO("in_c4_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c5_large  = new PLIO("in_c5_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c6_large  = new PLIO("in_c6_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c7_large  = new PLIO("in_c7_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c8_large  = new PLIO("in_c8_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c9_large  = new PLIO("in_c9_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c10_large  = new PLIO("in_c10_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c11_large  = new PLIO("in_c11_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c12_large  = new PLIO("in_c12_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c13_large  = new PLIO("in_c13_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c14_large  = new PLIO("in_c14_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c15_large  = new PLIO("in_c15_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c16_large  = new PLIO("in_c16_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c17_large  = new PLIO("in_c17_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c18_large  = new PLIO("in_c18_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c19_large  = new PLIO("in_c19_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c20_large  = new PLIO("in_c20_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c21_large  = new PLIO("in_c21_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c22_large  = new PLIO("in_c22_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c23_large  = new PLIO("in_c23_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c24_large  = new PLIO("in_c24_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c25_large  = new PLIO("in_c25_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c26_large  = new PLIO("in_c26_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c27_large  = new PLIO("in_c27_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c28_large  = new PLIO("in_c28_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c29_large  = new PLIO("in_c29_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c30_large  = new PLIO("in_c30_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c31_large  = new PLIO("in_c31_large",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *out_0_large = new PLIO("out_0_large", adf::plio_128_bits, "./data/output_0_large.txt", 250);
PLIO *out_1_large = new PLIO("out_1_large", adf::plio_128_bits, "./data/output_1_large.txt", 250);
PLIO *out_2_large = new PLIO("out_2_large", adf::plio_128_bits, "./data/output_2_large.txt", 250);
PLIO *out_3_large = new PLIO("out_3_large", adf::plio_128_bits, "./data/output_3_large.txt", 250);
PLIO *out_4_large = new PLIO("out_4_large", adf::plio_128_bits, "./data/output_4_large.txt", 250);
PLIO *out_5_large = new PLIO("out_5_large", adf::plio_128_bits, "./data/output_5_large.txt", 250);
PLIO *out_6_large = new PLIO("out_6_large", adf::plio_128_bits, "./data/output_6_large.txt", 250);
PLIO *out_7_large = new PLIO("out_7_large", adf::plio_128_bits, "./data/output_7_large.txt", 250);
PLIO *out_8_large = new PLIO("out_8_large", adf::plio_128_bits, "./data/output_8_large.txt", 250);
PLIO *out_9_large = new PLIO("out_9_large", adf::plio_128_bits, "./data/output_9_large.txt", 250);
PLIO *out_10_large = new PLIO("out_10_large", adf::plio_128_bits, "./data/output_10_large.txt", 250);
PLIO *out_11_large = new PLIO("out_11_large", adf::plio_128_bits, "./data/output_11_large.txt", 250);
PLIO *out_12_large = new PLIO("out_12_large", adf::plio_128_bits, "./data/output_12_large.txt", 250);
PLIO *out_13_large = new PLIO("out_13_large", adf::plio_128_bits, "./data/output_13_large.txt", 250);
PLIO *out_14_large = new PLIO("out_14_large", adf::plio_128_bits, "./data/output_14_large.txt", 250);
PLIO *out_15_large = new PLIO("out_15_large", adf::plio_128_bits, "./data/output_15_large.txt", 250);
PLIO *in_r0_small  = new PLIO("in_r0_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r1_small  = new PLIO("in_r1_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r2_small  = new PLIO("in_r2_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r3_small  = new PLIO("in_r3_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r4_small  = new PLIO("in_r4_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r5_small  = new PLIO("in_r5_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r6_small  = new PLIO("in_r6_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_r7_small  = new PLIO("in_r7_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c0_small  = new PLIO("in_c0_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c1_small  = new PLIO("in_c1_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c2_small  = new PLIO("in_c2_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c3_small  = new PLIO("in_c3_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c4_small  = new PLIO("in_c4_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c5_small  = new PLIO("in_c5_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c6_small  = new PLIO("in_c6_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *in_c7_small  = new PLIO("in_c7_small",  adf::plio_128_bits, "data1/input0.txt", 250);
PLIO *out_0_small  = new PLIO("out_0_small", adf::plio_128_bits, "./data1/output_0_small.txt", 250);
PLIO *out_1_small  = new PLIO("out_1_small", adf::plio_128_bits, "./data1/output_1_small.txt", 250);
PLIO *out_2_small  = new PLIO("out_2_small", adf::plio_128_bits, "./data1/output_2_small.txt", 250);
PLIO *out_3_small  = new PLIO("out_3_small", adf::plio_128_bits, "./data1/output_3_small.txt", 250);
mm_x64_x4_graph<COL_OFFSET_LARGE,ROW_OFFSET_LARGE> myGraph_large;
mm_x16_x2_graph<COL_OFFSET_SMALL,ROW_OFFSET_SMALL> myGraph_small;
	
	simulation::platform<64,20> platform(
                                  in_r0_large,
                                  in_r1_large,
                                  in_r2_large,
                                  in_r3_large,
                                  in_r4_large,
                                  in_r5_large,
                                  in_r6_large,
                                  in_r7_large,
                                  in_r8_large,
                                  in_r9_large,
                                  in_r10_large,
                                  in_r11_large,
                                  in_r12_large,
                                  in_r13_large,
                                  in_r14_large,
                                  in_r15_large,
                                  in_c0_large,
                                  in_c1_large,
                                  in_c2_large,
                                  in_c3_large,
                                  in_c4_large,
                                  in_c5_large,
                                  in_c6_large,
                                  in_c7_large,
                                  in_c8_large,
                                  in_c9_large,
                                  in_c10_large,
                                  in_c11_large,
                                  in_c12_large,
                                  in_c13_large,
                                  in_c14_large,
                                  in_c15_large,
                                  in_c16_large,
                                  in_c17_large,
                                  in_c18_large,
                                  in_c19_large,
                                  in_c20_large,
                                  in_c21_large,
                                  in_c22_large,
                                  in_c23_large,
                                  in_c24_large,
                                  in_c25_large,
                                  in_c26_large,
                                  in_c27_large,
                                  in_c28_large,
                                  in_c29_large,
                                  in_c30_large,
                                  in_c31_large,
                                  in_r0_small,
                                  in_r1_small,
                                  in_r2_small,
                                  in_r3_small,
                                  in_r4_small,
                                  in_r5_small,
                                  in_r6_small,
                                  in_r7_small,
                                  in_c0_small,
                                  in_c1_small,
                                  in_c2_small,
                                  in_c3_small,
                                  in_c4_small,
                                  in_c5_small,
                                  in_c6_small,
                                  in_c7_small,
                                  out_0_large,
                                  out_1_large,
                                  out_2_large,
                                  out_3_large,
                                  out_4_large,
                                  out_5_large,
                                  out_6_large,
                                  out_7_large,
                                  out_8_large,
                                  out_9_large,
                                  out_10_large,
                                  out_11_large,
                                  out_12_large,
                                  out_13_large,
                                  out_14_large,
                                  out_15_large,
                                  out_0_small,
                                  out_1_small,
                                  out_2_small,
                                  out_3_small
													);
connect<> netir0_large(platform.src[0], myGraph_large.in_row[0]);
connect<> netir1_large(platform.src[1], myGraph_large.in_row[1]);
connect<> netir2_large(platform.src[2], myGraph_large.in_row[2]);
connect<> netir3_large(platform.src[3], myGraph_large.in_row[3]);
connect<> netir4_large(platform.src[4], myGraph_large.in_row[4]);
connect<> netir5_large(platform.src[5], myGraph_large.in_row[5]);
connect<> netir6_large(platform.src[6], myGraph_large.in_row[6]);
connect<> netir7_large(platform.src[7], myGraph_large.in_row[7]);
connect<> netir8_large(platform.src[8], myGraph_large.in_row[8]);
connect<> netir9_large(platform.src[9], myGraph_large.in_row[9]);
connect<> netir10_large(platform.src[10], myGraph_large.in_row[10]);
connect<> netir11_large(platform.src[11], myGraph_large.in_row[11]);
connect<> netir12_large(platform.src[12], myGraph_large.in_row[12]);
connect<> netir13_large(platform.src[13], myGraph_large.in_row[13]);
connect<> netir14_large(platform.src[14], myGraph_large.in_row[14]);
connect<> netir15_large(platform.src[15], myGraph_large.in_row[15]);
connect<> netic0_large(platform.src[16], myGraph_large.in_col[0]);
connect<> netic1_large(platform.src[17], myGraph_large.in_col[1]);
connect<> netic2_large(platform.src[18], myGraph_large.in_col[2]);
connect<> netic3_large(platform.src[19], myGraph_large.in_col[3]);
connect<> netic4_large(platform.src[20], myGraph_large.in_col[4]);
connect<> netic5_large(platform.src[21], myGraph_large.in_col[5]);
connect<> netic6_large(platform.src[22], myGraph_large.in_col[6]);
connect<> netic7_large(platform.src[23], myGraph_large.in_col[7]);
connect<> netic8_large(platform.src[24], myGraph_large.in_col[8]);
connect<> netic9_large(platform.src[25], myGraph_large.in_col[9]);
connect<> netic10_large(platform.src[26], myGraph_large.in_col[10]);
connect<> netic11_large(platform.src[27], myGraph_large.in_col[11]);
connect<> netic12_large(platform.src[28], myGraph_large.in_col[12]);
connect<> netic13_large(platform.src[29], myGraph_large.in_col[13]);
connect<> netic14_large(platform.src[30], myGraph_large.in_col[14]);
connect<> netic15_large(platform.src[31], myGraph_large.in_col[15]);
connect<> netic16_large(platform.src[32], myGraph_large.in_col[16]);
connect<> netic17_large(platform.src[33], myGraph_large.in_col[17]);
connect<> netic18_large(platform.src[34], myGraph_large.in_col[18]);
connect<> netic19_large(platform.src[35], myGraph_large.in_col[19]);
connect<> netic20_large(platform.src[36], myGraph_large.in_col[20]);
connect<> netic21_large(platform.src[37], myGraph_large.in_col[21]);
connect<> netic22_large(platform.src[38], myGraph_large.in_col[22]);
connect<> netic23_large(platform.src[39], myGraph_large.in_col[23]);
connect<> netic24_large(platform.src[40], myGraph_large.in_col[24]);
connect<> netic25_large(platform.src[41], myGraph_large.in_col[25]);
connect<> netic26_large(platform.src[42], myGraph_large.in_col[26]);
connect<> netic27_large(platform.src[43], myGraph_large.in_col[27]);
connect<> netic28_large(platform.src[44], myGraph_large.in_col[28]);
connect<> netic29_large(platform.src[45], myGraph_large.in_col[29]);
connect<> netic30_large(platform.src[46], myGraph_large.in_col[30]);
connect<> netic31_large(platform.src[47], myGraph_large.in_col[31]);
connect<> neto0_large(myGraph_large.out[0],  platform.sink[0]);
connect<> neto1_large(myGraph_large.out[1],  platform.sink[1]);
connect<> neto2_large(myGraph_large.out[2],  platform.sink[2]);
connect<> neto3_large(myGraph_large.out[3],  platform.sink[3]);
connect<> neto4_large(myGraph_large.out[4],  platform.sink[4]);
connect<> neto5_large(myGraph_large.out[5],  platform.sink[5]);
connect<> neto6_large(myGraph_large.out[6],  platform.sink[6]);
connect<> neto7_large(myGraph_large.out[7],  platform.sink[7]);
connect<> neto8_large(myGraph_large.out[8],  platform.sink[8]);
connect<> neto9_large(myGraph_large.out[9],  platform.sink[9]);
connect<> neto10_large(myGraph_large.out[10],  platform.sink[10]);
connect<> neto11_large(myGraph_large.out[11],  platform.sink[11]);
connect<> neto12_large(myGraph_large.out[12],  platform.sink[12]);
connect<> neto13_large(myGraph_large.out[13],  platform.sink[13]);
connect<> neto14_large(myGraph_large.out[14],  platform.sink[14]);
connect<> neto15_large(myGraph_large.out[15],  platform.sink[15]);
connect<> netir0_small(platform.src[48], myGraph_small.in_row[0]);
connect<> netir1_small(platform.src[49], myGraph_small.in_row[1]);
connect<> netir2_small(platform.src[50], myGraph_small.in_row[2]);
connect<> netir3_small(platform.src[51], myGraph_small.in_row[3]);
connect<> netir4_small(platform.src[52], myGraph_small.in_row[4]);
connect<> netir5_small(platform.src[53], myGraph_small.in_row[5]);
connect<> netir6_small(platform.src[54], myGraph_small.in_row[6]);
connect<> netir7_small(platform.src[55], myGraph_small.in_row[7]);
connect<> netic0_small(platform.src[56], myGraph_small.in_col[0]);
connect<> netic1_small(platform.src[57], myGraph_small.in_col[1]);
connect<> netic2_small(platform.src[58], myGraph_small.in_col[2]);
connect<> netic3_small(platform.src[59], myGraph_small.in_col[3]);
connect<> netic4_small(platform.src[60], myGraph_small.in_col[4]);
connect<> netic5_small(platform.src[61], myGraph_small.in_col[5]);
connect<> netic6_small(platform.src[62], myGraph_small.in_col[6]);
connect<> netic7_small(platform.src[63], myGraph_small.in_col[7]);
connect<> neto0_small(myGraph_small.out[0],  platform.sink[16]);
connect<> neto1_small(myGraph_small.out[1],  platform.sink[17]);
connect<> neto2_small(myGraph_small.out[2],  platform.sink[18]);
connect<> neto3_small(myGraph_small.out[3],  platform.sink[19]);

#ifdef __AIESIM__

int main(void) {
  myGraph_large.init();
  myGraph_small.init();
  myGraph_large.run(4);
  myGraph_small.run(4);
  myGraph_large.end();
  myGraph_small.end();
  return 0;
}

#endif
