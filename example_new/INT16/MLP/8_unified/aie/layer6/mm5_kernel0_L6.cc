#include <adf.h>
#include <stdio.h>
#include "para_L6.h"
    
void mm5_kernel0_L6(input_window_int16* __restrict matA,
        input_window_int16* __restrict matB,
        output_stream_acc48* __restrict matC){

    
    v32int16 chess_storage(xa) buf_matA0 = undef_v32int16();
    v32int16 chess_storage(xb) buf_matA1 = undef_v32int16();
    v16int16 chess_storage(wc0) buf_matB0=undef_v16int16();
    v16int16 chess_storage(wc1) buf_matB1=undef_v16int16();

    buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));  
    window_incr(matA,L6_h1);                                
    buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));  
    window_incr(matA,L6_h1);

    buf_matB0 = upd_v(buf_matB0,0,window_read_v8(matB));  
    window_incr(matB,L6_w1);
    buf_matB0 = upd_v(buf_matB0,1,window_read_v8(matB));  
    window_decr(matB,L6_jump_B0);
    
    for (unsigned int i=0;i<L6_boundary_i;i++)  //i/16
    chess_prepare_for_pipelining
    chess_loop_range(L6_boundary_i,)
    {
        for (unsigned int j=0;j<L6_boundary_j;j++)  // j/2
        chess_prepare_for_pipelining
        chess_loop_range(L6_boundary_j,)
        {
            v16acc48 acc0=null_v16acc48();
            v16acc48 acc1=null_v16acc48();
            int jump=L6_h1;
            if (j==L6_judge_j){
                jump=L6_jump_A0;
            }
            else{
                jump=L6_h1;
            }

            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,0,0x0,0x0,1);
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            buf_matB1 = upd_v(buf_matB1,0,window_read_v8(matB));   	
            window_incr(matB,L6_w1); 
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,8,0x0,0x0,1);
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            buf_matB1 = upd_v(buf_matB1,1,window_read_v8(matB));   
            window_decr(matB,L6_jump_B0); 
        
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,2,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,10,0x0,0x0,1);
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
        
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,4,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,12,0x0,0x0,1); 
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
        
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,6,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,14,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
        
        
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,0,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,8,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            
        
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,2,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,10,0x0,0x0,1);
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
        
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,4,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,12,0x0,0x0,1); 
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
        
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,6,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            buf_matB0=upd_v(buf_matB0,0,window_read_v8(matB));     
            window_incr(matA,L6_h1);
            window_incr(matB,L6_w1);
        
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,14,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            buf_matB0=upd_v(buf_matB0,1,window_read_v8(matB));  
            window_incr(matA,L6_h1);
            window_decr(matB,L6_jump_B0);
            
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,0,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            buf_matB1 = upd_v(buf_matB1,0,window_read_v8(matB));   
            window_incr(matA,L6_h1);
            window_incr(matB,L6_w1); 
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,8,0x0,0x0,1);  
            buf_matB1 = upd_v(buf_matB1,1,window_read_v8(matB));   
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            window_incr(matB,8);
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,2,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,10,0x0,0x0,1);
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
    
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,4,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB0,12,0x0,0x0,1); 
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
    
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,6,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB0,14,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
    
    
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,0,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,8,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            
    
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,2,0x0,0x0,1); 
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,10,0x0,0x0,1);
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            window_incr(matA,L6_h1);
    
            acc0 = mac16(acc0,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,4,0x0,0x0,1);  
            buf_matA1=upd_w(buf_matA1,0,window_read_v16(matA));    
            window_incr(matA,L6_h1);
            acc1 = mac16(acc1,buf_matA0,0,0x73727170,0x77767574,0x3120,buf_matB1,12,0x0,0x0,1); 
            buf_matA1=upd_w(buf_matA1,1,window_read_v16(matA));    
            window_incr(matA,jump);
    
    
            acc0 = mac16(acc0,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,6,0x0,0x0,1); 
            writeincr_v8(matC,ext_lo(acc0));
            writeincr_v8(matC,ext_hi(acc0));
    
            buf_matA0=upd_w(buf_matA0,0,window_read_v16(matA));    
            buf_matB0=upd_v(buf_matB0,0,window_read_v8(matB));     
            window_incr(matA,L6_h1);
            window_incr(matB,L6_w1);
    
            acc1 = mac16(acc1,buf_matA1,0,0x73727170,0x77767574,0x3120,buf_matB1,14,0x0,0x0,1); 
            writeincr_v8(matC,ext_lo(acc1));
            writeincr_v8(matC,ext_hi(acc1));
    
            buf_matA0=upd_w(buf_matA0,1,window_read_v16(matA));    
            buf_matB0=upd_v(buf_matB0,1,window_read_v8(matB));  
            window_incr(matA,L6_h1);
            window_decr(matB,L6_jump_B0); 
        }
    }	
}