#include <stdint.h>
#include "packet_sender_large.hpp"

static const unsigned int tile0[10]={0,1,0,2,1,0,3,2,1,0};
static const unsigned int tile2[6]={3, 2, 1, 3, 2, 3};
static const unsigned int tileB[16]={1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1};
static const unsigned int packet_id0[10]={0, 0, 1, 0, 1, 2, 0, 1, 2, 3};
static const unsigned int packet_id2[6]={1, 2, 3, 2, 3, 3};

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
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][LEFT_SIZE*PACKET_NUM],axis_stream& txA0, axis_stream& txA1,axis_stream& txA2,axis_stream& txA3, bool enable){
#pragma HLS inline off
    if(enable){
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        axis_pkt tmp;
        data_t data;
        data_t da;
        for (int k = 0; k < X*PACKET_NUM*Y*Z; k++) {    
            unsigned int ID=0;
            int tile=0;
            if(k<10){
                ID=packet_id0[k];
                tile=tile0[k];
            }
            else if(k<(X*PACKET_NUM*Y*Z-6)){
                int cnt=k-10;
                ID=cnt%PACKET_NUM;
                tile=(4-(cnt%PACKET_NUM)+cnt/PACKET_NUM)%(X*Y);
            }
            else{
                int pos2=k-X*PACKET_NUM*Y*Z+6;
                ID=packet_id2[pos2];
                tile=tile2[pos2]+(X*Y-PACKET_NUM);
            }
            ap_uint<32> header=generateHeader(PKTTYPE,ID);
            int position=ID*LEFT_SIZE;
    
            data=a_buf[tile][position];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            tmp.data  =  da;
            tmp.keep  = -1;
            tmp.last  = 0;
            txA0.write(tmp);
            txA1.write(tmp);
            txA2.write(tmp);
            txA3.write(tmp);
    
            for (int i = 1; i < LEFT_SIZE; i++){ 
            #pragma HLS PIPELINE II = 1
                int posa=ID*LEFT_SIZE+i;
    
                data=a_buf[tile][posa];
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
                txA2.write(tmp);
                txA3.write(tmp);
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
            txA2.write(tmp);
            txA3.write(tmp);
        }
    }
    
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE*PACKET_NUM], axis_stream& txB0, axis_stream& txB1, bool enable){
#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        data_t data;
        data_t da;
    
    
        for (int k = 0; k < PACKET_NUM*Y*X*Z; k++) {
            unsigned int ID=0;
            int tile=0;
    
            if(k<10){
                ID=packet_id0[k];
                tile=0;
            }
            else if(k<PACKET_NUM*Y*X*Z-6){
                int cnt=k-10;
                ID=cnt%PACKET_NUM;

                if(cnt<16){
                    tile=0;
                }
                else if(cnt<32){
                    int pos=cnt-16;
                    tile=tileB[pos];
                }
                else{
                    tile=1;
                }
            }
            else{
                int pos2=k-PACKET_NUM*Y*X*Z+6;
                ID=packet_id2[pos2];
                tile=1;
            }
    
            ap_uint<32> header=generateHeader(PKTTYPE,ID);
            int position=ID*RIGHT_SIZE;
    
            data=b_buf[tile][position];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            
            tmp.data  =  da;
            tmp.keep  =  -1;
            tmp.last  =  0;
        
            txB0.write(tmp);
            txB1.write(tmp);

            for (int i = 1; i < RIGHT_SIZE; i++){
            #pragma HLS PIPELINE II = 1
                int posb=ID*RIGHT_SIZE+i;   
    
                data=b_buf[tile][posb];
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

template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM][OUT_SIZE],axis_stream& rxC, bool enable){   
#pragma HLS inline off
    if (enable){
        
        axis_pkt tmp; 
        int cnt[4];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0

        fp_int intfp0;
        fp_int intfp1;
        fp_int intfp2;
        fp_int intfp3;
        fp_int ifp_temp0;
        fp_int ifp_temp1;
        fp_int ifp_temp2;
        fp_int ifp_temp3;   
    for(int i=0;i<PACKET_NUM;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM;j++){
                        ap_uint<32> header;
                        tmp=rxC.read();
                        
                        header=tmp.data(31,0);
                        
                        ifp_temp1.uintval=tmp.data(63,32);
                        ifp_temp2.uintval=tmp.data(95,64);
                        ifp_temp3.uintval=tmp.data(127,96);
            
            
                        data_temp[0][1]=ifp_temp1.data_cbuff;
                        data_temp[0][2]=ifp_temp2.data_cbuff;
                        data_temp[0][3]=ifp_temp3.data_cbuff;
                        
                        unsigned int ID=getPacketId(header);
    
                        unsigned int tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;
    
    
    
                        for(int i=0;i<OUT_SIZE;i++){
                        #pragma HLS PIPELINE II = 1
                            tmp=rxC.read();
                            

                            ifp_temp0.uintval=tmp.data(31,0);
                            ifp_temp1.uintval=tmp.data(63,32);
                            ifp_temp2.uintval=tmp.data(95,64);
                            ifp_temp3.uintval=tmp.data(127,96);

                            data_temp[(i+1)%2][0]=ifp_temp0.data_cbuff;
                            data_temp[(i+1)%2][1]=ifp_temp1.data_cbuff;
                            data_temp[(i+1)%2][2]=ifp_temp2.data_cbuff;
                            data_temp[(i+1)%2][3]=ifp_temp3.data_cbuff;
                            
                            intfp0.uintval=c_buf[tile_x][ID][i](31,0);
                            intfp1.uintval=c_buf[tile_x][ID][i](63,32);
                            intfp2.uintval=c_buf[tile_x][ID][i](95,64);
                            intfp3.uintval=c_buf[tile_x][ID][i](127,96);

                            intfp0.data_cbuff = data_temp[i%2][1] + intfp0.data_cbuff ;
                            intfp1.data_cbuff = data_temp[i%2][2] + intfp1.data_cbuff ;
                            intfp2.data_cbuff = data_temp[i%2][3] + intfp2.data_cbuff ;
                            intfp3.data_cbuff = data_temp[(i+1)%2][0] + intfp3.data_cbuff;

                            c_buf[tile_x][ID][i](31,0)   =  intfp0.uintval;  
                            c_buf[tile_x][ID][i](63,32)  =  intfp1.uintval;
                            c_buf[tile_x][ID][i](95,64)  =  intfp2.uintval;
                            c_buf[tile_x][ID][i](127,96) =  intfp3.uintval;
                            
                        }
                    }
                }
            } 
        }
    }
    
}

