

void compute(axis_stream_A& dataA_out, axis_stream_B& dataB_out, axis_stream_C& dataC_in,
             {% set num_lhs=A*NUM_TXL*(B//PACK_IN) -%}
             {% set div_lhs=num_lhs//4 -%}
             {% set left_lhs=num_lhs%4 -%}
             {% for i in range(div_lhs) -%}
             axis_stream& txA{{i*4}}, axis_stream& txA{{i*4+1}}, axis_stream& txA{{i*4+2}}, axis_stream& txA{{i*4+3}},
             {% endfor-%}
             {% for i in range(left_lhs)%} axis_stream& txA{{i+div_lhs*4}},{% endfor-%}
             
             {% set num_rhs=C*NUM_TXR*(B//PACK_IN) -%}
             {% set div_rhs=num_rhs//4 -%}
             {% set left_rhs=num_rhs%4 -%}
             {% for i in range(div_rhs) -%}
             axis_stream& txB{{i*4}}, axis_stream& txB{{i*4+1}}, axis_stream& txB{{i*4+2}}, axis_stream& txB{{i*4+3}},
             {% endfor-%}
             {% for i in range(left_rhs)%} axis_stream& txB{{i+div_rhs*4}},{% endfor-%}

             {% set num_out=A*(C//PACK_OUT) -%}
             {% set div_out=num_out//4 -%}
             {% set left_out=num_out%4 -%}
             {% for i in range(div_out) -%}
             axis_stream& rxC{{i*4}}, axis_stream& rxC{{i*4+1}}, axis_stream& rxC{{i*4+2}}, axis_stream& rxC{{i*4+3}},
             {% endfor-%}
             {% for i in range(left_out)%} axis_stream& rxC{{i+div_out*4}},{% endfor-%}

             const int TX, const int TY, const int TZ){

    ap_uint<BUFF_WIDTH> buff0_A[A*(B/PACK_IN)][X*Y][PACK_IN][LEFT_SIZE_BUFF];
    #pragma HLS bind_storage variable=buff0_A type=RAM_T2P impl={{BUFF_SEL[0]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_A block factor=BUFFA_FACTOR dim=4
    
    ap_uint<BUFF_WIDTH> buff1_A[A*(B/PACK_IN)][X*Y][PACK_IN][LEFT_SIZE_BUFF];
    #pragma HLS bind_storage variable=buff1_A type=RAM_T2P impl={{BUFF_SEL[0]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_A block factor=BUFFA_FACTOR dim=4

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACK_IN)*C][Z*Y][PACK_IN][RIGHT_SIZE];
    #pragma HLS bind_storage variable=buff0_B type=RAM_T2P impl={{BUFF_SEL[1]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_B block factor=BUFFB_FACTOR dim=4

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACK_IN)*C][Z*Y][PACK_IN][RIGHT_SIZE];
    #pragma HLS bind_storage variable=buff1_B type=RAM_T2P impl={{BUFF_SEL[1]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_B block factor=BUFFB_FACTOR dim=4

    ap_uint<BUFF_WIDTH> buff0_C[(C/PACK_OUT)*A][2][Z*X][PACK_OUT][OUT_SIZE_BUFF/2];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl={{BUFF_SEL[2]}}
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=2
    #pragma HLS ARRAY_PARTITION variable=buff0_C block factor=BUFFC_FACTOR dim=5

    ap_uint<BUFF_WIDTH> buff1_C[(C/PACK_OUT)*A][2][Z*X][PACK_OUT][OUT_SIZE_BUFF/2];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl={{BUFF_SEL[2]}}
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=2
    #pragma HLS ARRAY_PARTITION variable=buff1_C block factor=BUFFC_FACTOR dim=5

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
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendA<{{num}}>(buff1_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendB<{{num}}>(buff1_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C//PACK_OUT) -%}
            {% set num=(j+k*(C//PACK_OUT)) -%}
            receiveC<{{num}}>(buff0_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==1&&c_flg==0){
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendA<{{num}}>(buff0_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendB<{{num}}>(buff0_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C//PACK_OUT) -%}
            {% set num=(j+k*(C//PACK_OUT)) -%}
            receiveC<{{num}}>(buff0_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendA<{{num}}>(buff1_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendB<{{num}}>(buff1_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C//PACK_OUT) -%}
            {% set num=(j+k*(C//PACK_OUT)) -%}
            receiveC<{{num}}>(buff1_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
        else{ //if(rd%2==1&&c_flg==1)
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);   

            {% for k in range(A) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendA<{{num}}>(buff0_A[{{num}}],
            {%- for i in range(NUM_TXL) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXL) -%} txA{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(C) -%}
            {% for j in range(B//PACK_IN) -%}
            {% set num=(j+k*(B//PACK_IN)) -%}
            sendB<{{num}}>(buff0_B[{{num}}],
            {%- for i in range(NUM_TXR) -%} {% set port=(i*(B//PACK_IN)+j+k*(B//PACK_IN)*NUM_TXR) -%} txB{{port}}, {%- endfor-%}
            rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            {% for k in range(A) -%}
            {% for j in range(C//PACK_OUT) -%}
            {% set num=(j+k*(C//PACK_OUT)) -%}
            receiveC<{{num}}>(buff1_C[{{num}}],rxC{{num}}, rd>0&&rd<Total_rd+1);
            {% endfor-%}
            {% endfor%}

            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
    }
}