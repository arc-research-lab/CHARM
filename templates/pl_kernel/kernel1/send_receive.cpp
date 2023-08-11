
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
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][PACK_IN*W1*(H1/NUM_PER_TRA)],
           {% for i in range(NUM_TXL) -%} axis_stream& txA{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        ap_uint<32> header;
        for (int z = 0; z < Z; z++) {
            for (int xy = 0; xy < X*Y; xy++) {//y first then x
                for (int pack = 0; pack < PACK_IN; pack++){ 
                    header=generateHeader(0,pack);
                    data(127,0) = (ap_uint<96>(0),header);
                    tmp.data   = data;
                    tmp.keep   = 0x000f;
                    tmp.last   = 0;
                    {% for i in range(NUM_TXL) -%} 
                    txA{{i}}.write(tmp);
                    {% endfor %}
                    for (int i = 0; i < W1*(H1/NUM_PER_TRA); i++){ 
                    #pragma HLS PIPELINE II = 1
                        int pos=i+pack*W1*(H1/NUM_PER_TRA);
                        data = a_buf[xy][pos];
                        tmp.data   = data;
                        tmp.keep   = -1;
                        tmp.last   = (i==(W1*(H1/NUM_PER_TRA))-1);
                        {% for i in range(NUM_TXL) -%} 
                        txA{{i}}.write(tmp);
                        {% endfor %}
                    }
                }
                
            }
        }
    }
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Z*Y][PACK_IN*W2*(W1/NUM_PER_TRA)],
           {% for i in range(NUM_TXR) -%} axis_stream& txB{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        ap_uint<32> header;
        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int pack = 0; pack < PACK_IN; pack++){  
                        header=generateHeader(0,pack);
                        data(127,0) = (ap_uint<96>(0),header);
                        tmp.data   = data;
                        tmp.keep   = 0x000f;
                        tmp.last   = 0
                        {% for i in range(NUM_TXR) -%} 
                        txB{{i}}.write(tmp);
                        {% endfor%}
                        for (int i = 0; i < W2*(W1/NUM_PER_TRA); i++){ 
                        #pragma HLS PIPELINE II = 1
                            int pos0=i+pack*W2*(W1/NUM_PER_TRA);
                            int pos1=y+z*Y;
                            data = b_buf[pos1][pos0];
                            tmp.data   = data;
                            tmp.keep   = -1;
                            tmp.last   = (i==(W2*(W1/NUM_PER_TRA))-1);
                            {% for i in range(NUM_TXR) -%} 
                            txB{{i}}.write(tmp);
                            {% endfor%}  
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void receiveC(ap_uint<PLIO_WIDTH> c_buf[Z*X][PACK_OUT][W2*(H1/NUM_PER_TRA)],axis_stream& rxC, bool enable){ 

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        fp_int data_temp0[(PLIO_WIDTH/DATA_TYPE)];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=1

        fp_int data_temp1[(PLIO_WIDTH/DATA_TYPE)];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=1

        fp_int data_temp2[(PLIO_WIDTH/DATA_TYPE)];
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
                    for (int pack = 0; pack < PACK_IN; pack++){
                        tmp=rxC.read();
                        header=tmp.data(31,0);
                        ID=getPacketId(header);
                        tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;

                        for (int i = 0; i < (H1/NUM_PER_TRA/2); i++){ 
                            for (int j = 0; j < W2; j++){ 
                                for (int h2=0; h2<2; h2++){ 
                                #pragma HLS PIPELINE II = 1
                                #pragma HLS dependence variable=c_buf type=inter false
                                    int pos0=h2+i*2+j*(H1/NUM_PER_TRA);
                                    int pos1=x+z*X;
                                    tmp=rxC.read();
                                    for(int un=0; un<NUM_PER_TRA; un++){
                                    #pragma HLS UNROLL factor=NUM_PER_TRA
                                        data_temp0[un].data_uint=tmp.data(32*un+31,32*un);
                                        data_temp1[un].data_uint=c_buf[pos1][tile_x][pos0](32*un+31,32*un);
                                        data_temp2[un].data_float=data_temp0[un].data_float + data_temp1[un].data_float; 
                                        c_buf[pos1][tile_x][pos0](32*un+31,32*un)   =  data_temp2[un].data_uint;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}