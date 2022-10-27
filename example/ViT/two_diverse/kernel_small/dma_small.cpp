#include <stdint.h>
#include "packet_sender_small.hpp"

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


void address_A_ddr(axis_stream_64& addrA_out,const int TX,const int TY,const int TZ, const int D0,const int D1) {
#pragma HLS inline off
    for(ap_uint<64> dot_x=0;dot_x<D0;dot_x++){
        for(ap_uint<64> dot_y=0;dot_y<D1;dot_y++){
            for(ap_uint<64> tx=0;tx<TX;tx++){
                for(ap_uint<64> tz=0;tz<TZ;tz++){
                    for(ap_uint<64> ty=0;ty<TY;ty++){
                        ap_uint<64> pos;
                        for(ap_uint<64> j=0;j<K;j++){
                            for(ap_uint<64> i=0;i<M/A_PER_TRA;i++){
                            #pragma HLS PIPELINE II = 1
                                pos=i+j*(M/A_PER_TRA)*TX*D0+ty*K*(M/A_PER_TRA)*TX*D0+tx*(M/A_PER_TRA)+dot_y*TY*K*(M/A_PER_TRA)*TX*D0+dot_x*TX*(M/A_PER_TRA);
                                addrA_out.write(pos);
                            }
                        }
                    }
                }
            }
        }
    }
}


void loadA_ddr(ap_uint<AXI_WIDTH_512>* ina, axis_stream_64& addrA_in,axis_stream_512& dataA_out,const int TX,const int TY,const int TZ, const int D0,const int D1) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    int64_t bound=boundA*TX*TY*TZ*D0*D1;
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
void address_B_ddr(axis_stream_64& addrB_out,const int TX,const int TY,const int TZ, const int D0,const int D1) {
#pragma HLS inline off    
    for(ap_uint<64> dot_x=0;dot_x<D0;dot_x++){
        for(ap_uint<64> dot_y=0;dot_y<D1;dot_y++){
            for(ap_uint<64> tx=0;tx<TX;tx++){
                for(ap_uint<64> tz=0;tz<TZ;tz++){
                    for(ap_uint<64> ty=0;ty<TY;ty++){
                        ap_uint<64> pos;
                        for(ap_uint<64> j=0;j<N;j++){
                            for(ap_uint<64> i=0;i<K/A_PER_TRA;i++){
                            #pragma HLS PIPELINE II = 1
                                pos=i+j*(K/A_PER_TRA)*TY*D0+ty*(K/A_PER_TRA)+tz*N*(K/A_PER_TRA)*TY*D0+dot_y*TZ*N*(K/A_PER_TRA)*TY*D0+dot_x*TY*(K/A_PER_TRA);
                                addrB_out.write(pos);
                            }
                        }
                    }
                }
            }
        }
    }
}


void loadB_ddr(ap_uint<AXI_WIDTH_512>* inb, axis_stream_64& addrB_in,axis_stream_512& dataB_out,const int TX,const int TY,const int TZ, const int D0,const int D1) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    int64_t bound=boundB*TX*TY*TZ*D0*D1;
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

template<int NC>
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][LEFT_SIZE*PACKET_NUM],axis_stream& txA0, bool enable){
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
            ID=k;
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
            }
    
            da(31,0)=data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            tmp.data  =  da; 
            tmp.keep  = 0x000f;
            tmp.last  = 1;
             
            txA0.write(tmp);
        }
    }
    
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE*PACKET_NUM], axis_stream& txB0, bool enable){
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

            ID=k;
            tile=0;
    
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
            }
    
            da(31,0)   = data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            
    
            tmp.data =  da;
            tmp.keep  = 0x000f;
            tmp.last  = 1;
        
            txB0.write(tmp);
            
        }
    }
}

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
    ap_uint<32> ID=0;
    ID(4,0)=header(4,0);
    return ID;
}



