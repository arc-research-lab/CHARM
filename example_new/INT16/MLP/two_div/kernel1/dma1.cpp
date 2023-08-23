#include <stdint.h>
#include "dma1.hpp"

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

ap_uint<32> generateHeader(unsigned int pktType, unsigned int ID){
#pragma HLS inline
    ap_uint<32> header=0;
    header(4,0)=ID;
    header(11,5)=0;
    header(14,12)=pktType;
    header[15]=0;
    header(20,16)=-1;//source row
    header(27,21)=-1;//source column
    header(30,28)=0;
    header[31]=header(30,0).xor_reduce()?(ap_uint<1>)0:(ap_uint<1>)1;
    return header;
}

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
    ap_uint<32> ID=0;
    ID(4,0)=header(4,0);
    return ID;
}

template<int NC>
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][PACK_IN*LEFT_SIZE],
           axis_stream& txA0, axis_stream& txA1, 
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        data_t da;
        ap_uint<32> header;
        
        ap_uint<32> data_temp[2][NUM_PER_TRA];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0

        for (int z = 0; z < Z; z++) {
            for (int xy = 0; xy < X*Y; xy++) {//y first then x
                for (int pack = 0; pack < PACK_IN; pack++){ 
                    header=generateHeader(0,pack);
                    int position=pack*LEFT_SIZE;
                    data=a_buf[xy][position];

                    data_temp[0][0]=data(31,0);
                    data_temp[0][1]=data(63,32);
                    data_temp[0][2]=data(95,64);
                    data_temp[0][3]=data(127,96);
                    
                    da(31,0)   = header;
                    da(63,32)  = data_temp[0][0];
                    da(95,64)  = data_temp[0][1];
                    da(127,96) = data_temp[0][2];

                    tmp.data   = da;
                    tmp.keep   = -1;
                    tmp.last   = 0;
                    
                    txA0.write(tmp);
                    txA1.write(tmp);
                    
                    for (int i = 1; i < LEFT_SIZE; i++){ 
                    #pragma HLS PIPELINE II = 1
                        int pos=i+position;
                        data = a_buf[xy][pos];

                        data_temp[i%2][0]=data(31,0);
                        data_temp[i%2][1]=data(63,32);
                        data_temp[i%2][2]=data(95,64);
                        data_temp[i%2][3]=data(127,96);
                        da(31,0)   = data_temp[(i+1)%2][3];
                        da(63,32)  = data_temp[i%2][0];
                        da(95,64)  = data_temp[i%2][1];
                        da(127,96) = data_temp[i%2][2];

                        tmp.data   = da;
                        tmp.keep   = -1;
                        tmp.last   = 0;
                        txA0.write(tmp);
                        txA1.write(tmp);
                        
                    }
                    
                    da(31,0)=data_temp[1][3];
                    da(63,32)  = 0;
                    da(95,64)  = 0;
                    da(127,96) = 0;
                    tmp.data  =  da; 
                    tmp.keep  = 0x000f;
                    tmp.last  = 1;

                    txA0.write(tmp);
                    txA1.write(tmp);
                    

                }
                
            }
        }
    }
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Z*Y][PACK_IN*RIGHT_SIZE],
           axis_stream& txB0, axis_stream& txB1, 
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        data_t da;
        ap_uint<32> header;

        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0

        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int pack = 0; pack < PACK_IN; pack++){  
                        header=generateHeader(0,pack);
                        int position=pack*RIGHT_SIZE;
                        int pos1=y+z*Y;
                        data=b_buf[pos1][position];

                        data_temp[0][0]=data(31,0);
                        data_temp[0][1]=data(63,32);
                        data_temp[0][2]=data(95,64);
                        data_temp[0][3]=data(127,96);
                        
                        da(31,0)   = header;
                        da(63,32)  = data_temp[0][0];
                        da(95,64)  = data_temp[0][1];
                        da(127,96) = data_temp[0][2];

                        tmp.data   = da;
                        tmp.keep   = -1;
                        tmp.last   = 0;

                        txB0.write(tmp);
                        txB1.write(tmp);
                        
                        for (int i = 1; i < RIGHT_SIZE; i++){ 
                        #pragma HLS PIPELINE II = 1
                            int pos0=i+position;
                            data = b_buf[pos1][pos0];

                            data_temp[i%2][0]=data(31,0);
                            data_temp[i%2][1]=data(63,32);
                            data_temp[i%2][2]=data(95,64);
                            data_temp[i%2][3]=data(127,96);
                            da(31,0)   = data_temp[(i+1)%2][3];
                            da(63,32)  = data_temp[i%2][0];
                            da(95,64)  = data_temp[i%2][1];
                            da(127,96) = data_temp[i%2][2];

                            tmp.data   = da;
                            tmp.keep   = -1;
                            tmp.last   = 0;
                            txB0.write(tmp);
                            txB1.write(tmp);
                              
                        }
                        
                        da(31,0)   = data_temp[1][3];
                        da(63,32)  = 0;
                        da(95,64)  = 0;
                        da(127,96) = 0;

                        tmp.data =  da;
                        tmp.keep  = 0x000f;
                        tmp.last  = 1;
                        txB0.write(tmp);
                        txB1.write(tmp);
                        

                    }
                }
            }
        }
    }
}

