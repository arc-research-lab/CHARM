
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
void sendA(ap_uint<PLIO_WIDTH> a_buf[X][Y][PACK_IN][W1*(H1/NUM_PER_TRA)],
           {% for i in range(NUM_TXL) -%} axis_stream& txA{{i}}, {% endfor%}
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
                        for (int i = 0; i < W1*(H1/NUM_PER_TRA)+1; i++){ 
                        #pragma HLS PIPELINE II = 1
                            if(i==0){
                                header=generateHeader(0,pack);
                                data(127,0) = (ap_uint<96>(0),header);
                                tmp.data   = data;
                                tmp.keep   = 0x000f;
                                {% for i in range(NUM_TXL) -%} 
                                txA{{i}}.write(tmp);
                                {% endfor %}
                            }
                            else{
                                data = a_buf[x][y][pack][i-1];
                                tmp.data   = data;
                                tmp.keep   = -1;
                                {% for i in range(NUM_TXL) -%} 
                                txA{{i}}.write(tmp);
                                {% endfor %}
                            }
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Z][Y][PACK_IN][W2*(W1/NUM_PER_TRA)],
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
                        for (int i = 0; i < W2*(W1/NUM_PER_TRA)+1; i++){ 
                        #pragma HLS PIPELINE II = 1
                            if(i==0){
                                header=generateHeader(0,pack);
                                data(127,0) = (ap_uint<96>(0),header);
                                tmp.data   = data;
                                tmp.keep   = 0x000f;
                                {% for i in range(NUM_TXR) -%} 
                                txB{{i}}.write(tmp);
                                {% endfor%}
                            }
                            else{
                                data = b_buf[z][y][pack][i-1];
                                tmp.data   = data;
                                tmp.keep   = -1;
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
}

template<int NC>
void receiveC(ap_uint<PLIO_WIDTH> c_buf[Z][X][PACK_OUT][W2*(H1/NUM_PER_TRA)],axis_stream& rxC, bool enable){ 

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
                        for (int i = 0; i < W2*(H1/NUM_PER_TRA)+1; i++){ 
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=c_buf type=inter false
                            
                            if(i==0){
                                tmp=rxC.read();
                                header=tmp.data(31,0);
                                ID=getPacketId(header);
                                tile_x=cnt[ID]/Y;
                                cnt[ID]=cnt[ID]+1;
                            }
                            else{
                                int real_pos=i-1;
                                int pos0=real_pos%2;
                                int pos1=(real_pos/2)%W2; //2 is (H1_P/NUM_PER_TRA)
                                int pos2=real_pos/(2*W2);
                                int pos3=pos0+pos2*2;
                                tmp=rxC.read();
                                for(int un=0; un<NUM_PER_TRA; un++){
                                #pragma HLS UNROLL factor=NUM_PER_TRA
                                    data_temp0[un].data_uint=tmp.data(32*un+31,32*un);
                                    data_temp1[un].data_uint=c_buf[z][x][tile_x][pos3](32*un+31,32*un);
                                    data_temp2[un].data_float=data_temp0[un].data_float + data_temp1[un].data_float; 
                                    c_buf[z][x][tile_x][pos3](32*un+31,32*un)   =  data_temp2[un].data_uint;
                                }
                            } 

                        }
                    }
                }
            }
        }
    }
}