////////////////////////////////////

void address_A_ddr(axis_stream_64& addrA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    for(ap_uint<64> tx=0;tx<TX;tx++){
        for(ap_uint<64> tz=0;tz<TZ;tz++){
            for(ap_uint<64> ty=0;ty<TY;ty++){
                ap_uint<64> pos;
                for(ap_uint<64> j=0;j<K;j++){
                    for(ap_uint<64> i=0;i<M/A_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(M/A_PER_TRA)*TX+ty*K*(M/A_PER_TRA)*TX+tx*(M/A_PER_TRA);
                        addrA_out.write(pos);
                    }
                }
            }
        }
    }
}


void loadA_ddr(ap_uint<AXI_WIDTH_512>* ina, axis_stream_64& addrA_in,axis_stream_512& dataA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    int64_t bound=boundA*TX*TY*TZ;
    for(int64_t i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<64> addr = addrA_in.read();
        temp_data=ina[addr];
        dataA_out.write(temp_data);        
    }
}

void loadA(axis_stream_512& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(int64_t y=0;y<Y;y++){
            for(int64_t k=0;k<W1*B;k++){
                for(int64_t x=0;x<X;x++){
                    for(int64_t a=0;a<A;a++){
                        for(int64_t i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*4+(k%(W1*PACKET_NUM))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM));
                            ap_uint<512> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(127,0);
                            a_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                            a_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                            a_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                        }
                    }
                }
            }
        }
    }
}



//////////////////////////////////load B//////////////////////////////////////
void address_B_ddr(axis_stream_64& addrB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off    
    for(ap_uint<64> tx=0;tx<TX;tx++){
        for(ap_uint<64> tz=0;tz<TZ;tz++){
            for(ap_uint<64> ty=0;ty<TY;ty++){
                ap_uint<64> pos;
                for(ap_uint<64> j=0;j<N;j++){
                    for(ap_uint<64> i=0;i<K/A_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(K/A_PER_TRA)*TY+ty*(K/A_PER_TRA)+tz*N*(K/A_PER_TRA)*TY;
                        addrB_out.write(pos);
                    }
                }
            }
        }
    }
}


void loadB_ddr(ap_uint<AXI_WIDTH_512>* inb, axis_stream_64& addrB_in,axis_stream_512& dataB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    int64_t bound=boundB*TX*TY*TZ;
    for(int64_t i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<64> addr = addrB_in.read();
        temp_data=inb[addr];
        dataB_out.write(temp_data);
    }  
}