template<int NC>
void receiveC(ap_uint<PLIO_WIDTH> c_buf[Z*X][PACK_OUT][OUT_SIZE],axis_stream& rxC, bool enable){ 

#pragma HLS inline off
if (enable){
        
    axis_pkt tmp; 
    int cnt[2];
    #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
    comb_32 data_temp[2][4];
    #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
    comb_32 d0,d1,d2,d3;
    ap_uint<32> header;
    unsigned int ID;
    unsigned int tile_x;

    for(int i=0;i<PACK_OUT;i++){
    #pragma HLS unroll
        cnt[i]=0;
    }
    
    for(int z = 0; z < Z; z++){
        for(int x = 0; x < X; x++){
            for (int n = 0; n < Y; n++){
                for(int pack=0;pack<PACK_OUT;pack++){
                    tmp=rxC.read();
                    header=tmp.data(31,0);

                    data_temp[0][1].low=tmp.data(47,32);
                    data_temp[0][1].high=tmp.data(63,48);
                    data_temp[0][2].low=tmp.data(79,64);
                    data_temp[0][2].high=tmp.data(95,80);
                    data_temp[0][3].low=tmp.data(111,96);
                    data_temp[0][3].high=tmp.data(127,112);
                    
                    ID=getPacketId(header);
                    tile_x=cnt[ID]/Y;
                    cnt[ID]=cnt[ID]+1;

                    for(int i=0;i<OUT_SIZE;i++){
                    #pragma HLS PIPELINE II = 1
                    #pragma HLS dependence variable=c_buf type=inter false
                        tmp=rxC.read();
                        
                        data_temp[(i+1)%2][0].low=tmp.data(15,0);
                        data_temp[(i+1)%2][0].high=tmp.data(31,16);
                        data_temp[(i+1)%2][1].low=tmp.data(47,32);
                        data_temp[(i+1)%2][1].high=tmp.data(63,48);
                        data_temp[(i+1)%2][2].low=tmp.data(79,64);
                        data_temp[(i+1)%2][2].high=tmp.data(95,80);
                        data_temp[(i+1)%2][3].low=tmp.data(111,96);
                        data_temp[(i+1)%2][3].high=tmp.data(127,112);
                        
                        d0.low =c_buf[tile_x][ID][i](15,0)  ;
                        d0.high=c_buf[tile_x][ID][i](31,16) ;
                        d1.low =c_buf[tile_x][ID][i](47,32) ;
                        d1.high=c_buf[tile_x][ID][i](63,48) ;
                        d2.low =c_buf[tile_x][ID][i](79,64) ;
                        d2.high=c_buf[tile_x][ID][i](95,80) ;
                        d3.low =c_buf[tile_x][ID][i](111,96);
                        d3.high=c_buf[tile_x][ID][i](127,112);


                        d0.low = data_temp[i%2][1].low       + d0.low;
                        d0.high= data_temp[i%2][1].high      + d0.high;
                        d1.low = data_temp[i%2][2].low       + d1.low;
                        d1.high= data_temp[i%2][2].high      + d1.high;
                        d2.low = data_temp[i%2][3].low       + d2.low;
                        d2.high= data_temp[i%2][3].high      + d2.high;
                        d3.low = data_temp[(i+1)%2][0].low   + d3.low;
                        d3.high= data_temp[(i+1)%2][0].high  + d3.high;
                        
                        c_buf[tile_x][ID][i](15,0)   = d0.low ;
                        c_buf[tile_x][ID][i](31,16)  = d0.high;
                        c_buf[tile_x][ID][i](47,32)  = d1.low ;
                        c_buf[tile_x][ID][i](63,48)  = d1.high;
                        c_buf[tile_x][ID][i](79,64)  = d2.low ;
                        c_buf[tile_x][ID][i](95,80)  = d2.high;
                        c_buf[tile_x][ID][i](111,96) = d3.low ;
                        c_buf[tile_x][ID][i](127,112)= d3.high;
                    }
                }
            }
        } 
    }
}  
}

