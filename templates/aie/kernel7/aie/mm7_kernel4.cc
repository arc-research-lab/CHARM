#include <adf.h>
#include <stdio.h>

const int h1=64;
const int w1=64;
const int w2=64;
const int boundry_i=h1/8;
const int boundry_j=w2/4;
const int boundry_k=w1/16-1;
const int jumpB0=w2*8;
const int jumpB1=w2*8+32;
const int jumpB2=32;
const int jumpA0=32;
const int jumpA1=(h1-8)*w1+32;


void mm_test(input_window_int8* __restrict matA,
		input_window_int8* __restrict matB,
		output_window_int8* __restrict matC){

	v64int8 buf_matA0=undef_v64int8();//chess_storage(xa)
	v64int8 buf_matA1=undef_v64int8();//chess_storage(xb)
			
	v32int8 buf_matB0=undef_v32int8();//chess_storage(wc0) 
	v32int8 buf_matB1=undef_v32int8();//chess_storage(wc1) 
	

	buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	window_incr(matA,jumpA0);

	buf_matB0=window_read_v32(matB);
	window_incr(matB,jumpB0);

	buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	window_incr(matA,jumpA0);
	
	for (int i=0;i<boundry_i;i++)
	//chess_prepare_for_pipelining
	//chess_loop_range(4,)
	{ //h1/8
		
		for (int j=0;j<boundry_j;j++)
		chess_prepare_for_pipelining
		chess_loop_range(boundry_j,){ //W2/4
			int jump;
			int jump1;
			if(j==boundry_j-1){
				jump=jumpA0;
				jump1=jumpB2;
			}
			else{
				jump=jumpA1;
				jump1=jumpB1;
			}
			

			v16acc48  chess_storage(bm0) acc0=null_v16acc48();//
			v16acc48  chess_storage(bm1) acc1=null_v16acc48();//
			for (int k=0;k<boundry_k;k++)
			chess_prepare_for_pipelining
			chess_loop_range(boundry_k,)
			{ //w1/16-1
				 
				acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
				buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
				window_incr(matA,jumpA0);
				buf_matB1=window_read_v32(matB);
				window_incr(matB,jumpB0);
			
				acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
				buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
				window_incr(matA,jumpA0);

			
				acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
				buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
				window_incr(matA,jumpA0);
				buf_matB0=window_read_v32(matB);
				window_incr(matB,jumpB0);
			
				acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
				buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
				window_incr(matA,jumpA0);
				
			}

			acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
			buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
			window_incr(matA,jumpA0);
			buf_matB1=window_read_v32(matB);
			window_incr(matB,jump1);
		
			acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
			buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
			window_incr(matA,jump);
		
			acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
			buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
			window_incr(matA,jumpA0);
			buf_matB0=window_read_v32(matB);
			window_incr(matB,jumpB0);
			window_write(matC,bsrs(acc0,0));
			window_incr(matC,16);
			
		
			acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
			buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
			window_incr(matA,jumpA0);
			window_write(matC,bsrs(acc1,0));
			window_incr(matC,16);
		}
	
	}
}