void loadB(axis_stream_512& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/A_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM)*C+c;
                                ap_uint<512> temp_data=dataB_in.read();
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


void address_C_ddr(axis_stream_64& addrC_out,const int TX,const int TZ) {
#pragma HLS inline off
    for(ap_uint<64> tx=0;tx<TX;tx++){
        for(ap_uint<64> tz=0;tz<TZ;tz++){
            ap_uint<64> pos;
            for(ap_uint<64> j=0;j<N;j++){
                for(ap_uint<64> i=0;i<M/C_PER_TRA;i++){
                #pragma HLS PIPELINE II = 1
                    pos=i+j*(M/C_PER_TRA)*TX+tx*(M/C_PER_TRA)+tz*N*(M/C_PER_TRA)*TX;
                    addrC_out.write(pos);
                }
            }
        }
    }
}

void storeC_ddr(ap_uint<AXI_WIDTH_256>* outc, axis_stream_64& addrC_in,axis_stream_256& dataC_in,const int TX,const int TZ) { 
#pragma HLS inline off
    int64_t bound=boundC*TX*TZ;
    for(int64_t i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<256> temp_data=dataC_in.read();
        ap_uint<64> addr = addrC_in.read();
        outc[addr]=temp_data;
            
    }
    
}

void storeC(axis_stream_256& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=n*2+w2*(H1/NUM_PER_TRA);
                                int pos1=c%PACKET_NUM;
                                int pos2=x+z*X;
                                int pos3=a*(C/PACKET_NUM)+(c/PACKET_NUM);
                                ap_uint<AXI_WIDTH_256> temp_data;
                                temp_data(127,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+1];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}