void compute(axis_stream_A& dataA_out, axis_stream_B& dataB_out, axis_stream_C& dataC_in,
             axis_stream& txA0, axis_stream& txA1, axis_stream& txA2, axis_stream& txA3,
             axis_stream& txA4, axis_stream& txA5, axis_stream& txA6, axis_stream& txA7,
             axis_stream& txB0, axis_stream& txB1, axis_stream& txB2, axis_stream& txB3,
             axis_stream& txB4, axis_stream& txB5, axis_stream& txB6, axis_stream& txB7,
             axis_stream& txB8, axis_stream& txB9, axis_stream& txB10, axis_stream& txB11,
             axis_stream& txB12, axis_stream& txB13, axis_stream& txB14, axis_stream& txB15,
             axis_stream& rxC0, axis_stream& rxC1, axis_stream& rxC2, axis_stream& rxC3,
             axis_stream& rxC4, axis_stream& rxC5, axis_stream& rxC6, axis_stream& rxC7,
             axis_stream& rxC8, axis_stream& rxC9, axis_stream& rxC10, axis_stream& rxC11,
             axis_stream& rxC12, axis_stream& rxC13, axis_stream& rxC14, axis_stream& rxC15,
             const int TX, const int TY, const int TZ){

    ap_uint<PLIO_WIDTH> buff0_A[A*(B/PACK_IN)][X*Y][PACK_IN*LEFT_SIZE];
    #pragma HLS bind_storage variable=buff0_A type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=BUFFA_FACTOR dim=3
    
    ap_uint<PLIO_WIDTH> buff1_A[A*(B/PACK_IN)][X*Y][PACK_IN*LEFT_SIZE];
    #pragma HLS bind_storage variable=buff1_A type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=BUFFA_FACTOR dim=3

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACK_IN)*C][Z*Y][PACK_IN*RIGHT_SIZE];
    #pragma HLS bind_storage variable=buff0_B type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=BUFFB_FACTOR dim=3

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACK_IN)*C][Z*Y][PACK_IN*RIGHT_SIZE];
    #pragma HLS bind_storage variable=buff1_B type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=BUFFB_FACTOR dim=3

    ap_uint<PLIO_WIDTH> buff0_C[(C/PACK_OUT)*A][Z*X][PACK_OUT][OUT_SIZE];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_C cyclic factor=BUFFC_FACTOR dim=4

    ap_uint<PLIO_WIDTH> buff1_C[(C/PACK_OUT)*A][Z*X][PACK_OUT][OUT_SIZE];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_C cyclic factor=BUFFC_FACTOR dim=4

    const int Total_rd=TX*TY*TZ;

    for (int rd=0; rd<Total_rd+2;rd++){
        int c_flg=0,s_flg=0;
        if(rd>0){
            c_flg=((rd-1)/TY)%2;
        }
        if(rd>1){
            s_flg=(rd-2)%TY;
        }
        if(rd%2==0&&c_flg==0){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);   

            sendA<0>(buff1_A[0],txA0,txA1,rd>0&&rd<Total_rd+1);
            sendA<1>(buff1_A[1],txA2,txA3,rd>0&&rd<Total_rd+1);
            sendA<2>(buff1_A[2],txA4,txA5,rd>0&&rd<Total_rd+1);
            sendA<3>(buff1_A[3],txA6,txA7,rd>0&&rd<Total_rd+1);
            

            sendB<0>(buff1_B[0],txB0,txB1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff1_B[1],txB2,txB3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff1_B[2],txB4,txB5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff1_B[3],txB6,txB7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff1_B[4],txB8,txB9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff1_B[5],txB10,txB11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff1_B[6],txB12,txB13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff1_B[7],txB14,txB15,rd>0&&rd<Total_rd+1);
            

            receiveC<0>(buff0_C[0],rxC0, rd>0&&rd<Total_rd+1);
            receiveC<1>(buff0_C[1],rxC1, rd>0&&rd<Total_rd+1);
            receiveC<2>(buff0_C[2],rxC2, rd>0&&rd<Total_rd+1);
            receiveC<3>(buff0_C[3],rxC3, rd>0&&rd<Total_rd+1);
            receiveC<4>(buff0_C[4],rxC4, rd>0&&rd<Total_rd+1);
            receiveC<5>(buff0_C[5],rxC5, rd>0&&rd<Total_rd+1);
            receiveC<6>(buff0_C[6],rxC6, rd>0&&rd<Total_rd+1);
            receiveC<7>(buff0_C[7],rxC7, rd>0&&rd<Total_rd+1);
            receiveC<8>(buff0_C[8],rxC8, rd>0&&rd<Total_rd+1);
            receiveC<9>(buff0_C[9],rxC9, rd>0&&rd<Total_rd+1);
            receiveC<10>(buff0_C[10],rxC10, rd>0&&rd<Total_rd+1);
            receiveC<11>(buff0_C[11],rxC11, rd>0&&rd<Total_rd+1);
            receiveC<12>(buff0_C[12],rxC12, rd>0&&rd<Total_rd+1);
            receiveC<13>(buff0_C[13],rxC13, rd>0&&rd<Total_rd+1);
            receiveC<14>(buff0_C[14],rxC14, rd>0&&rd<Total_rd+1);
            receiveC<15>(buff0_C[15],rxC15, rd>0&&rd<Total_rd+1);
            

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==1&&c_flg==0){
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            sendA<0>(buff0_A[0],txA0,txA1,rd>0&&rd<Total_rd+1);
            sendA<1>(buff0_A[1],txA2,txA3,rd>0&&rd<Total_rd+1);
            sendA<2>(buff0_A[2],txA4,txA5,rd>0&&rd<Total_rd+1);
            sendA<3>(buff0_A[3],txA6,txA7,rd>0&&rd<Total_rd+1);
            

            sendB<0>(buff0_B[0],txB0,txB1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff0_B[1],txB2,txB3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff0_B[2],txB4,txB5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff0_B[3],txB6,txB7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff0_B[4],txB8,txB9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff0_B[5],txB10,txB11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff0_B[6],txB12,txB13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff0_B[7],txB14,txB15,rd>0&&rd<Total_rd+1);
            

            receiveC<0>(buff0_C[0],rxC0, rd>0&&rd<Total_rd+1);
            receiveC<1>(buff0_C[1],rxC1, rd>0&&rd<Total_rd+1);
            receiveC<2>(buff0_C[2],rxC2, rd>0&&rd<Total_rd+1);
            receiveC<3>(buff0_C[3],rxC3, rd>0&&rd<Total_rd+1);
            receiveC<4>(buff0_C[4],rxC4, rd>0&&rd<Total_rd+1);
            receiveC<5>(buff0_C[5],rxC5, rd>0&&rd<Total_rd+1);
            receiveC<6>(buff0_C[6],rxC6, rd>0&&rd<Total_rd+1);
            receiveC<7>(buff0_C[7],rxC7, rd>0&&rd<Total_rd+1);
            receiveC<8>(buff0_C[8],rxC8, rd>0&&rd<Total_rd+1);
            receiveC<9>(buff0_C[9],rxC9, rd>0&&rd<Total_rd+1);
            receiveC<10>(buff0_C[10],rxC10, rd>0&&rd<Total_rd+1);
            receiveC<11>(buff0_C[11],rxC11, rd>0&&rd<Total_rd+1);
            receiveC<12>(buff0_C[12],rxC12, rd>0&&rd<Total_rd+1);
            receiveC<13>(buff0_C[13],rxC13, rd>0&&rd<Total_rd+1);
            receiveC<14>(buff0_C[14],rxC14, rd>0&&rd<Total_rd+1);
            receiveC<15>(buff0_C[15],rxC15, rd>0&&rd<Total_rd+1);
            

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);   

            sendA<0>(buff1_A[0],txA0,txA1,rd>0&&rd<Total_rd+1);
            sendA<1>(buff1_A[1],txA2,txA3,rd>0&&rd<Total_rd+1);
            sendA<2>(buff1_A[2],txA4,txA5,rd>0&&rd<Total_rd+1);
            sendA<3>(buff1_A[3],txA6,txA7,rd>0&&rd<Total_rd+1);
            

            sendB<0>(buff1_B[0],txB0,txB1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff1_B[1],txB2,txB3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff1_B[2],txB4,txB5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff1_B[3],txB6,txB7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff1_B[4],txB8,txB9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff1_B[5],txB10,txB11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff1_B[6],txB12,txB13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff1_B[7],txB14,txB15,rd>0&&rd<Total_rd+1);
            

            receiveC<0>(buff1_C[0],rxC0, rd>0&&rd<Total_rd+1);
            receiveC<1>(buff1_C[1],rxC1, rd>0&&rd<Total_rd+1);
            receiveC<2>(buff1_C[2],rxC2, rd>0&&rd<Total_rd+1);
            receiveC<3>(buff1_C[3],rxC3, rd>0&&rd<Total_rd+1);
            receiveC<4>(buff1_C[4],rxC4, rd>0&&rd<Total_rd+1);
            receiveC<5>(buff1_C[5],rxC5, rd>0&&rd<Total_rd+1);
            receiveC<6>(buff1_C[6],rxC6, rd>0&&rd<Total_rd+1);
            receiveC<7>(buff1_C[7],rxC7, rd>0&&rd<Total_rd+1);
            receiveC<8>(buff1_C[8],rxC8, rd>0&&rd<Total_rd+1);
            receiveC<9>(buff1_C[9],rxC9, rd>0&&rd<Total_rd+1);
            receiveC<10>(buff1_C[10],rxC10, rd>0&&rd<Total_rd+1);
            receiveC<11>(buff1_C[11],rxC11, rd>0&&rd<Total_rd+1);
            receiveC<12>(buff1_C[12],rxC12, rd>0&&rd<Total_rd+1);
            receiveC<13>(buff1_C[13],rxC13, rd>0&&rd<Total_rd+1);
            receiveC<14>(buff1_C[14],rxC14, rd>0&&rd<Total_rd+1);
            receiveC<15>(buff1_C[15],rxC15, rd>0&&rd<Total_rd+1);
            

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
        else{ //if(rd%2==1&&c_flg==1)
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            sendA<0>(buff0_A[0],txA0,txA1,rd>0&&rd<Total_rd+1);
            sendA<1>(buff0_A[1],txA2,txA3,rd>0&&rd<Total_rd+1);
            sendA<2>(buff0_A[2],txA4,txA5,rd>0&&rd<Total_rd+1);
            sendA<3>(buff0_A[3],txA6,txA7,rd>0&&rd<Total_rd+1);
            

            sendB<0>(buff0_B[0],txB0,txB1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff0_B[1],txB2,txB3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff0_B[2],txB4,txB5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff0_B[3],txB6,txB7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff0_B[4],txB8,txB9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff0_B[5],txB10,txB11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff0_B[6],txB12,txB13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff0_B[7],txB14,txB15,rd>0&&rd<Total_rd+1);
            

            receiveC<0>(buff1_C[0],rxC0, rd>0&&rd<Total_rd+1);
            receiveC<1>(buff1_C[1],rxC1, rd>0&&rd<Total_rd+1);
            receiveC<2>(buff1_C[2],rxC2, rd>0&&rd<Total_rd+1);
            receiveC<3>(buff1_C[3],rxC3, rd>0&&rd<Total_rd+1);
            receiveC<4>(buff1_C[4],rxC4, rd>0&&rd<Total_rd+1);
            receiveC<5>(buff1_C[5],rxC5, rd>0&&rd<Total_rd+1);
            receiveC<6>(buff1_C[6],rxC6, rd>0&&rd<Total_rd+1);
            receiveC<7>(buff1_C[7],rxC7, rd>0&&rd<Total_rd+1);
            receiveC<8>(buff1_C[8],rxC8, rd>0&&rd<Total_rd+1);
            receiveC<9>(buff1_C[9],rxC9, rd>0&&rd<Total_rd+1);
            receiveC<10>(buff1_C[10],rxC10, rd>0&&rd<Total_rd+1);
            receiveC<11>(buff1_C[11],rxC11, rd>0&&rd<Total_rd+1);
            receiveC<12>(buff1_C[12],rxC12, rd>0&&rd<Total_rd+1);
            receiveC<13>(buff1_C[13],rxC13, rd>0&&rd<Total_rd+1);
            receiveC<14>(buff1_C[14],rxC14, rd>0&&rd<Total_rd+1);
            receiveC<15>(buff1_C[15],rxC15, rd>0&&rd<Total_rd+1);
            

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
    }
}

