
template<int NC>
void sendA(ap_uint<BUFF_WIDTH> a_buf[X*Y][LEFT_SIZE_BUFF],
           {% for i in range(NUM_TXL) -%} axis_stream& txA{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;
        data_t da;
        
        for (int z = 0; z < Z; z++) {
            for (int xy = 0; xy < X*Y; xy++) {//y first then x 
                for (int i = 0; i < LEFT_SIZE_BUFF/2; i++){ 
                #pragma HLS PIPELINE II = 1
                    data(63,0)=a_buf[xy][i*2];
                    data(127,64)=a_buf[xy][i*2+1];
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

template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE],
           {% for i in range(NUM_TXR) -%} axis_stream& txB{{i}}, {% endfor%}
           bool enable){

#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        data_t data;

        for (int z = 0; z < Z; z++) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++){
                    for (int i = 0; i < RIGHT_SIZE; i++){ 
                    #pragma HLS PIPELINE II = 1
                        data=b_buf[pos1][i];
                        tmp.data   = da;
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

template<int NC>
void receiveC(ap_uint<BUFF_WIDTH> c_buf[2][X*Z][OUT_SIZE_BUFF/2],axis_stream& rxC, bool enable){ 

#pragma HLS inline off
if (enable){
        
    axis_pkt tmp; 
    comb_32 data_temp[4];
    #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
    comb_32 d0,d1,d2,d3;

    for(int xz = 0; xz < X*Z; xz++){
        for (int n = 0; n < Y; n++){
            for(int i=0;i<OUT_SIZE_BUFF/2;i++){
            #pragma HLS PIPELINE II = 1
            #pragma HLS dependence variable=c_buf type=inter false
                tmp=rxC.read();
                
                data_temp[0].low0=tmp.data(7,0);
                data_temp[0].low1=tmp.data(15,8);
                data_temp[0].high0=tmp.data(23,16);
                data_temp[0].high1=tmp.data(31,24);
                data_temp[1].low0=tmp.data(39,32);
                data_temp[1].low1=tmp.data(47,40);
                data_temp[1].high0=tmp.data(55,48);
                data_temp[1].high1=tmp.data(63,56);
                data_temp[2].low0=tmp.data(71,64);
                data_temp[2].low1=tmp.data(79,72);
                data_temp[2].high0=tmp.data(87,80);
                data_temp[2].high1=tmp.data(95,88);
                data_temp[3].low0=tmp.data(103,96);
                data_temp[3].low1=tmp.data(111,104);
                data_temp[3].high0=tmp.data(119,112);
                data_temp[3].high1=tmp.data(127,120);
                
                d0.low0  =c_buf[0][xz][i](7,0)  ;
                d0.low1  =c_buf[0][xz][i](15,8) ;
                d0.high0 =c_buf[0][xz][i](23,16);
                d0.high1 =c_buf[0][xz][i](31,24);
                d1.low0  =c_buf[0][xz][i](39,32);
                d1.low1  =c_buf[0][xz][i](47,40);
                d1.high0 =c_buf[0][xz][i](55,48);
                d1.high1 =c_buf[0][xz][i](63,56);
                d2.low0  =c_buf[1][xz][i](7,0)  ;
                d2.low1  =c_buf[1][xz][i](15,8) ;
                d2.high0 =c_buf[1][xz][i](23,16);
                d2.high1 =c_buf[1][xz][i](31,24);
                d3.low0  =c_buf[1][xz][i](39,32);
                d3.low1  =c_buf[1][xz][i](47,40);
                d3.high0 =c_buf[1][xz][i](55,48);
                d3.high1 =c_buf[1][xz][i](63,56);
                d0.low0  = data_temp[0].low0       + d0.low0 ;
                d0.low1  = data_temp[0].low1       + d0.low1 ;
                d0.high0 = data_temp[0].high0      + d0.high0;
                d0.high1 = data_temp[0].high1      + d0.high1;
                d1.low0  = data_temp[1].low0       + d1.low0 ;
                d1.low1  = data_temp[1].low1       + d1.low1 ;
                d1.high0 = data_temp[1].high0      + d1.high0;
                d1.high1 = data_temp[1].high1      + d1.high1;
                d2.low0  = data_temp[2].low0       + d2.low0 ;
                d2.low1  = data_temp[2].low1       + d2.low1 ;
                d2.high0 = data_temp[2].high0      + d2.high0;
                d2.high1 = data_temp[2].high1      + d2.high1;
                d3.low0  = data_temp[3].low0   + d3.low0 ;
                d3.low1  = data_temp[3].low1   + d3.low1 ;
                d3.high0 = data_temp[3].high0  + d3.high0;
                d3.high1 = data_temp[3].high1  + d3.high1;
                
                c_buf[0][xz][i](7,0)    = d0.low0  ;
                c_buf[0][xz][i](15,8)   = d0.low1  ;
                c_buf[0][xz][i](23,16)  = d0.high0 ;
                c_buf[0][xz][i](31,24)  = d0.high1 ;
                c_buf[0][xz][i](39,32)  = d1.low0  ;
                c_buf[0][xz][i](47,40)  = d1.low1  ;
                c_buf[0][xz][i](55,48)  = d1.high0 ;
                c_buf[0][xz][i](63,56)  = d1.high1 ;
                c_buf[1][xz][i](7,0)    = d2.low0  ;
                c_buf[1][xz][i](15,8)   = d2.low1  ;
                c_buf[1][xz][i](23,16)  = d2.high0 ;
                c_buf[1][xz][i](31,24)  = d2.high1 ;
                c_buf[1][xz][i](39,32)  = d3.low0  ;
                c_buf[1][xz][i](47,40)  = d3.low1  ;
                c_buf[1][xz][i](55,48)  = d3.high0 ;
                c_buf[1][xz][i](63,56)  = d3.high1 ;
            }
        }
        
    }
}  
}