template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM_O][OUT_SIZE],axis_stream& rxC, bool enable){   
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
    for(int i=0;i<PACKET_NUM_O;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM_O;j++){
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

void address_C_ddr(axis_stream_64& addrC_out,const int TX,const int TZ, const int D0,const int D1) {
#pragma HLS inline off
    for(ap_uint<64> dot_x=0;dot_x<D0;dot_x++){
        for(ap_uint<64> dot_y=0;dot_y<D1;dot_y++){
            for(ap_uint<64> tx=0;tx<TX;tx++){
                for(ap_uint<64> tz=0;tz<TZ;tz++){
                    ap_uint<64> pos;
                    for(ap_uint<64> j=0;j<N;j++){
                        for(ap_uint<64> i=0;i<M/C_PER_TRA;i++){
                        #pragma HLS PIPELINE II = 1
                            pos=i+j*(M/C_PER_TRA)*TX*D0+tx*(M/C_PER_TRA)+tz*N*(M/C_PER_TRA)*TX*D0+dot_y*TZ*N*(M/C_PER_TRA)*TX*D0+dot_x*TX*(M/C_PER_TRA);
                            addrC_out.write(pos);
                        }
                    }    
                }
            }
        }
    }
}

void storeC_ddr(ap_uint<AXI_WIDTH_256>* outc, axis_stream_64& addrC_in,axis_stream_256& dataC_in,const int TX,const int TZ, const int D0,const int D1) { 
#pragma HLS inline off
    int64_t bound=boundC*TX*TZ*D0*D1;
    for(int64_t i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<256> temp_data=dataC_in.read();
        ap_uint<64> addr = addrC_in.read();
        outc[addr]=temp_data;
    }
}

void storeC(axis_stream_256& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM_O][X*Z][PACKET_NUM_O][OUT_SIZE], bool enable){
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
                                int pos1=c+(a%2)*2;
                                int pos2=x+z*X;
                                int pos3=a/2;
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
            for(int j=0;j<PACKET_NUM_O;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_O); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}

void compute(axis_stream_512& dataA_out, axis_stream_512& dataB_out, axis_stream_256& dataC_in,
             const int TX, const int TY, const int TZ, const int D0,const int D1,
             axis_stream& txA_0,  axis_stream& txA_1,
             axis_stream& txB_0,  axis_stream& txB_1,
             axis_stream& rxC_0){
    ap_uint<PLIO_WIDTH> buff0_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_A type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1

    ap_uint<PLIO_WIDTH> buff1_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_A type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_B type=RAM_1P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_B type=RAM_1P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1

    ap_uint<PLIO_WIDTH> buff0_C[A*C/PACKET_NUM_O][X*Z][PACKET_NUM_O][OUT_SIZE];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1

    ap_uint<PLIO_WIDTH> buff1_C[A*C/PACKET_NUM_O][X*Z][PACKET_NUM_O][OUT_SIZE];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1

    const int Total_rd=TX*TY*TZ;
    
    for (int batch_dot=0; batch_dot<D0*D1;batch_dot++){
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_O;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_O); a++){
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

                sendA<0>(buff1_A[0],txA_0,rd>0&&rd<Total_rd+1);
                sendA<1>(buff1_A[1],txA_1,rd>0&&rd<Total_rd+1);

                sendB<0>(buff1_B[0],txB_0,rd>0&&rd<Total_rd+1);
                sendB<1>(buff1_B[1],txB_1,rd>0&&rd<Total_rd+1);

                reshapeC<0>(buff0_C[0],rxC_0,rd>0&&rd<Total_rd+1);
                storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
            }
            else if(rd%2==1&&c_flg==0){
                loadA(dataA_out,buff1_A,rd<Total_rd);
                loadB(dataB_out,buff1_B,rd<Total_rd);

                sendA<0>(buff0_A[0],txA_0,rd>0&&rd<Total_rd+1);
                sendA<1>(buff0_A[1],txA_1,rd>0&&rd<Total_rd+1);

                sendB<0>(buff0_B[0],txB_0,rd>0&&rd<Total_rd+1);
                sendB<1>(buff0_B[1],txB_1,rd>0&&rd<Total_rd+1);

                reshapeC<0>(buff0_C[0],rxC_0,rd>0&&rd<Total_rd+1);
                storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
            }
            else if(rd%2==0&&c_flg==1){
                loadA(dataA_out,buff0_A,rd<Total_rd);
                loadB(dataB_out,buff0_B,rd<Total_rd);

                sendA<0>(buff1_A[0],txA_0,rd>0&&rd<Total_rd+1);
                sendA<1>(buff1_A[1],txA_1,rd>0&&rd<Total_rd+1);

                sendB<0>(buff1_B[0],txB_0,rd>0&&rd<Total_rd+1);
                sendB<1>(buff1_B[1],txB_1,rd>0&&rd<Total_rd+1);

                reshapeC<0>(buff1_C[0],rxC_0,rd>0&&rd<Total_rd+1);
                storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
            }
            else { //if(rd%2==1&&c_flg==1)
                loadA(dataA_out,buff1_A,rd<Total_rd);
                loadB(dataB_out,buff1_B,rd<Total_rd);

                sendA<0>(buff0_A[0],txA_0,rd>0&&rd<Total_rd+1);
                sendA<1>(buff0_A[1],txA_1,rd>0&&rd<Total_rd+1);

                sendB<0>(buff0_B[0],txB_0,rd>0&&rd<Total_rd+1);
                sendB<1>(buff0_B[1],txB_1,rd>0&&rd<Total_rd+1);

                reshapeC<0>(buff1_C[0],rxC_0,rd>0&&rd<Total_rd+1); 
                storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
            }
        }
    }

}

