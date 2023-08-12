
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
void sendA(ap_uint<BUFF_WIDTH> a_buf[X*Y][PACK_IN][LEFT_SIZE_BUFF],
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
                    data(63,0)=a_buf[xy][pack][0];
                    data(127,64)=a_buf[xy][pack][1];

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
                    for (int i = 1; i < LEFT_SIZE_BUFF/2; i++){ 
                    #pragma HLS PIPELINE II = 1
                        data(63,0)=a_buf[xy][pack][i*2];
                        data(127,64)=a_buf[xy][pack][i*2+1];

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
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][PACK_IN][RIGHT_SIZE],
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
                        int pos1=y+z*Y;
                        data=b_buf[pos1][pack][0];


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
                            data=b_buf[pos1][pack][i];

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
void receiveC(ap_uint<BUFF_WIDTH> c_buf[2][X*Z][PACK_OUT][OUT_SIZE_BUFF/2],axis_stream& rxC, bool enable){ 

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

                    data_temp[0][1].low0=tmp.data(39,32);
                    data_temp[0][1].low1=tmp.data(47,40);
                    data_temp[0][1].high0=tmp.data(55,48);
                    data_temp[0][1].high1=tmp.data(63,56);
                    data_temp[0][2].low0=tmp.data(71,64);
                    data_temp[0][2].low1=tmp.data(79,72);
                    data_temp[0][2].high0=tmp.data(87,80);
                    data_temp[0][2].high1=tmp.data(95,88);
                    data_temp[0][3].low0=tmp.data(103,96);
                    data_temp[0][3].low1=tmp.data(111,104);
                    data_temp[0][3].high0=tmp.data(119,112);
                    data_temp[0][3].high1=tmp.data(127,120);
                    
                    ID=getPacketId(header);
                    tile_x=cnt[ID]/Y;
                    cnt[ID]=cnt[ID]+1;

                    for(int i=0;i<OUT_SIZE_BUFF/2;i++){
                    #pragma HLS PIPELINE II = 1
                    #pragma HLS dependence variable=c_buf type=inter false
                    #pragma HLS dependence variable=c_buf type=intra false
                        tmp=rxC.read();
                        
                        data_temp[(i+1)%2][0].low0=tmp.data(7,0);
                        data_temp[(i+1)%2][0].low1=tmp.data(15,8);
                        data_temp[(i+1)%2][0].high0=tmp.data(23,16);
                        data_temp[(i+1)%2][0].high1=tmp.data(31,24);
                        data_temp[(i+1)%2][1].low0=tmp.data(39,32);
                        data_temp[(i+1)%2][1].low1=tmp.data(47,40);
                        data_temp[(i+1)%2][1].high0=tmp.data(55,48);
                        data_temp[(i+1)%2][1].high1=tmp.data(63,56);
                        data_temp[(i+1)%2][2].low0=tmp.data(71,64);
                        data_temp[(i+1)%2][2].low1=tmp.data(79,72);
                        data_temp[(i+1)%2][2].high0=tmp.data(87,80);
                        data_temp[(i+1)%2][2].high1=tmp.data(95,88);
                        data_temp[(i+1)%2][3].low0=tmp.data(103,96);
                        data_temp[(i+1)%2][3].low1=tmp.data(111,104);
                        data_temp[(i+1)%2][3].high0=tmp.data(119,112);
                        data_temp[(i+1)%2][3].high1=tmp.data(127,120);
                        
                        d0.low0  =c_buf[0][tile_x][ID][i](7,0)  ;
                        d0.low1  =c_buf[0][tile_x][ID][i](15,8) ;
                        d0.high0 =c_buf[0][tile_x][ID][i](23,16);
                        d0.high1 =c_buf[0][tile_x][ID][i](31,24);
                        d1.low0  =c_buf[0][tile_x][ID][i](39,32);
                        d1.low1  =c_buf[0][tile_x][ID][i](47,40);
                        d1.high0 =c_buf[0][tile_x][ID][i](55,48);
                        d1.high1 =c_buf[0][tile_x][ID][i](63,56);
                        d2.low0  =c_buf[1][tile_x][ID][i](7,0)  ;
                        d2.low1  =c_buf[1][tile_x][ID][i](15,8) ;
                        d2.high0 =c_buf[1][tile_x][ID][i](23,16);
                        d2.high1 =c_buf[1][tile_x][ID][i](31,24);
                        d3.low0  =c_buf[1][tile_x][ID][i](39,32);
                        d3.low1  =c_buf[1][tile_x][ID][i](47,40);
                        d3.high0 =c_buf[1][tile_x][ID][i](55,48);
                        d3.high1 =c_buf[1][tile_x][ID][i](63,56);


                        d0.low0  = data_temp[i%2][1].low0       + d0.low0 ;
                        d0.low1  = data_temp[i%2][1].low1       + d0.low1 ;
                        d0.high0 = data_temp[i%2][1].high0      + d0.high0;
                        d0.high1 = data_temp[i%2][1].high1      + d0.high1;
                        d1.low0  = data_temp[i%2][2].low0       + d1.low0 ;
                        d1.low1  = data_temp[i%2][2].low1       + d1.low1 ;
                        d1.high0 = data_temp[i%2][2].high0      + d1.high0;
                        d1.high1 = data_temp[i%2][2].high1      + d1.high1;
                        d2.low0  = data_temp[i%2][3].low0       + d2.low0 ;
                        d2.low1  = data_temp[i%2][3].low1       + d2.low1 ;
                        d2.high0 = data_temp[i%2][3].high0      + d2.high0;
                        d2.high1 = data_temp[i%2][3].high1      + d2.high1;
                        d3.low0  = data_temp[(i+1)%2][0].low0   + d3.low0 ;
                        d3.low1  = data_temp[(i+1)%2][0].low1   + d3.low1 ;
                        d3.high0 = data_temp[(i+1)%2][0].high0  + d3.high0;
                        d3.high1 = data_temp[(i+1)%2][0].high1  + d3.high1;
                        
                        c_buf[0][tile_x][ID][i](7,0)    = d0.low0  ;
                        c_buf[0][tile_x][ID][i](15,8)   = d0.low1  ;
                        c_buf[0][tile_x][ID][i](23,16)  = d0.high0 ;
                        c_buf[0][tile_x][ID][i](31,24)  = d0.high1 ;
                        c_buf[0][tile_x][ID][i](39,32)  = d1.low0  ;
                        c_buf[0][tile_x][ID][i](47,40)  = d1.low1  ;
                        c_buf[0][tile_x][ID][i](55,48)  = d1.high0 ;
                        c_buf[0][tile_x][ID][i](63,56)  = d1.high1 ;
                        c_buf[1][tile_x][ID][i](7,0)    = d2.low0  ;
                        c_buf[1][tile_x][ID][i](15,8)   = d2.low1  ;
                        c_buf[1][tile_x][ID][i](23,16)  = d2.high0 ;
                        c_buf[1][tile_x][ID][i](31,24)  = d2.high1 ;
                        c_buf[1][tile_x][ID][i](39,32)  = d3.low0  ;
                        c_buf[1][tile_x][ID][i](47,40)  = d3.low1  ;
                        c_buf[1][tile_x][ID][i](55,48)  = d3.high0 ;
                        c_buf[1][tile_x][ID][i](63,56)  = d3.high1 ;
                    }
                }
            }
        } 
    }
}  
}