#include "mm_top.h"
using namespace adf;
#define COL_OFFSET 2


PLIO *in_r0  = new PLIO("in_r0",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r1  = new PLIO("in_r1",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r2  = new PLIO("in_r2",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r3  = new PLIO("in_r3",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r4  = new PLIO("in_r4",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r5  = new PLIO("in_r5",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r6  = new PLIO("in_r6",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r7  = new PLIO("in_r7",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r8  = new PLIO("in_r8",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r9  = new PLIO("in_r9",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r10  = new PLIO("in_r10",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r11  = new PLIO("in_r11",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r12  = new PLIO("in_r12",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r13  = new PLIO("in_r13",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r14  = new PLIO("in_r14",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r15  = new PLIO("in_r15",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r16  = new PLIO("in_r16",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r17  = new PLIO("in_r17",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r18  = new PLIO("in_r18",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r19  = new PLIO("in_r19",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r20  = new PLIO("in_r20",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r21  = new PLIO("in_r21",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r22  = new PLIO("in_r22",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_r23  = new PLIO("in_r23",  adf::plio_128_bits, "data/input0.txt", 250);

PLIO *in_c0  = new PLIO("in_c0",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c1  = new PLIO("in_c1",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c2  = new PLIO("in_c2",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c3  = new PLIO("in_c3",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c4  = new PLIO("in_c4",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c5  = new PLIO("in_c5",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c6  = new PLIO("in_c6",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c7  = new PLIO("in_c7",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c8  = new PLIO("in_c8",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c9  = new PLIO("in_c9",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c10  = new PLIO("in_c10",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c11  = new PLIO("in_c11",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c12  = new PLIO("in_c12",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c13  = new PLIO("in_c13",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c14  = new PLIO("in_c14",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c15  = new PLIO("in_c15",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c16  = new PLIO("in_c16",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c17  = new PLIO("in_c17",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c18  = new PLIO("in_c18",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c19  = new PLIO("in_c19",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c20  = new PLIO("in_c20",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c21  = new PLIO("in_c21",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c22  = new PLIO("in_c22",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c23  = new PLIO("in_c23",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c24  = new PLIO("in_c24",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c25  = new PLIO("in_c25",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c26  = new PLIO("in_c26",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c27  = new PLIO("in_c27",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c28  = new PLIO("in_c28",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c29  = new PLIO("in_c29",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c30  = new PLIO("in_c30",  adf::plio_128_bits, "data/input0.txt", 250);
PLIO *in_c31  = new PLIO("in_c31",  adf::plio_128_bits, "data/input0.txt", 250);
	
		
PLIO *out_r0 = new PLIO("out_r0", adf::plio_128_bits, "./data/output_r0.txt", 250);
PLIO *out_r1 = new PLIO("out_r1", adf::plio_128_bits, "./data/output_r1.txt", 250);
PLIO *out_r2 = new PLIO("out_r2", adf::plio_128_bits, "./data/output_r2.txt", 250);
PLIO *out_r3 = new PLIO("out_r3", adf::plio_128_bits, "./data/output_r3.txt", 250);
PLIO *out_r4 = new PLIO("out_r4", adf::plio_128_bits, "./data/output_r4.txt", 250);
PLIO *out_r5 = new PLIO("out_r5", adf::plio_128_bits, "./data/output_r5.txt", 250);
PLIO *out_r6 = new PLIO("out_r6", adf::plio_128_bits, "./data/output_r6.txt", 250);
PLIO *out_r7 = new PLIO("out_r7", adf::plio_128_bits, "./data/output_r7.txt", 250);
PLIO *out_r8 = new PLIO("out_r8", adf::plio_128_bits, "./data/output_r8.txt", 250);
PLIO *out_r9 = new PLIO("out_r9", adf::plio_128_bits, "./data/output_r9.txt", 250);
PLIO *out_r10 = new PLIO("out_r10", adf::plio_128_bits, "./data/output_r10.txt", 250);
PLIO *out_r11 = new PLIO("out_r11", adf::plio_128_bits, "./data/output_r11.txt", 250);
PLIO *out_r12 = new PLIO("out_r12", adf::plio_128_bits, "./data/output_r12.txt", 250);
PLIO *out_r13 = new PLIO("out_r13", adf::plio_128_bits, "./data/output_r13.txt", 250);
PLIO *out_r14 = new PLIO("out_r14", adf::plio_128_bits, "./data/output_r14.txt", 250);
PLIO *out_r15 = new PLIO("out_r15", adf::plio_128_bits, "./data/output_r15.txt", 250);
PLIO *out_r16 = new PLIO("out_r16", adf::plio_128_bits, "./data/output_r16.txt", 250);
PLIO *out_r17 = new PLIO("out_r17", adf::plio_128_bits, "./data/output_r17.txt", 250);
PLIO *out_r18 = new PLIO("out_r18", adf::plio_128_bits, "./data/output_r18.txt", 250);
PLIO *out_r19 = new PLIO("out_r19", adf::plio_128_bits, "./data/output_r19.txt", 250);
PLIO *out_r20 = new PLIO("out_r20", adf::plio_128_bits, "./data/output_r20.txt", 250);
PLIO *out_r21 = new PLIO("out_r21", adf::plio_128_bits, "./data/output_r21.txt", 250);
PLIO *out_r22 = new PLIO("out_r22", adf::plio_128_bits, "./data/output_r22.txt", 250);
PLIO *out_r23 = new PLIO("out_r23", adf::plio_128_bits, "./data/output_r23.txt", 250);
mm_x96_x4_graph<COL_OFFSET> myGraph;
	
	simulation::platform<56,24> platform(
                                  in_r0,
                                  in_r1,
                                  in_r2,
                                  in_r3,
                                  in_r4,
                                  in_r5,
                                  in_r6,
                                  in_r7,
                                  in_r8,
                                  in_r9,
                                  in_r10,
                                  in_r11,
                                  in_r12,
                                  in_r13,
                                  in_r14,
                                  in_r15,
                                  in_r16,
                                  in_r17,
                                  in_r18,
                                  in_r19,
                                  in_r20,
                                  in_r21,
                                  in_r22,
                                  in_r23,
                                  in_c0,
                                  in_c1,
                                  in_c2,
                                  in_c3,
                                  in_c4,
                                  in_c5,
                                  in_c6,
                                  in_c7,
                                  in_c8,
                                  in_c9,
                                  in_c10,
                                  in_c11,
                                  in_c12,
                                  in_c13,
                                  in_c14,
                                  in_c15,
                                  in_c16,
                                  in_c17,
                                  in_c18,
                                  in_c19,
                                  in_c20,
                                  in_c21,
                                  in_c22,
                                  in_c23,
                                  in_c24,
                                  in_c25,
                                  in_c26,
                                  in_c27,
                                  in_c28,
                                  in_c29,
                                  in_c30,
                                  in_c31,
                                  out_r0,
                                  out_r1,
                                  out_r2,
                                  out_r3,
                                  out_r4,
                                  out_r5,
                                  out_r6,
                                  out_r7,
                                  out_r8,
                                  out_r9,
                                  out_r10,
                                  out_r11,
                                  out_r12,
                                  out_r13,
                                  out_r14,
                                  out_r15,
                                  out_r16,
                                  out_r17,
                                  out_r18,
                                  out_r19,
                                  out_r20,
                                  out_r21,
                                  out_r22,
                                  out_r23
													);
connect<> netir0(platform.src[0], myGraph.in_row[0]);
connect<> netir1(platform.src[1], myGraph.in_row[1]);
connect<> netir2(platform.src[2], myGraph.in_row[2]);
connect<> netir3(platform.src[3], myGraph.in_row[3]);
connect<> netir4(platform.src[4], myGraph.in_row[4]);
connect<> netir5(platform.src[5], myGraph.in_row[5]);
connect<> netir6(platform.src[6], myGraph.in_row[6]);
connect<> netir7(platform.src[7], myGraph.in_row[7]);
connect<> netir8(platform.src[8], myGraph.in_row[8]);
connect<> netir9(platform.src[9], myGraph.in_row[9]);
connect<> netir10(platform.src[10], myGraph.in_row[10]);
connect<> netir11(platform.src[11], myGraph.in_row[11]);
connect<> netir12(platform.src[12], myGraph.in_row[12]);
connect<> netir13(platform.src[13], myGraph.in_row[13]);
connect<> netir14(platform.src[14], myGraph.in_row[14]);
connect<> netir15(platform.src[15], myGraph.in_row[15]);
connect<> netir16(platform.src[16], myGraph.in_row[16]);
connect<> netir17(platform.src[17], myGraph.in_row[17]);
connect<> netir18(platform.src[18], myGraph.in_row[18]);
connect<> netir19(platform.src[19], myGraph.in_row[19]);
connect<> netir20(platform.src[20], myGraph.in_row[20]);
connect<> netir21(platform.src[21], myGraph.in_row[21]);
connect<> netir22(platform.src[22], myGraph.in_row[22]);
connect<> netir23(platform.src[23], myGraph.in_row[23]);
 connect<> netic0(platform.src[24], myGraph.in_col[0]);
 connect<> netic1(platform.src[25], myGraph.in_col[1]);
 connect<> netic2(platform.src[26], myGraph.in_col[2]);
 connect<> netic3(platform.src[27], myGraph.in_col[3]);
 connect<> netic4(platform.src[28], myGraph.in_col[4]);
 connect<> netic5(platform.src[29], myGraph.in_col[5]);
 connect<> netic6(platform.src[30], myGraph.in_col[6]);
 connect<> netic7(platform.src[31], myGraph.in_col[7]);
 connect<> netic8(platform.src[32], myGraph.in_col[8]);
 connect<> netic9(platform.src[33], myGraph.in_col[9]);
connect<> netic10(platform.src[34], myGraph.in_col[10]);
connect<> netic11(platform.src[35], myGraph.in_col[11]);
connect<> netic12(platform.src[36], myGraph.in_col[12]);
connect<> netic13(platform.src[37], myGraph.in_col[13]);
connect<> netic14(platform.src[38], myGraph.in_col[14]);
connect<> netic15(platform.src[39], myGraph.in_col[15]);
connect<> netic16(platform.src[40], myGraph.in_col[16]);
connect<> netic17(platform.src[41], myGraph.in_col[17]);
connect<> netic18(platform.src[42], myGraph.in_col[18]);
connect<> netic19(platform.src[43], myGraph.in_col[19]);
connect<> netic20(platform.src[44], myGraph.in_col[20]);
connect<> netic21(platform.src[45], myGraph.in_col[21]);
connect<> netic22(platform.src[46], myGraph.in_col[22]);
connect<> netic23(platform.src[47], myGraph.in_col[23]);
connect<> netic24(platform.src[48], myGraph.in_col[24]);
connect<> netic25(platform.src[49], myGraph.in_col[25]);
connect<> netic26(platform.src[50], myGraph.in_col[26]);
connect<> netic27(platform.src[51], myGraph.in_col[27]);
connect<> netic28(platform.src[52], myGraph.in_col[28]);
connect<> netic29(platform.src[53], myGraph.in_col[29]);
connect<> netic30(platform.src[54], myGraph.in_col[30]);
connect<> netic31(platform.src[55], myGraph.in_col[31]);

connect<> netor0(myGraph.out[0],  platform.sink[0]);
connect<> netor1(myGraph.out[1],  platform.sink[1]);
connect<> netor2(myGraph.out[2],  platform.sink[2]);
connect<> netor3(myGraph.out[3],  platform.sink[3]);
connect<> netor4(myGraph.out[4],  platform.sink[4]);
connect<> netor5(myGraph.out[5],  platform.sink[5]);
connect<> netor6(myGraph.out[6],  platform.sink[6]);
connect<> netor7(myGraph.out[7],  platform.sink[7]);
connect<> netor8(myGraph.out[8],  platform.sink[8]);
connect<> netor9(myGraph.out[9],  platform.sink[9]);
connect<> netor10(myGraph.out[10],  platform.sink[10]);
connect<> netor11(myGraph.out[11],  platform.sink[11]);
connect<> netor12(myGraph.out[12],  platform.sink[12]);
connect<> netor13(myGraph.out[13],  platform.sink[13]);
connect<> netor14(myGraph.out[14],  platform.sink[14]);
connect<> netor15(myGraph.out[15],  platform.sink[15]);
connect<> netor16(myGraph.out[16],  platform.sink[16]);
connect<> netor17(myGraph.out[17],  platform.sink[17]);
connect<> netor18(myGraph.out[18],  platform.sink[18]);
connect<> netor19(myGraph.out[19],  platform.sink[19]);
connect<> netor20(myGraph.out[20],  platform.sink[20]);
connect<> netor21(myGraph.out[21],  platform.sink[21]);
connect<> netor22(myGraph.out[22],  platform.sink[22]);
connect<> netor23(myGraph.out[23],  platform.sink[23]);

#ifdef __AIESIM__

int main(void) {
  myGraph.init();
  myGraph.run(8);
  myGraph.end();
  return 0;
}

#endif