void dma_small(ap_uint<AXI_WIDTH_512>* ina, ap_uint<AXI_WIDTH_512>* inb, ap_uint<AXI_WIDTH_256>* out0,
            const int TX, const int TY, const int TZ, const int D0, const int D1,
            axis_stream& txA_0,  axis_stream& txA_1,
            axis_stream& txB_0,  axis_stream& txB_1,
            axis_stream& rxC_0)
{
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina max_read_burst_length=16 num_read_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=ina
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=inb max_read_burst_length=16 num_read_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=inb
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=out0 max_write_burst_length=16 num_write_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=TX
    #pragma HLS interface s_axilite bundle=control port=TY
    #pragma HLS interface s_axilite bundle=control port=TZ
    #pragma HLS interface s_axilite bundle=control port=D0
    #pragma HLS interface s_axilite bundle=control port=D1
    #pragma HLS interface axis port=txA_0
    #pragma HLS interface axis port=txA_1
    #pragma HLS interface axis port=txB_0
    #pragma HLS interface axis port=txB_1
    #pragma HLS interface axis port=rxC_0
    #pragma HLS interface s_axilite bundle=control port=return
    
    #pragma HLS dataflow

    axis_stream_512 dataA_out;
    axis_stream_512 dataB_out;
    axis_stream_256 dataC_in;
    axis_stream_64 addrA_out;
    axis_stream_64 addrB_out;
    axis_stream_64 addrC_out;

    address_A_ddr(addrA_out,TX,TY,TZ,D0,D1);
    loadA_ddr(ina, addrA_out,dataA_out,TX,TY,TZ,D0,D1);

    address_B_ddr(addrB_out,TX,TY,TZ,D0,D1);
    loadB_ddr(inb,addrB_out,dataB_out,TX,TY,TZ,D0,D1);

    address_C_ddr(addrC_out,TX,TZ,D0,D1);
    storeC_ddr(out0,addrC_out,dataC_in,TX,TZ,D0,D1);

    compute(dataA_out, dataB_out, dataC_in, TX, TY, TZ,D0,D1,
            txA_0, txA_1,
            txB_0, txB_1,
            rxC_0);

}