
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
           {% for i in range(NUM_TXL) -%} axis_stream& txA{{i}}, {% endfor%}
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
                    
                    {% for i in range(NUM_TXL) -%} 
                    txA{{i}}.write(tmp);
                    {% endfor %}
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
                        {% for i in range(NUM_TXL) -%} 
                        txA{{i}}.write(tmp);
                        {% endfor %}
                    }
                    
                    da(31,0)=data_temp[1][3];
                    da(63,32)  = 0;
                    da(95,64)  = 0;
                    da(127,96) = 0;
                    tmp.data  =  da; 
                    tmp.keep  = 0x000f;
                    tmp.last  = 1;

                    {% for i in range(NUM_TXL) -%} 
                    txA{{i}}.write(tmp);
                    {% endfor %}

                }
                
            }
        }
    }
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Z*Y][PACK_IN*RIGHT_SIZE],
           {% for i in range(NUM_TXR) -%} axis_stream& txB{{i}}, {% endfor%}
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

                        {% for i in range(NUM_TXR) -%} 
                        txB{{i}}.write(tmp);
                        {% endfor%}
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
                            {% for i in range(NUM_TXR) -%} 
                            txB{{i}}.write(tmp);
                            {% endfor%}  
                        }
                        
                        da(31,0)   = data_temp[1][3];
                        da(63,32)  = 0;
                        da(95,64)  = 0;
                        da(127,96) = 0;

                        tmp.data =  da;
                        tmp.keep  = 0x000f;
                        tmp.last  = 1;
                        {% for i in range(NUM_TXR) -%} 
                        txB{{i}}.write(tmp);
                        {% endfor%}

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