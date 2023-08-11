
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
                        int pos=i+pack*LEFT_SIZE;
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
                            int pos0=i+pack*RIGHT_SIZE;
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
    if(enable){
        axis_pkt tmp;
        data_t data;

        fp_int data_temp0[2][NUM_PER_TRA];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        fp_int data_temp1[NUM_PER_TRA];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=1

        fp_int data_temp2[NUM_PER_TRA];
        #pragma HLS ARRAY_PARTITION variable=data_temp2 complete dim=1

        int cnt[PACK_OUT];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0

        unsigned int ID;
        unsigned int tile_x;
        ap_uint<32> header;

        for(int i=0;i<PACK_OUT;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }

        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int pack = 0; pack < PACK_OUT; pack++){
                        tmp=rxC.read();
                        header=tmp.data(31,0);
                        
                        data_temp0[0][1].data_uint=tmp.data(63,32);
                        data_temp0[0][2].data_uint=tmp.data(95,64);
                        data_temp0[0][3].data_uint=tmp.data(127,96);

                        ID=getPacketId(header);
                        tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;

                        for (int i = 0; i < OUT_SIZE; i++){ 
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=c_buf type=inter false
                            tmp=rxC.read();

                            for(int un=0; un<NUM_PER_TRA; un++){
                            #pragma HLS UNROLL factor=NUM_PER_TRA
                                data_temp0[(i+1)%2][un].data_uint=tmp.data(un*32+31,un*32);
                                data_temp1[un].data_uint= c_buf[tile_x][ID][i](un*32+31,un*32);
                                
                            }
                            data_temp2[0].data_float= data_temp0[i%2][1].data_float+data_temp1[0].data_float;
                            data_temp2[1].data_float= data_temp0[i%2][2].data_float+data_temp1[1].data_float;
                            data_temp2[2].data_float= data_temp0[i%2][3].data_float+data_temp1[2].data_float;
                            data_temp2[3].data_float= data_temp0[(i+1)%2][0].data_float+data_temp1[3].data_float;

                            c_buf[tile_x][ID][i](31,0)   =  data_temp2[0].data_uint;  
                            c_buf[tile_x][ID][i](63,32)  =  data_temp2[1].data_uint;
                            c_buf[tile_x][ID][i](95,64)  =  data_temp2[2].data_uint;
                            c_buf[tile_x][ID][i](127,96) =  data_temp2[3].data_uint;
                                
                        }
                    }
                }
            }
        }
    }
}