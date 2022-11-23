#include <adf.h>
#include <stdio.h>
#include "para.h"

void mm_kernel0(input_window_int16* __restrict matA,
		input_window_int16* __restrict matB,
		output_window_int16* __restrict matC){

	
	v32int16 chess_storage(xa) buf_matA0 = undef_v32int16();
	v32int16 chess_storage(xb) buf_matA1 = undef_v32int16();
	v16int16 chess_storage(wc0) buf_matB0=undef_v16int16();
	v16int16 chess_storage(wc1) buf_matB1=undef_v16int16();

	buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));  //0
	window_incr(matA,h1);                                
	buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));  //1
	window_incr(matA,h1);

	buf_matB0 = upd_v(buf_matB0,0,window_read_v8(matB));  //0
	window_incr(matB,w1);
	buf_matB0 = upd_v(buf_matB0,1,window_read_v8(matB));  //1
	window_decr(matB,w1-8);
	
	for (unsigned int i=0;i<boundary_i;i++)  //i/16
	chess_prepare_for_pipelining
	chess_loop_range(boundary_i,)
	{
		for (unsigned int j=0;j<boundary_j;j++)  // j/2
		chess_prepare_for_pipelining
		chess_loop_range(boundary_j,)
		{
			v16acc48 acc0=null_v16acc48();
			v16acc48 acc1=null_v16acc48();
			int jump=h1;
			if (j==judge_j){
				jump=h1+16;
			}
			else{
				jump=h1;
			}
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,0,0x0,0x0,1);  //0 1
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //2
			window_incr(matA,h1);
			buf_matB1 = upd_v(buf_matB1,0,window_read_v8(matB));   //2	
			window_incr(matB,w1); 
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,8,0x0,0x0,1);  //0 1
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //3
			window_incr(matA,h1);
			buf_matB1 = upd_v(buf_matB1,1,window_read_v8(matB));   //3
			window_decr(matB,w1-8); 
		
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,2,0x0,0x0,1); //2 3
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //4
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,10,0x0,0x0,1);//2 3
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //5
			window_incr(matA,h1);
		
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,4,0x0,0x0,1);  //4 5
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //6
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,12,0x0,0x0,1); //4 5
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //7
			window_incr(matA,h1);
		
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,6,0x0,0x0,1); //6 7
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //8
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,14,0x0,0x0,1); //6 7
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //9
			window_incr(matA,h1);
		
		
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,0,0x0,0x0,1);  //8 9
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //10
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,8,0x0,0x0,1);  //8 9
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //11
			window_incr(matA,h1);
				
		
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,2,0x0,0x0,1); //10 11
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //12
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,10,0x0,0x0,1);//10 11
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //13
			window_incr(matA,h1);
		
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,4,0x0,0x0,1);  //12 13
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //14
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,12,0x0,0x0,1); //12 13
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //15
			window_incr(matA,h1);
		
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,6,0x0,0x0,1); //14 15
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //0
			buf_matB0=upd_v(buf_matB0,0,window_read_v8(matB));     //0
			window_incr(matA,h1);
			window_incr(matB,w1);
		
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,14,0x0,0x0,1); //14 15
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //1
			buf_matB0=upd_v(buf_matB0,1,window_read_v8(matB));  //1
			window_incr(matA,h1);
			window_decr(matB,w1-8);
				
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,0,0x0,0x0,1);  //0 1
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //2
			buf_matB1 = upd_v(buf_matB1,0,window_read_v8(matB));   //2
			window_incr(matA,h1);
			window_incr(matB,w1); 
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,8,0x0,0x0,1);  //0 1
			buf_matB1 = upd_v(buf_matB1,1,window_read_v8(matB));   //3
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //3
			window_incr(matA,h1);
			window_incr(matB,8);
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,2,0x0,0x0,1); //2 3
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //4
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,10,0x0,0x0,1);//2 3
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //5
			window_incr(matA,h1);
	
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,4,0x0,0x0,1);  //4 5
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //6
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,12,0x0,0x0,1); //4 5
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //7
			window_incr(matA,h1);
	
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,6,0x0,0x0,1); //6 7
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //8
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,14,0x0,0x0,1); //6 7
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //9
			window_incr(matA,h1);
	
	
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,0,0x0,0x0,1);  //8 9
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //10
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,8,0x0,0x0,1);  //8 9
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //11
			window_incr(matA,h1);
			
	
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,2,0x0,0x0,1); //10 11
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //12
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,10,0x0,0x0,1);//10 11
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //13
			window_incr(matA,h1);
	
			acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,4,0x0,0x0,1);  //12 13
			buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    //14
			window_incr(matA,h1);
			acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,12,0x0,0x0,1); //12 13
			buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    //15
			window_incr(matA,jump);
	
	
			acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,6,0x0,0x0,1); //14 15
			window_write(matC,srs(acc0,0));
			window_incr(matC,h1);
	
			buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    //0
			buf_matB0=upd_v(buf_matB0,0,window_read_v8(matB));     //0
			window_incr(matA,h1);
			window_incr(matB,w1);
	
	
			acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,14,0x0,0x0,1); //14 15
			window_write(matC,srs(acc1,0));
			window_incr(matC,jump);                       //h1+16
	
			
			buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    //1
			buf_matB0=upd_v(buf_matB0,1,window_read_v8(matB));  //1
			window_incr(matA,h1);
			window_decr(matB,w1-8); 
		}
	}	
}