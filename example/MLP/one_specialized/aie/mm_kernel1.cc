#include <adf.h>
#include <stdio.h>
#include "para.h"
void mm_kernel1(input_window_float* __restrict matA,
		input_window_float* __restrict matB,
		input_window_float* __restrict acc_in,
		output_window_float* __restrict matC){

	v8float acc0=null_v8float();//For first output column
	v8float acc1=null_v8float();//For second output column
	v16float buf_matB = undef_v16float();
	v16float buf_matA = undef_v16float();

	buf_matB = upd_v(buf_matB,0,window_read_v4(matB));
	window_incr(matB,w1);
	buf_matB = upd_v(buf_matB,1,window_read_v4(matB));
	window_decr(matB,w1-4);

	buf_matA=upd_w(buf_matA,0,window_read_v8(matA));
	window_incr(matA,h1);
	
	for (unsigned int i=0;i<boundary_i;i++) 
	chess_prepare_for_pipelining
	chess_loop_range(boundary_i,)
	{	

		for (unsigned int j=0;j<boundary_j;j++)
		chess_prepare_for_pipelining
		chess_loop_range(boundary_j,){
			int jump=h1;
			if (j==judge_j){
				if(i==judge_i){
					jump=8;
				}
				else{
					jump=h1+8;
				}
			}
			else{
				jump=h1;
			}
			acc0=window_read_v8(acc_in);
			window_incr(acc_in,h1);
			acc1=window_read_v8(acc_in);
			window_incr(acc_in,jump);
			for (unsigned int k=0;k<boundary_k;k++)
			chess_prepare_for_pipelining
			chess_loop_range(boundary_k,)
			{
				acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,0),0,0x0); 
				buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
				window_incr(matA,h1);
				buf_matB = upd_v(buf_matB,2,window_read_v4(matB));
				window_incr(matB,w1);	
				acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,0),4,0x0); 
				buf_matB = upd_v(buf_matB,3,window_read_v4(matB));
				window_decr(matB,w1-4); 
		
				acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,0),1,0x0); 
				buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
				window_incr(matA,h1);
				acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,0),5,0x0);
			
				acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,0),2,0x0);
				buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
				window_incr(matA,h1);
				acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,0),6,0x0);
		
				acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,0),3,0x0);
				buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
				window_incr(matA,h1);
				acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,0),7,0x0);
		
		
				////////////////////////////////////////////////////////////////////////
				acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,1),0,0x0);
				buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
				window_incr(matA,h1);	
				acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,1),4,0x0);
				
		
				acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,1),1,0x0);
				buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
				window_incr(matA,h1);
				acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,1),5,0x0);
			
				acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,1),2,0x0);
				buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
				window_incr(matA,h1);
				acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,1),6,0x0);
		
				acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,1),3,0x0);
				//window_write(matC,srs(acc0,0));
				//window_incr(matC,h1);
				buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
				window_incr(matA,h1);
				buf_matB = upd_v(buf_matB,0,window_read_v4(matB));
				window_incr(matB,w1);
				acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,1),7,0x0);
				//window_write(matC,srs(acc1,0));
				//window_incr(matC,h1+8);
				buf_matB = upd_v(buf_matB,1,window_read_v4(matB));
				window_decr(matB,w1-4);
			}
			acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,0),0,0x0); 
			buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
			window_incr(matA,h1);
			buf_matB = upd_v(buf_matB,2,window_read_v4(matB));
			window_incr(matB,w1);	
			acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,0),4,0x0); 
			buf_matB = upd_v(buf_matB,3,window_read_v4(matB));
			window_incr(matB,4); 
	
			acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,0),1,0x0); 
			buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
			window_incr(matA,h1);
			acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,0),5,0x0);
		
			acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,0),2,0x0);
			buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
			window_incr(matA,h1);
			acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,0),6,0x0);
	
			acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,0),3,0x0);
			buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
			window_incr(matA,h1);
			acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,0),7,0x0);
	
	
			////////////////////////////////////////////////////////////////////////
			acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,1),0,0x0);
			buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
			window_incr(matA,h1);	
			acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,1),4,0x0);
			
	
			acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,1),1,0x0);
			buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
			window_incr(matA,h1);
			acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,1),5,0x0);
		
			acc0 = fpmac(acc0,buf_matA,0,0x76543210,ext_w(buf_matB,1),2,0x0);
			buf_matA = upd_w(buf_matA,1,window_read_v8(matA));
			window_incr(matA,jump);
			acc1 = fpmac(acc1,buf_matA,0,0x76543210,ext_w(buf_matB,1),6,0x0);
	
			acc0 = fpmac(acc0,buf_matA,8,0x76543210,ext_w(buf_matB,1),3,0x0);
			window_write(matC,acc0);
			window_incr(matC,h1);
			buf_matA = upd_w(buf_matA,0,window_read_v8(matA));
			window_incr(matA,h1);
			buf_matB = upd_v(buf_matB,0,window_read_v4(matB));
			window_incr(matB,w1);
			acc1 = fpmac(acc1,buf_matA,8,0x76543210,ext_w(buf_matB,1),7,0x0);
			window_write(matC,acc1);
			window_incr(matC,jump);
			buf_matB = upd_v(buf_matB,1,window_read_v4(matB));
			window_decr(matB,w1-4);	

			
		}
	}
}