void compute(axis_stream_512& dataA_out, axis_stream_512& dataB_out, axis_stream_256& dataC_in,const int TX, const int TY, const int TZ,
             axis_stream& txA_0,  axis_stream& txA_1,  axis_stream& txA_2,  axis_stream& txA_3,
             axis_stream& txA_4,  axis_stream& txA_5,  axis_stream& txA_6,  axis_stream& txA_7,
             axis_stream& txA_8,  axis_stream& txA_9,  axis_stream& txA_10, axis_stream& txA_11,
             axis_stream& txA_12, axis_stream& txA_13, axis_stream& txA_14, axis_stream& txA_15,
             axis_stream& txB_0,  axis_stream& txB_1,  axis_stream& txB_2,  axis_stream& txB_3,
             axis_stream& txB_4,  axis_stream& txB_5,  axis_stream& txB_6,  axis_stream& txB_7,
             axis_stream& txB_8,  axis_stream& txB_9,  axis_stream& txB_10, axis_stream& txB_11,
             axis_stream& txB_12, axis_stream& txB_13, axis_stream& txB_14, axis_stream& txB_15,
             axis_stream& txB_16, axis_stream& txB_17, axis_stream& txB_18, axis_stream& txB_19,
             axis_stream& txB_20, axis_stream& txB_21, axis_stream& txB_22, axis_stream& txB_23,
             axis_stream& txB_24, axis_stream& txB_25, axis_stream& txB_26, axis_stream& txB_27,
             axis_stream& txB_28, axis_stream& txB_29, axis_stream& txB_30, axis_stream& txB_31,
             axis_stream& rxC_0, axis_stream& rxC_1, axis_stream& rxC_2, axis_stream& rxC_3,
             axis_stream& rxC_4, axis_stream& rxC_5, axis_stream& rxC_6, axis_stream& rxC_7,
             axis_stream& rxC_8, axis_stream& rxC_9, axis_stream& rxC_10, axis_stream& rxC_11,
             axis_stream& rxC_12, axis_stream& rxC_13, axis_stream& rxC_14, axis_stream& rxC_15)
{
    ap_uint<PLIO_WIDTH> buff0_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_A type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1

    ap_uint<PLIO_WIDTH> buff1_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_A type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_B type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_B type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1

    ap_uint<PLIO_WIDTH> buff0_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1

    ap_uint<PLIO_WIDTH> buff1_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1

    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM;j++){
            for (int i = 0; i < OUT_SIZE/2; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM); a++){
                    buff0_C[a][x][j][i*2+0]=0; 
                    buff0_C[a][x][j][i*2+1]=0;
                    buff1_C[a][x][j][i*2+0]=0; 
                    buff1_C[a][x][j][i*2+1]=0; 
                }
            }
        }
    }

    for (int rd=0; rd<Total_rd+2;rd++){
        int c_flg=0,s_flg=0;
        int tx_A=0,ty_A=0;
        int ty_B=0,tz_B=0;
        int tx_C=0,tz_C=0;
        tx_A=rd/(TY*TZ);
        ty_A=rd%TY;
        ty_B=rd%TY;
        tz_B=rd/TY;
        if(rd>0){
            c_flg=((rd-1)/TY)%2;
        }
        if(rd>1){
            tx_C=(rd-2)/(TY*TZ);
            tz_C=(rd-2)/TY;
            s_flg=(rd-2)%TY;
        }
        if(rd%2==0&&c_flg==0){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);

            sendA<0>(buff1_A[0],txA_0,txA_1,txA_2,txA_3,rd>0&&rd<Total_rd+1);
            sendA<1>(buff1_A[1],txA_4,txA_5,txA_6,txA_7,rd>0&&rd<Total_rd+1);
            sendA<2>(buff1_A[2],txA_8,txA_9,txA_10,txA_11,rd>0&&rd<Total_rd+1);
            sendA<3>(buff1_A[3],txA_12,txA_13,txA_14,txA_15,rd>0&&rd<Total_rd+1);

            sendB<0>(buff1_B[0],txB_0,txB_1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff1_B[1],txB_2,txB_3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff1_B[2],txB_4,txB_5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff1_B[3],txB_6,txB_7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff1_B[4],txB_8,txB_9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff1_B[5],txB_10,txB_11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff1_B[6],txB_12,txB_13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff1_B[7],txB_14,txB_15,rd>0&&rd<Total_rd+1);
            sendB<8>(buff1_B[8],txB_16,txB_17,rd>0&&rd<Total_rd+1);
            sendB<9>(buff1_B[9],txB_18,txB_19,rd>0&&rd<Total_rd+1);
            sendB<10>(buff1_B[10],txB_20,txB_21,rd>0&&rd<Total_rd+1);
            sendB<11>(buff1_B[11],txB_22,txB_23,rd>0&&rd<Total_rd+1);
            sendB<12>(buff1_B[12],txB_24,txB_25,rd>0&&rd<Total_rd+1);
            sendB<13>(buff1_B[13],txB_26,txB_27,rd>0&&rd<Total_rd+1);
            sendB<14>(buff1_B[14],txB_28,txB_29,rd>0&&rd<Total_rd+1);
            sendB<15>(buff1_B[15],txB_30,txB_31,rd>0&&rd<Total_rd+1);
            
            reshapeC<0>(buff0_C[0],rxC_0,rd>0&&rd<Total_rd+1);
            reshapeC<1>(buff0_C[1],rxC_1,rd>0&&rd<Total_rd+1);
            reshapeC<2>(buff0_C[2],rxC_2,rd>0&&rd<Total_rd+1);
            reshapeC<3>(buff0_C[3],rxC_3,rd>0&&rd<Total_rd+1);
            reshapeC<4>(buff0_C[4],rxC_4,rd>0&&rd<Total_rd+1);
            reshapeC<5>(buff0_C[5],rxC_5,rd>0&&rd<Total_rd+1);
            reshapeC<6>(buff0_C[6],rxC_6,rd>0&&rd<Total_rd+1);
            reshapeC<7>(buff0_C[7],rxC_7,rd>0&&rd<Total_rd+1);
            reshapeC<8>(buff0_C[8],rxC_8,rd>0&&rd<Total_rd+1);
            reshapeC<9>(buff0_C[9],rxC_9,rd>0&&rd<Total_rd+1);
            reshapeC<10>(buff0_C[10],rxC_10,rd>0&&rd<Total_rd+1);
            reshapeC<11>(buff0_C[11],rxC_11,rd>0&&rd<Total_rd+1);
            reshapeC<12>(buff0_C[12],rxC_12,rd>0&&rd<Total_rd+1);
            reshapeC<13>(buff0_C[13],rxC_13,rd>0&&rd<Total_rd+1);
            reshapeC<14>(buff0_C[14],rxC_14,rd>0&&rd<Total_rd+1);
            reshapeC<15>(buff0_C[15],rxC_15,rd>0&&rd<Total_rd+1);
            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==1&&c_flg==0){
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);

            sendA<0>(buff0_A[0],txA_0,txA_1,txA_2,txA_3,rd>0&&rd<Total_rd+1);
            sendA<1>(buff0_A[1],txA_4,txA_5,txA_6,txA_7,rd>0&&rd<Total_rd+1);
            sendA<2>(buff0_A[2],txA_8,txA_9,txA_10,txA_11,rd>0&&rd<Total_rd+1);
            sendA<3>(buff0_A[3],txA_12,txA_13,txA_14,txA_15,rd>0&&rd<Total_rd+1);
            sendB<0>(buff0_B[0],txB_0,txB_1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff0_B[1],txB_2,txB_3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff0_B[2],txB_4,txB_5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff0_B[3],txB_6,txB_7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff0_B[4],txB_8,txB_9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff0_B[5],txB_10,txB_11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff0_B[6],txB_12,txB_13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff0_B[7],txB_14,txB_15,rd>0&&rd<Total_rd+1);
            sendB<8>(buff0_B[8],txB_16,txB_17,rd>0&&rd<Total_rd+1);
            sendB<9>(buff0_B[9],txB_18,txB_19,rd>0&&rd<Total_rd+1);
            sendB<10>(buff0_B[10],txB_20,txB_21,rd>0&&rd<Total_rd+1);
            sendB<11>(buff0_B[11],txB_22,txB_23,rd>0&&rd<Total_rd+1);
            sendB<12>(buff0_B[12],txB_24,txB_25,rd>0&&rd<Total_rd+1);
            sendB<13>(buff0_B[13],txB_26,txB_27,rd>0&&rd<Total_rd+1);
            sendB<14>(buff0_B[14],txB_28,txB_29,rd>0&&rd<Total_rd+1);
            sendB<15>(buff0_B[15],txB_30,txB_31,rd>0&&rd<Total_rd+1);
            reshapeC<0>(buff0_C[0],rxC_0,rd>0&&rd<Total_rd+1);
            reshapeC<1>(buff0_C[1],rxC_1,rd>0&&rd<Total_rd+1);
            reshapeC<2>(buff0_C[2],rxC_2,rd>0&&rd<Total_rd+1);
            reshapeC<3>(buff0_C[3],rxC_3,rd>0&&rd<Total_rd+1);
            reshapeC<4>(buff0_C[4],rxC_4,rd>0&&rd<Total_rd+1);
            reshapeC<5>(buff0_C[5],rxC_5,rd>0&&rd<Total_rd+1);
            reshapeC<6>(buff0_C[6],rxC_6,rd>0&&rd<Total_rd+1);
            reshapeC<7>(buff0_C[7],rxC_7,rd>0&&rd<Total_rd+1);
            reshapeC<8>(buff0_C[8],rxC_8,rd>0&&rd<Total_rd+1);
            reshapeC<9>(buff0_C[9],rxC_9,rd>0&&rd<Total_rd+1);
            reshapeC<10>(buff0_C[10],rxC_10,rd>0&&rd<Total_rd+1);
            reshapeC<11>(buff0_C[11],rxC_11,rd>0&&rd<Total_rd+1);
            reshapeC<12>(buff0_C[12],rxC_12,rd>0&&rd<Total_rd+1);
            reshapeC<13>(buff0_C[13],rxC_13,rd>0&&rd<Total_rd+1);
            reshapeC<14>(buff0_C[14],rxC_14,rd>0&&rd<Total_rd+1);
            reshapeC<15>(buff0_C[15],rxC_15,rd>0&&rd<Total_rd+1);
            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);

            sendA<0>(buff1_A[0],txA_0,txA_1,txA_2,txA_3,rd>0&&rd<Total_rd+1);
            sendA<1>(buff1_A[1],txA_4,txA_5,txA_6,txA_7,rd>0&&rd<Total_rd+1);
            sendA<2>(buff1_A[2],txA_8,txA_9,txA_10,txA_11,rd>0&&rd<Total_rd+1);
            sendA<3>(buff1_A[3],txA_12,txA_13,txA_14,txA_15,rd>0&&rd<Total_rd+1);
            sendB<0>(buff1_B[0],txB_0,txB_1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff1_B[1],txB_2,txB_3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff1_B[2],txB_4,txB_5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff1_B[3],txB_6,txB_7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff1_B[4],txB_8,txB_9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff1_B[5],txB_10,txB_11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff1_B[6],txB_12,txB_13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff1_B[7],txB_14,txB_15,rd>0&&rd<Total_rd+1);
            sendB<8>(buff1_B[8],txB_16,txB_17,rd>0&&rd<Total_rd+1);
            sendB<9>(buff1_B[9],txB_18,txB_19,rd>0&&rd<Total_rd+1);
            sendB<10>(buff1_B[10],txB_20,txB_21,rd>0&&rd<Total_rd+1);
            sendB<11>(buff1_B[11],txB_22,txB_23,rd>0&&rd<Total_rd+1);
            sendB<12>(buff1_B[12],txB_24,txB_25,rd>0&&rd<Total_rd+1);
            sendB<13>(buff1_B[13],txB_26,txB_27,rd>0&&rd<Total_rd+1);
            sendB<14>(buff1_B[14],txB_28,txB_29,rd>0&&rd<Total_rd+1);
            sendB<15>(buff1_B[15],txB_30,txB_31,rd>0&&rd<Total_rd+1);
            reshapeC<0>(buff1_C[0],rxC_0,rd>0&&rd<Total_rd+1);
            reshapeC<1>(buff1_C[1],rxC_1,rd>0&&rd<Total_rd+1);
            reshapeC<2>(buff1_C[2],rxC_2,rd>0&&rd<Total_rd+1);
            reshapeC<3>(buff1_C[3],rxC_3,rd>0&&rd<Total_rd+1);
            reshapeC<4>(buff1_C[4],rxC_4,rd>0&&rd<Total_rd+1);
            reshapeC<5>(buff1_C[5],rxC_5,rd>0&&rd<Total_rd+1);
            reshapeC<6>(buff1_C[6],rxC_6,rd>0&&rd<Total_rd+1);
            reshapeC<7>(buff1_C[7],rxC_7,rd>0&&rd<Total_rd+1);
            reshapeC<8>(buff1_C[8],rxC_8,rd>0&&rd<Total_rd+1);
            reshapeC<9>(buff1_C[9],rxC_9,rd>0&&rd<Total_rd+1);
            reshapeC<10>(buff1_C[10],rxC_10,rd>0&&rd<Total_rd+1);
            reshapeC<11>(buff1_C[11],rxC_11,rd>0&&rd<Total_rd+1);
            reshapeC<12>(buff1_C[12],rxC_12,rd>0&&rd<Total_rd+1);
            reshapeC<13>(buff1_C[13],rxC_13,rd>0&&rd<Total_rd+1);
            reshapeC<14>(buff1_C[14],rxC_14,rd>0&&rd<Total_rd+1);
            reshapeC<15>(buff1_C[15],rxC_15,rd>0&&rd<Total_rd+1);
            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
        else { //if(rd%2==1&&c_flg==1)
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);

            sendA<0>(buff0_A[0],txA_0,txA_1,txA_2,txA_3,rd>0&&rd<Total_rd+1);
            sendA<1>(buff0_A[1],txA_4,txA_5,txA_6,txA_7,rd>0&&rd<Total_rd+1);
            sendA<2>(buff0_A[2],txA_8,txA_9,txA_10,txA_11,rd>0&&rd<Total_rd+1);
            sendA<3>(buff0_A[3],txA_12,txA_13,txA_14,txA_15,rd>0&&rd<Total_rd+1);
            sendB<0>(buff0_B[0],txB_0,txB_1,rd>0&&rd<Total_rd+1);
            sendB<1>(buff0_B[1],txB_2,txB_3,rd>0&&rd<Total_rd+1);
            sendB<2>(buff0_B[2],txB_4,txB_5,rd>0&&rd<Total_rd+1);
            sendB<3>(buff0_B[3],txB_6,txB_7,rd>0&&rd<Total_rd+1);
            sendB<4>(buff0_B[4],txB_8,txB_9,rd>0&&rd<Total_rd+1);
            sendB<5>(buff0_B[5],txB_10,txB_11,rd>0&&rd<Total_rd+1);
            sendB<6>(buff0_B[6],txB_12,txB_13,rd>0&&rd<Total_rd+1);
            sendB<7>(buff0_B[7],txB_14,txB_15,rd>0&&rd<Total_rd+1);
            sendB<8>(buff0_B[8],txB_16,txB_17,rd>0&&rd<Total_rd+1);
            sendB<9>(buff0_B[9],txB_18,txB_19,rd>0&&rd<Total_rd+1);
            sendB<10>(buff0_B[10],txB_20,txB_21,rd>0&&rd<Total_rd+1);
            sendB<11>(buff0_B[11],txB_22,txB_23,rd>0&&rd<Total_rd+1);
            sendB<12>(buff0_B[12],txB_24,txB_25,rd>0&&rd<Total_rd+1);
            sendB<13>(buff0_B[13],txB_26,txB_27,rd>0&&rd<Total_rd+1);
            sendB<14>(buff0_B[14],txB_28,txB_29,rd>0&&rd<Total_rd+1);
            sendB<15>(buff0_B[15],txB_30,txB_31,rd>0&&rd<Total_rd+1);
            reshapeC<0>(buff1_C[0],rxC_0,rd>0&&rd<Total_rd+1);
            reshapeC<1>(buff1_C[1],rxC_1,rd>0&&rd<Total_rd+1);
            reshapeC<2>(buff1_C[2],rxC_2,rd>0&&rd<Total_rd+1);
            reshapeC<3>(buff1_C[3],rxC_3,rd>0&&rd<Total_rd+1);
            reshapeC<4>(buff1_C[4],rxC_4,rd>0&&rd<Total_rd+1);
            reshapeC<5>(buff1_C[5],rxC_5,rd>0&&rd<Total_rd+1);
            reshapeC<6>(buff1_C[6],rxC_6,rd>0&&rd<Total_rd+1);
            reshapeC<7>(buff1_C[7],rxC_7,rd>0&&rd<Total_rd+1);
            reshapeC<8>(buff1_C[8],rxC_8,rd>0&&rd<Total_rd+1);
            reshapeC<9>(buff1_C[9],rxC_9,rd>0&&rd<Total_rd+1);
            reshapeC<10>(buff1_C[10],rxC_10,rd>0&&rd<Total_rd+1);
            reshapeC<11>(buff1_C[11],rxC_11,rd>0&&rd<Total_rd+1);
            reshapeC<12>(buff1_C[12],rxC_12,rd>0&&rd<Total_rd+1);
            reshapeC<13>(buff1_C[13],rxC_13,rd>0&&rd<Total_rd+1);
            reshapeC<14>(buff1_C[14],rxC_14,rd>0&&rd<Total_rd+1);
            reshapeC<15>(buff1_C[15],rxC_15,rd>0&&rd<Total_rd+1);
            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
    }
        

}


