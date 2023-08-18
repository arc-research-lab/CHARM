#include <stdint.h>
#include "dma{{layer}}.hpp"

void address_A_ddr(axis_stream_32& addrA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    for(int tx=0;tx<TX;tx++){
        for(int tz=0;tz<TZ;tz++){
            for(int ty=0;ty<TY;ty++){
                ap_uint<32> pos;
                for(int j=0;j<K;j++){
                    for(int i=0;i<M/A_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(M/A_PER_TRA)*TX+ty*K*(M/A_PER_TRA)*TX+tx*(M/A_PER_TRA);
                        addrA_out.write(pos);
                    }
                }
            }
        }
    }
}

void loadA_ddr(ap_uint<AXI_WIDTH_A>* ina, axis_stream_32& addrA_in,axis_stream_A& dataA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<AXI_WIDTH_A> temp_data;
    int bound=boundA*TX*TY*TZ;
    for(int i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        int addr = addrA_in.read();
        temp_data=ina[addr];
        dataA_out.write(temp_data);        
    }
}

void address_B_ddr(axis_stream_32& addrB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off    
    for(int tx=0;tx<TX;tx++){
        for(int tz=0;tz<TZ;tz++){
            for(int ty=0;ty<TY;ty++){
                ap_uint<32> pos;
                for(int j=0;j<N;j++){
                    for(int i=0;i<K/B_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(K/B_PER_TRA)*TY+ty*(K/B_PER_TRA)+tz*N*(K/B_PER_TRA)*TY;
                        addrB_out.write(pos);
                    }
                }
            }
        }
    }
}


void loadB_ddr(ap_uint<AXI_WIDTH_B>* inb, axis_stream_32& addrB_in,axis_stream_B& dataB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<AXI_WIDTH_B> temp_data;
    int bound=boundB*TX*TY*TZ;
    for(int i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<32> addr = addrB_in.read();
        temp_data=inb[addr];
        dataB_out.write(temp_data);
    }  
}

void address_C_ddr(axis_stream_32& addrC_out,const int TX,const int TZ) {
#pragma HLS inline off
    for(int tx=0;tx<TX;tx++){
        for(int tz=0;tz<TZ;tz++){
            ap_uint<32> pos;
            for(int j=0;j<N;j++){
                for(int i=0;i<M/C_PER_TRA;i++){
                #pragma HLS PIPELINE II = 1
                    pos=i+j*(M/C_PER_TRA)*TX+tx*(M/C_PER_TRA)+tz*N*(M/C_PER_TRA)*TX;
                    addrC_out.write(pos);
                }
            }
        }
    }
}

void storeC_ddr(ap_uint<AXI_WIDTH_C>* outc, axis_stream_32& addrC_in,axis_stream_C& dataC_in,const int TX,const int TZ) { 
#pragma HLS inline off
    int bound=boundC*TX*TZ;
    for(int i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<AXI_WIDTH_C> temp_data=dataC_in.read();
        ap_uint<32> addr = addrC_in.read();
        outc[addr]=temp_data;
            
    }
    
}

void loadA(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACK_IN)][X*Y][PACK_IN*LEFT_SIZE],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A/2;a++){
                        for(int i=0;i<(H1*2/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=a_buf type=intra false
                            int temp= (k%(W1*PACK_IN))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=(2*a)*(B/PACK_IN)+(k/(W1*PACK_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            if(i==0){
                                a_buf[pos2][pos1][temp]=temp_data(127,0);
                                a_buf[pos2][pos1][temp+1]=temp_data(255,128);
                                a_buf[pos2][pos1][temp+2]=temp_data(383,256);
                                a_buf[pos2][pos1][temp+3]=temp_data(511,384);
                            }
                            else if(i==1){
                                a_buf[pos2][pos1][temp+4]=temp_data(127,0);
                                a_buf[pos2][pos1][temp+5]=temp_data(255,128);
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+0]=temp_data(383,256);
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+1]=temp_data(511,384);
                            }
                            else{
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+2]=temp_data(127,0);
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+3]=temp_data(255,128);
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+4]=temp_data(383,256);
                                a_buf[pos2+(B/PACK_IN)][pos1][temp+5]=temp_data(511,384);                       
                            }
                        }
                    }
                }
            }
        }
    }
}

void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACK_IN)*C][Z*Y][PACK_IN*RIGHT_SIZE],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                            #pragma HLS dependence variable=b_buf type=intra false
                                int pos0=m*4+w2*(W1/NUM_PER_TRA)+(b%PACK_IN)*RIGHT_SIZE;
                                int pos1=y+z*Y;
                                int pos2=(b/PACK_IN)+c*(B/PACK_IN);
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                                b_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                                b_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                            }  
                        }
                    }
                }
            }
        }
    }
}

void storeC(axis_stream_C& dataC_out, ap_uint<PLIO_WIDTH> c_buf[(C/PACK_OUT)*A][Z*X][PACK_OUT][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A/2;a++){
                            for (int n=0; n<H1*2/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                            #pragma HLS dependence variable=c_buf type=intra false
                                int temp=c+(2*a)*C; 
                                int pos0=w2*(H1/NUM_PER_TRA);
                                int pos1=temp%PACK_OUT;
                                int pos2=x+z*X;
                                int pos3=temp/PACK_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                if(n==0){
                                    temp_data(127,0)  =c_buf[pos3][pos2][pos1][pos0];
                                    temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+1];
                                    temp_data(383,256)=c_buf[pos3][pos2][pos1][pos0+2];
                                    temp_data(511,384)=c_buf[pos3][pos2][pos1][pos0+3];
                                }
                                else if(n==1){
                                    temp_data(127,0)  =c_buf[pos3][pos2][pos1][pos0+4];
                                    temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+5];
                                    temp_data(383,256)=c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0];
                                    temp_data(511,384)=c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0+1];
                                }
                                else{
                                    temp_data(127,0)  =c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0+2];
                                    temp_data(255,128)=c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0+3];
                                    temp_data(383,256)=c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0+4];
                                    temp_data(511,384)=c_buf[pos3+(C/PACK_OUT)][pos2][pos1][pos0+5];                     
                                }
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int pack=0;pack<PACK_OUT;pack++){
                for (int i = 0; i < OUT_SIZE/4; i++){
                #pragma HLS PIPELINE II = 1
                #pragma HLS dependence variable=c_buf type=intra false
                    for(int a = 0; a < (A*C/PACK_OUT); a++){
                        c_buf[a][x][pack][i*4+0]=0; 
                        c_buf[a][x][pack][i*4+1]=0;
                        c_buf[a][x][pack][i*4+2]=0; 
                        c_buf[a][x][pack][i*4+3]=0;
                    }
                }
            }
        }
    }
}

