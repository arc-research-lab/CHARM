#include <adf.h>
#include <stdio.h>
#include "para.h"
void mm_kernel0(input_window_int8* __restrict matA,
		input_window_int8* __restrict matB,
		output_window_int8* __restrict matC){

	v64int8 chess_storage(xa) buf_matA0=undef_v64int8();
	v64int8 chess_storage(xb) buf_matA1=undef_v64int8();
			
	v32int8 chess_storage(wc0) buf_matB0=undef_v32int8(); 
	v32int8 chess_storage(wc1) buf_matB1=undef_v32int8(); 
	

	buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	window_incr(matA,jumpA0);

	buf_matB0=window_read_v32(matB);
	window_incr(matB,jumpB0);

	buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	window_incr(matA,jumpA0);

    v16acc48  acc2=null_v16acc48();//chess_storage(bm0) 
	v16acc48  acc3=null_v16acc48();//chess_storage(bm1) 

    for (int i=0;i<boundry_i;i++)
	chess_prepare_for_pipelining
	chess_loop_range(boundry_i,)
	{ //h1/8
        for (int j=0;j<boundry_j;j++)
	    chess_prepare_for_pipelining
	    chess_loop_range(boundry_j,)
        { //W2/4

            int jumpA=jumpA1;
            if(j==boundry_j-1){
				jumpA=jumpA0;
			}

            v16acc48  acc0=acc2;
            v16acc48  acc1=acc3;      

            //K iteration 0
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x88880000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,jumpB0);

            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB1,0,0x88880000,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);

            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,jumpA0);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            buf_matB0=window_read_v32(matB); 
	        window_incr(matB,jumpB0);

            //K iteration 1
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x88880000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,jumpB0);

            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB1,0,0x88880000,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);

            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,jumpA0);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            buf_matB0=window_read_v32(matB); 
	        window_incr(matB,jumpB0);

            //K iteration 2
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x88880000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,jumpB0);

            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB1,0,0x88880000,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);

            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            acc2=null_v16acc48();

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            buf_matB0=window_read_v32(matB); 
	        window_incr(matB,jumpB0);

            //K iteration 3
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x88880000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,jumpB0);

            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB1,0,0x88880000,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,jumpA);//jump
            acc3=null_v16acc48();

            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            window_write(matC,bsrs(acc0,0));
	        window_incr(matC,16);
            
	        

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC4444,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,jumpA0);
            buf_matB0=window_read_v32(matB); 
	        window_incr(matB,jumpB0);
            window_write(matC,bsrs(acc1,0));
	        window_incr(matC,16);
            
        }
    }

}