void dma_large(ap_uint<AXI_WIDTH_512>* ina, ap_uint<AXI_WIDTH_512>* inb, ap_uint<AXI_WIDTH_256>* out0, const int TX, const int TY, const int TZ,
            axis_stream& txA_0,  axis_stream& txA_1,  axis_stream& txA_2,  axis_stream& txA_3,
            axis_stream& txA_4,  axis_stream& txA_5,  axis_stream& txA_6,  axis_stream& txA_7,
            axis_stream& txA_8,  axis_stream& txA_9,  axis_stream& txA_10, axis_stream& txA_11,
            axis_stream& txA_12, axis_stream& txA_13, axis_stream& txA_14, axis_stream& txA_15,
            axis_stream& txB_0,  axis_stream& txB_1,  axis_stream& txB_2,  axis_stream& txB_3,
            axis_stream& txB_4,  axis_stream& txB_5,  axis_stream& txB_6,  axis_stream& txB_7,
            axis_stream& txB_8,  axis_stream& txB_9,  axis_stream& txB_10, axis_stream& txB_11,
            axis_stream& txB_12, axis_stream& txB_13, axis_stream& txB_14, axis_stream& txB_15,
            axis_stream& txB_16, axis_stream& txB_17, axis_stream& txB_18, axis_stream& txB_19,
            axis_stream& txB_20, axis_stream& txB_21, axis_stream& txB_22, axis_stream& txB_23,
            axis_stream& txB_24, axis_stream& txB_25, axis_stream& txB_26, axis_stream& txB_27,
            axis_stream& txB_28, axis_stream& txB_29, axis_stream& txB_30, axis_stream& txB_31,
            axis_stream& rxC_0, axis_stream& rxC_1, axis_stream& rxC_2, axis_stream& rxC_3,
            axis_stream& rxC_4, axis_stream& rxC_5, axis_stream& rxC_6, axis_stream& rxC_7,
            axis_stream& rxC_8, axis_stream& rxC_9, axis_stream& rxC_10, axis_stream& rxC_11,
            axis_stream& rxC_12, axis_stream& rxC_13, axis_stream& rxC_14, axis_stream& rxC_15)
{
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina max_read_burst_length=64 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=ina
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=inb max_read_burst_length=64 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=inb
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=out0 max_write_burst_length=64 num_write_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=TX
    #pragma HLS interface s_axilite bundle=control port=TY
    #pragma HLS interface s_axilite bundle=control port=TZ
    #pragma HLS interface axis port=txA_0
    #pragma HLS interface axis port=txA_1
    #pragma HLS interface axis port=txA_2
    #pragma HLS interface axis port=txA_3
    #pragma HLS interface axis port=txA_4
    #pragma HLS interface axis port=txA_5
    #pragma HLS interface axis port=txA_6
    #pragma HLS interface axis port=txA_7
    #pragma HLS interface axis port=txA_8
    #pragma HLS interface axis port=txA_9
    #pragma HLS interface axis port=txA_10
    #pragma HLS interface axis port=txA_11
    #pragma HLS interface axis port=txA_12
    #pragma HLS interface axis port=txA_13
    #pragma HLS interface axis port=txA_14
    #pragma HLS interface axis port=txA_15
    #pragma HLS interface axis port=txB_0
    #pragma HLS interface axis port=txB_1
    #pragma HLS interface axis port=txB_2
    #pragma HLS interface axis port=txB_3
    #pragma HLS interface axis port=txB_4
    #pragma HLS interface axis port=txB_5
    #pragma HLS interface axis port=txB_6
    #pragma HLS interface axis port=txB_7
    #pragma HLS interface axis port=txB_8
    #pragma HLS interface axis port=txB_9
    #pragma HLS interface axis port=txB_10
    #pragma HLS interface axis port=txB_11
    #pragma HLS interface axis port=txB_12
    #pragma HLS interface axis port=txB_13
    #pragma HLS interface axis port=txB_14
    #pragma HLS interface axis port=txB_15
    #pragma HLS interface axis port=txB_16
    #pragma HLS interface axis port=txB_17
    #pragma HLS interface axis port=txB_18
    #pragma HLS interface axis port=txB_19
    #pragma HLS interface axis port=txB_20
    #pragma HLS interface axis port=txB_21
    #pragma HLS interface axis port=txB_22
    #pragma HLS interface axis port=txB_23
    #pragma HLS interface axis port=txB_24
    #pragma HLS interface axis port=txB_25
    #pragma HLS interface axis port=txB_26
    #pragma HLS interface axis port=txB_27
    #pragma HLS interface axis port=txB_28
    #pragma HLS interface axis port=txB_29
    #pragma HLS interface axis port=txB_30
    #pragma HLS interface axis port=txB_31
    #pragma HLS interface axis port=rxC_0
    #pragma HLS interface axis port=rxC_1
    #pragma HLS interface axis port=rxC_2
    #pragma HLS interface axis port=rxC_3
    #pragma HLS interface axis port=rxC_4
    #pragma HLS interface axis port=rxC_5
    #pragma HLS interface axis port=rxC_6
    #pragma HLS interface axis port=rxC_7
    #pragma HLS interface axis port=rxC_8
    #pragma HLS interface axis port=rxC_9
    #pragma HLS interface axis port=rxC_10
    #pragma HLS interface axis port=rxC_11
    #pragma HLS interface axis port=rxC_12
    #pragma HLS interface axis port=rxC_13
    #pragma HLS interface axis port=rxC_14
    #pragma HLS interface axis port=rxC_15
    #pragma HLS interface s_axilite bundle=control port=return
    
    

    #pragma HLS dataflow

    axis_stream_512 dataA_out;
    axis_stream_512 dataB_out;
    axis_stream_256 dataC_in;
    axis_stream_64 addrA_out;
    axis_stream_64 addrB_out;
    axis_stream_64 addrC_out;

    address_A_ddr(addrA_out,TX,TY,TZ);
    loadA_ddr(ina, addrA_out,dataA_out,TX,TY,TZ);

    address_B_ddr(addrB_out,TX,TY,TZ);
    loadB_ddr(inb,addrB_out,dataB_out,TX,TY,TZ);

    address_C_ddr(addrC_out,TX,TZ);
    storeC_ddr(out0,addrC_out,dataC_in,TX,TZ);
    
    compute(dataA_out, dataB_out, dataC_in, TX, TY, TZ,
            txA_0,  txA_1,  txA_2,  txA_3,
            txA_4,  txA_5,  txA_6,  txA_7,
            txA_8,  txA_9,  txA_10, txA_11,
            txA_12, txA_13, txA_14, txA_15,
            txB_0,  txB_1,  txB_2,  txB_3,
            txB_4,  txB_5,  txB_6,  txB_7,
            txB_8,  txB_9,  txB_10, txB_11,
            txB_12, txB_13, txB_14, txB_15,
            txB_16, txB_17, txB_18, txB_19,
            txB_20, txB_21, txB_22, txB_23,
            txB_24, txB_25, txB_26, txB_27,
            txB_28, txB_29, txB_30, txB_31,
            rxC_0,  rxC_1,  rxC_2,  rxC_3,
            rxC_4,  rxC_5,  rxC_6,  rxC_7,
            rxC_8,  rxC_9,  rxC_10, rxC_11,
            rxC_12, rxC_13, rxC_14, rxC_15);
}