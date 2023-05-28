
void compute(axis_stream_A& dataA_out, axis_stream_B& dataB_out, axis_stream_C& dataC_in,
             {% for k in range(A) -%}
             {% for j in range(NUM_TXL) -%}
             {% for i in range(B) -%} {% set num=(i+j*B+k*NUM_TXL*B) -%} axis_stream& txA{{num}}, {% endfor%}
             {% endfor-%}
             {% endfor-%}

             {% for k in range(C) -%}
             {% for j in range(NUM_TXR) -%}
             {% for i in range(B) -%} {% set num=(i+j*B+k*NUM_TXR*B) -%} axis_stream& txB{{num}}, {% endfor%}
             {% endfor-%}
             {% endfor-%}

             {% for j in range(A) -%}
             {% for i in range(C) -%} {% set num=(i+j*C) -%} axis_stream& rxC{{num}}, {% endfor%}
             {% endfor-%}
             const int TX, const int TY, const int TZ){

    ap_uint<PLIO_WIDTH> buff0_A[A*B][X][Y][W1][(H1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff0_A type=RAM_T2P impl={{BUFF_SEL[0]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=BUFFA_FACTOR dim=5
    
    ap_uint<PLIO_WIDTH> buff1_A[A*B][X][Y][W1][(H1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff1_A type=RAM_T2P impl={{BUFF_SEL[0]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=BUFFA_FACTOR dim=5

    ap_uint<PLIO_WIDTH> buff0_B[B*C][Z][Y][W2][(W1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff0_B type=RAM_T2P impl={{BUFF_SEL[1]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=BUFFB_FACTOR dim=5

    ap_uint<PLIO_WIDTH> buff1_B[B*C][Z][Y][W2][(W1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff1_B type=RAM_T2P impl={{BUFF_SEL[1]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=BUFFB_FACTOR dim=5

    ap_uint<PLIO_WIDTH> buff0_C[C*A][Z][X][W2][(H1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl={{BUFF_SEL[2]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_C cyclic factor=BUFFC_FACTOR dim=5

    ap_uint<PLIO_WIDTH> buff1_C[C*A][Z][X][W2][(H1/NUM_PER_TRA)];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl={{BUFF_SEL[2]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_C cyclic factor=BUFFC_FACTOR dim=5

    const int Total_rd=TX*TY*TZ;

    for (int rd=0; rd<Total_rd+2;rd++){
        int c_flg=0,s_flg=0;
        if(rd>0){
            c_flg=((rd-1)/TY)%2;
        }
        if(rd>1){
            s_flg=(rd-2)%TY;
        }
        if(rd%2==0&&c_flg==0){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendA<{{num}}>(buff1_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*B+j+k*B*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendB<{{num}}>(buff1_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*B+j+k*B*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C) -%}
            {% set num=(j+k*C) -%}
            receiveC<{{num}}>(buff0_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==1&&c_flg==0){
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendA<{{num}}>(buff0_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*B+j+k*B*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendB<{{num}}>(buff0_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*B+j+k*B*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C) -%}
            {% set num=(j+k*C) -%}
            receiveC<{{num}}>(buff0_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendA<{{num}}>(buff1_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*B+j+k*B*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendB<{{num}}>(buff1_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*B+j+k*B*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C) -%}
            {% set num=(j+k*C) -%}
            receiveC<{{num}}>(buff1_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
        else{ //if(rd%2==1&&c_flg==1)
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendA<{{num}}>(buff0_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*B+j+k*B*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B) -%}
            {% set num=(j+k*B) -%}
            sendB<{{num}}>(buff0_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*B+j+k*B*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C) -%}
            {% set num=(j+k*C) -%}
            receiveC<{{num}}>(buff1_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
    }
}