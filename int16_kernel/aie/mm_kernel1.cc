#include <adf.h>
#include <stdio.h>
#include "para.h"

void mm_kernel1(input_window_int16* __restrict matA,
		input_window_int16* __restrict matB,
		input_window_int16* __restrict acc_in,
		output_window_int16* __restrict matC){

	v32int16 buf_matB=undef_v32int16();
	v64int16 buf_matA = undef_v64int16();

	buf_matB = upd_v(buf_matB,0,window_read_v8(matB));  //0
	
	window_incr(matB,w1);
	buf_matB = upd_v(buf_matB,1,window_read_v8(matB));  //1

	window_decr(matB,w1-8);    //w1-8
	buf_matA=upd_w(buf_matA,0,window_read_v16(matA));  //0
	window_incr(matA,h1);                                
	buf_matA=upd_w(buf_matA,1,window_read_v16(matA));  //1
	window_incr(matA,h1);

	v16acc48 acc2=ups(window_read_v16(acc_in),0);
	window_incr(acc_in,h1);
	v16acc48 acc3=ups(window_read_v16(acc_in),0);
	window_incr(acc_in,h1);
	for (unsigned int i=0;i<boundary_i;i++)  //i/16
	chess_prepare_for_pipelining
	chess_loop_range(boundary_i,)
	{
		for (unsigned int j=0;j<boundary_j;j++)  // j/2
	chess_prepare_for_pipelining
	chess_loop_range(boundary_j,)
		{
			int jump=h1;
			int jump_acc=h1;
			if (j==judge_j){
				jump=h1+16;
			}
			else if (j==judge_j-1){
				jump_acc=h1+16;
			}
			else{
				jump=h1;
			}
			v16acc48 acc0=acc2;
			v16acc48 acc1=acc3;
			for (unsigned int k=0;k<boundary_k;k++)  // k/16 - 1
		chess_prepare_for_pipelining
		chess_loop_range(boundary_k,)
			{	
				acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),0,0x0,0x0,1);  //0 1
				buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //2
				buf_matB = upd_v(buf_matB,2,window_read_v8(matB));   //2
				window_incr(matA,h1);
				window_incr(matB,w1); 
				acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),8,0x0,0x0,1);  //0 1
				buf_matB = upd_v(buf_matB,3,window_read_v8(matB));   //3
				buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //3
				window_incr(matA,h1);
				window_decr(matB,w1-8); 
		
				acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),2,0x0,0x0,1); //2 3
				buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //4
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),10,0x0,0x0,1);//2 3
				buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //5
				window_incr(matA,h1);
		
				acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),4,0x0,0x0,1);  //4 5
				buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //6
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),12,0x0,0x0,1); //4 5
				buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //7
				window_incr(matA,h1);
		
				acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),6,0x0,0x0,1); //6 7
				buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //8
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),14,0x0,0x0,1); //6 7
				buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //9
				window_incr(matA,h1);
		
		
				acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),0,0x0,0x0,1);  //8 9
				buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //10
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),8,0x0,0x0,1);  //8 9
				buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //11
				window_incr(matA,h1);
				
		
				acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),2,0x0,0x0,1); //10 11
				buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //12
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),10,0x0,0x0,1);//10 11
				buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //13
				window_incr(matA,h1);
		
				acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),4,0x0,0x0,1);  //12 13
				buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //14
				window_incr(matA,h1);
				acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),12,0x0,0x0,1); //12 13
				buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //15
				window_incr(matA,h1);
		
				acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),6,0x0,0x0,1); //14 15
				buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //0
				buf_matB=upd_v(buf_matB,0,window_read_v8(matB));     //0
				window_incr(matA,h1);
				window_incr(matB,w1);
		
		
				acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),14,0x0,0x0,1); //14 15
				buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //1
				buf_matB=upd_v(buf_matB,1,window_read_v8(matB));  //1
				window_incr(matA,h1);
				window_decr(matB,w1-8); 
				
			}
			acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),0,0x0,0x0,1);  //0 1
			buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //2
			buf_matB = upd_v(buf_matB,2,window_read_v8(matB));   //2
			window_incr(matA,h1);
			window_incr(matB,w1); 
			acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),8,0x0,0x0,1);  //0 1
			buf_matB = upd_v(buf_matB,3,window_read_v8(matB));   //3
			buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //3
			window_incr(matA,h1);
			window_incr(matB,8);
			acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),2,0x0,0x0,1); //2 3
			buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //4
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),10,0x0,0x0,1);//2 3
			buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //5
			window_incr(matA,h1);
	
			acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),4,0x0,0x0,1);  //4 5
			buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //6
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),12,0x0,0x0,1); //4 5
			buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //7
			window_incr(matA,h1);
	
			acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),6,0x0,0x0,1); //6 7
			buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //8
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,0),14,0x0,0x0,1); //6 7
			buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //9
			window_incr(matA,h1);
	
	
			acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),0,0x0,0x0,1);  //8 9
			buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //10
			window_incr(matA,h1);
			acc2=ups(window_read_v16(acc_in),0);
			window_incr(acc_in,h1);

			acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),8,0x0,0x0,1);  //8 9
			buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //11
			window_incr(matA,h1);
			acc3=ups(window_read_v16(acc_in),0);
			window_incr(acc_in,jump_acc);
			
			
	
			acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),2,0x0,0x0,1); //10 11
			buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //12
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),10,0x0,0x0,1);//10 11
			buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //13
			window_incr(matA,h1);
			
			
	
			acc0 = mac16(acc0,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),4,0x0,0x0,1);  //12 13
			buf_matA=upd_w(buf_matA,2,window_read_v16(matA));    //14
			window_incr(matA,h1);
			

			acc1 = mac16(acc1,buf_matA,0,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),12,0x0,0x0,1); //12 13
			buf_matA=upd_w(buf_matA,3,window_read_v16(matA));    //15
			window_incr(matA,jump);
			
	
			acc0 = mac16(acc0,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),6,0x0,0x0,1); //14 15
			window_write(matC,srs(acc0,0));
			window_incr(matC,h1);
	
			buf_matA=upd_w(buf_matA,0,window_read_v16(matA));    //0
			buf_matB=upd_v(buf_matB,0,window_read_v8(matB));     //0
			window_incr(matA,h1);
			window_incr(matB,w1);
	
	
			acc1 = mac16(acc1,buf_matA,32,0x73727170,0x77767574,0x3120,ext_w(buf_matB,1),14,0x0,0x0,1); //14 15
			window_write(matC,srs(acc1,0));
			window_incr(matC,jump);                       //h1+16
	
			
			buf_matA=upd_w(buf_matA,1,window_read_v16(matA));    //1
			buf_matB=upd_v(buf_matB,1,window_read_v8(matB));  //1
			window_incr(matA,h1);
			window_decr(matB,w1-8); 
		}
	}	
}
