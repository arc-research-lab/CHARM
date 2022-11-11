if [ "$#" -eq 2 ] 
then
    dir_name=$1;
    data_type=$2;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_reshapec Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi

if [ ${data_type} == "fp32" ]
then
echo \
"
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
                        #pragma HLS dependence variable=c_buf type=inter false
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
    
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${data_type} == "int32" ]
then
echo \
"
template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM][OUT_SIZE],axis_stream& rxC, bool enable){   
#pragma HLS inline off
    if (enable){
        
        axis_pkt tmp; 
        int cnt[4];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
        data_t1 data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        for(int i=0;i<PACKET_NUM;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for(int j=0;j<PACKET_NUM;j++){
                    for (int i = 0; i < OUT_SIZE; i++){
                    #pragma HLS PIPELINE II = 1
                        c_buf[x+z*X][j][i]=0; 
                    }
                }
            }
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM;j++){
                        ap_uint<32> header;
                        tmp=rxC.read();
                        
                        header=tmp.data(31,0);
                        data_temp[0][1]=tmp.data(63,32);
                        data_temp[0][2]=tmp.data(95,64);
                        data_temp[0][3]=tmp.data(127,96);
                        
                        unsigned int ID=getPacketId(header);
    
                        unsigned int tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;
    
    
    
                        for(int i=0;i<OUT_SIZE;i++){
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=c_buf type=inter false
                            tmp=rxC.read();
            
                            data_temp[(i+1)%2][0]=tmp.data(31,0);
                            data_temp[(i+1)%2][1]=tmp.data(63,32);
                            data_temp[(i+1)%2][2]=tmp.data(95,64);
                            data_temp[(i+1)%2][3]=tmp.data(127,96);
        
                            c_buf[tile_x][ID][i](31,0)  = data_temp[i%2][1] + c_buf[tile_x][ID][i](31,0)  ;
                            c_buf[tile_x][ID][i](63,32) = data_temp[i%2][2] + c_buf[tile_x][ID][i](63,32) ;
                            c_buf[tile_x][ID][i](95,64) = data_temp[i%2][3] + c_buf[tile_x][ID][i](95,64) ;
                            c_buf[tile_x][ID][i](127,96)= data_temp[(i+1)%2][0] + c_buf[tile_x][ID][i](127,96);
                            
                        }
                    }
                }
            } 
        }
    } 
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${data_type} == "int16" ]
then
echo \
"
template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM][OUT_SIZE],axis_stream& rxC, bool enable){   
#pragma HLS inline off
    if (enable){
        
        axis_pkt tmp; 
        int cnt[4];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
        comb_32 data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        comb_32 d0,d1,d2,d3;
        for(int i=0;i<PACKET_NUM;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for(int j=0;j<PACKET_NUM;j++){
                    for (int i = 0; i < OUT_SIZE; i++){
                    #pragma HLS PIPELINE II = 1
                        c_buf[x+z*X][j][i]=0;
                    }
                }
            }
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM;j++){
                        ap_uint<32> header;
                        tmp=rxC.read();
                        
                        header=tmp.data(31,0);

                        data_temp[0][1].low=tmp.data(47,32);
                        data_temp[0][1].high=tmp.data(63,48);
                        data_temp[0][2].low=tmp.data(79,64);
                        data_temp[0][2].high=tmp.data(95,80);
                        data_temp[0][3].low=tmp.data(111,96);
                        data_temp[0][3].high=tmp.data(127,112);
                        
                        unsigned int ID=getPacketId(header);
    
                        unsigned int tile_x=cnt[ID]/Y;
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
">> ./${dir_name}/kernel/dma.cpp;
fi