
template<int NC>
void sendA(ap_uint<PLIO_WIDTH> a_buf[X][Y][W1][(H1/NUM_PER_TRA)],
           {% for i in range(NUM_TXL) -%} axis_stream& txA{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int j = 0; j < W1; j++){ 
                        for (int i = 0; i < (H1/NUM_PER_TRA); i++){ 
                        #pragma HLS PIPELINE II = 1
                            data = a_buf[x][y][j][i];
                            tmp.data   = data;
                            tmp.keep   = -1;
                            {% for i in range(NUM_TXL) -%} 
                            txA{{i}}.write(tmp);
                            {% endfor%}
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Z][Y][W2][(W1/NUM_PER_TRA)],
           {% for i in range(NUM_TXR) -%} axis_stream& txB{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int j = 0; j < W2; j++){ 
                        for (int i = 0; i < (W1/NUM_PER_TRA); i++){ 
                        #pragma HLS PIPELINE II = 1
                            data = b_buf[z][y][j][i];
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

template<int NC>
void receiveC(ap_uint<PLIO_WIDTH> c_buf[Z][X][W2][(H1/NUM_PER_TRA)],axis_stream& rxC, bool enable){ 

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        fp_int data_temp0[(NUM_PER_TRA/DATA_TYPE)];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=1

        fp_int data_temp1[(NUM_PER_TRA/DATA_TYPE)];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=1

        fp_int data_temp2[(NUM_PER_TRA/DATA_TYPE)];
        #pragma HLS ARRAY_PARTITION variable=data_temp2 complete dim=1
        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int i = 0; i < (H1/NUM_PER_TRA); i++){ 
                        for (int j = 0; j < W2; j++){     
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=c_buf type=inter false
                            tmp=rxC.read();
                            data_temp0[0].data_uint=tmp.data(31,0);
                            data_temp0[1].data_uint=tmp.data(63,32);
                            data_temp0[2].data_uint=tmp.data(95,64);
                            data_temp0[3].data_uint=tmp.data(127,96);

                            if (y==0){
                                data_temp1[0].data_uint=0;
                                data_temp1[1].data_uint=0;
                                data_temp1[2].data_uint=0;
                                data_temp1[3].data_uint=0;
                            }
                            else{
                                data_temp1[0].data_uint=c_buf[z][x][j][i](31,0);
                                data_temp1[1].data_uint=c_buf[z][x][j][i](63,32);
                                data_temp1[2].data_uint=c_buf[z][x][j][i](95,64);
                                data_temp1[3].data_uint=c_buf[z][x][j][i](127,96);
                            }
                            

                            data_temp2[0].data_float=data_temp0[0].data_float + data_temp0[0].data_float; 
                            data_temp2[1].data_float=data_temp0[1].data_float + data_temp0[1].data_float;
                            data_temp2[2].data_float=data_temp0[2].data_float + data_temp0[2].data_float;
                            data_temp2[3].data_float=data_temp0[3].data_float + data_temp0[3].data_float;

                            c_buf[z][x][j][i](31,0)   =  data_temp2[0].data_uint;  
                            c_buf[z][x][j][i](63,32)  =  data_temp2[1].data_uint;
                            c_buf[z][x][j][i](95,64)  =  data_temp2[2].data_uint;
                            c_buf[z][x][j][i](127,96) =  data_temp2[3].data_uint;
                            
                        }
                    }
                }
            }
        }
    }
}