void dma1(ap_uint<AXI_WIDTH_A>* ina, ap_uint<AXI_WIDTH_B>* inb, ap_uint<AXI_WIDTH_C>* out0,
         axis_stream& txA0, axis_stream& txA1, axis_stream& txA2, axis_stream& txA3,
         axis_stream& txA4, axis_stream& txA5, axis_stream& txA6, axis_stream& txA7,
         axis_stream& txB0, axis_stream& txB1, axis_stream& txB2, axis_stream& txB3,
         axis_stream& txB4, axis_stream& txB5, axis_stream& txB6, axis_stream& txB7,
         axis_stream& txB8, axis_stream& txB9, axis_stream& txB10, axis_stream& txB11,
         axis_stream& txB12, axis_stream& txB13, axis_stream& txB14, axis_stream& txB15,
         axis_stream& rxC0, axis_stream& rxC1, axis_stream& rxC2, axis_stream& rxC3,
         axis_stream& rxC4, axis_stream& rxC5, axis_stream& rxC6, axis_stream& rxC7,
         axis_stream& rxC8, axis_stream& rxC9, axis_stream& rxC10, axis_stream& rxC11,
         axis_stream& rxC12, axis_stream& rxC13, axis_stream& rxC14, axis_stream& rxC15,
         const int TX, const int TY, const int TZ){
    
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=ina
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=inb max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=inb
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=out0 max_write_burst_length=16 num_write_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=TX
    #pragma HLS interface s_axilite bundle=control port=TY
    #pragma HLS interface s_axilite bundle=control port=TZ
    #pragma HLS interface axis port=txA0
    #pragma HLS interface axis port=txA1
    #pragma HLS interface axis port=txA2
    #pragma HLS interface axis port=txA3
    #pragma HLS interface axis port=txA4
    #pragma HLS interface axis port=txA5
    #pragma HLS interface axis port=txA6
    #pragma HLS interface axis port=txA7
    #pragma HLS interface axis port=txB0
    #pragma HLS interface axis port=txB1
    #pragma HLS interface axis port=txB2
    #pragma HLS interface axis port=txB3
    #pragma HLS interface axis port=txB4
    #pragma HLS interface axis port=txB5
    #pragma HLS interface axis port=txB6
    #pragma HLS interface axis port=txB7
    #pragma HLS interface axis port=txB8
    #pragma HLS interface axis port=txB9
    #pragma HLS interface axis port=txB10
    #pragma HLS interface axis port=txB11
    #pragma HLS interface axis port=txB12
    #pragma HLS interface axis port=txB13
    #pragma HLS interface axis port=txB14
    #pragma HLS interface axis port=txB15
    #pragma HLS interface axis port=rxC0
    #pragma HLS interface axis port=rxC1
    #pragma HLS interface axis port=rxC2
    #pragma HLS interface axis port=rxC3
    #pragma HLS interface axis port=rxC4
    #pragma HLS interface axis port=rxC5
    #pragma HLS interface axis port=rxC6
    #pragma HLS interface axis port=rxC7
    #pragma HLS interface axis port=rxC8
    #pragma HLS interface axis port=rxC9
    #pragma HLS interface axis port=rxC10
    #pragma HLS interface axis port=rxC11
    #pragma HLS interface axis port=rxC12
    #pragma HLS interface axis port=rxC13
    #pragma HLS interface axis port=rxC14
    #pragma HLS interface axis port=rxC15
    #pragma HLS interface s_axilite bundle=control port=return

    #pragma HLS dataflow
    axis_stream_A dataA_out;
    axis_stream_B dataB_out;
    axis_stream_C dataC_in;
    axis_stream_32 addrA_out;
    axis_stream_32 addrB_out;
    axis_stream_32 addrC_out;

    address_A_ddr(addrA_out,TX,TY,TZ);
    loadA_ddr(ina, addrA_out,dataA_out,TX,TY,TZ);

    address_B_ddr(addrB_out,TX,TY,TZ);
    loadB_ddr(inb,addrB_out,dataB_out,TX,TY,TZ);

    address_C_ddr(addrC_out,TX,TZ);
    storeC_ddr(out0,addrC_out,dataC_in,TX,TZ);

    compute(dataA_out, dataB_out, dataC_in,
            txA0, txA1, txA2, txA3,
            txA4, txA5, txA6, txA7,
            txB0, txB1, txB2, txB3,
            txB4, txB5, txB6, txB7,
            txB8, txB9, txB10, txB11,
            txB12, txB13, txB14, txB15,
            rxC0, rxC1, rxC2, rxC3,
            rxC4, rxC5, rxC6, rxC7,
            rxC8, rxC9, rxC10, rxC11,
            rxC12, rxC13, rxC14, rxC15,
            TX, TY, TZ);

}