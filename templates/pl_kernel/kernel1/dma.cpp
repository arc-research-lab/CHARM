

void dma(ap_uint<AXI_WIDTH_A>* ina, ap_uint<AXI_WIDTH_B>* inb, ap_uint<AXI_WIDTH_C>* out0,
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
    
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=ina
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=inb max_read_burst_length=16 num_read_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=inb
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=out0 max_write_burst_length=16 num_write_outstanding=64
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=TX
    #pragma HLS interface s_axilite bundle=control port=TY
    #pragma HLS interface s_axilite bundle=control port=TZ
    {% for k in range(A) -%}
    {% for j in range(NUM_TXL) -%}
    {% for i in range(B//PACK_IN) -%} 
    {% set num=(i+j*(B//PACK_IN)+k*NUM_TXL*(B//PACK_IN)) -%} 
    #pragma HLS interface axis port=txA{{num}}
    {% endfor-%}
    {% endfor-%}
    {% endfor-%}

    {% for k in range(C) -%}
    {% for j in range(NUM_TXR) -%}
    {% for i in range(B//PACK_IN) -%} 
    {% set num=(i+j*(B//PACK_IN)+k*NUM_TXR*(B//PACK_IN)) -%} 
    #pragma HLS interface axis port=txB{{num}}
    {% endfor-%}
    {% endfor-%}
    {% endfor-%}

    {% for j in range(A) -%}
    {% for i in range(C//PACK_OUT) -%}
    {% set num=(i+j*(C//PACK_OUT)) -%} 
    #pragma HLS interface axis port=rxC{{num}}
    {% endfor-%}
    {% endfor-%}
    #pragma HLS interface s_axilite bundle=control port=return

    #pragma HLS dataflow
    axis_stream_A dataA_out;
    axis_stream_B dataB_out;
    axis_stream_C dataC_in;
    axis_stream_32 addrA_out;
    axis_stream_32 addrB_out;
    axis_stream_32 addrC_out;

    address_A_ddr(addrA_out,TX,TY,TZ);
    loadA_ddr(ina, addrA_out,dataA_out,TX,TY,TZ);

    address_B_ddr(addrB_out,TX,TY,TZ);
    loadB_ddr(inb,addrB_out,dataB_out,TX,TY,TZ);

    address_C_ddr(addrC_out,TX,TZ);
    storeC_ddr(out0,addrC_out,dataC_in,TX,TZ);

    compute(dataA_out, dataB_out, dataC_in,
            {% set num_lhs=A*NUM_TXL*(B//PACK_IN) -%}
            {% set div_lhs=num_lhs//4 -%}
            {% set left_lhs=num_lhs%4 -%}
            {% for i in range(div_lhs) -%}
            txA{{i*4}}, txA{{i*4+1}}, txA{{i*4+2}}, txA{{i*4+3}},
            {% endfor-%}
            {% for i in range(left_lhs)%} txA{{i+div_lhs*4}},{% endfor-%}
                    
            {% set num_rhs=C*NUM_TXR*(B//PACK_IN) -%}
            {% set div_rhs=num_rhs//4 -%}
            {% set left_rhs=num_rhs%4 -%}
            {% for i in range(div_rhs) -%}
            txB{{i*4}}, txB{{i*4+1}}, txB{{i*4+2}}, txB{{i*4+3}},
            {% endfor-%}
            {% for i in range(left_rhs)%} txB{{i+div_rhs*4}},{% endfor-%}
                    
            {% set num_out=A*(C//PACK_OUT) -%}
            {% set div_out=num_out//4 -%}
            {% set left_out=num_out%4 -%}
            {% for i in range(div_out) -%}
            rxC{{i*4}}, rxC{{i*4+1}}, rxC{{i*4+2}}, rxC{{i*4+3}},
            {% endfor-%}
            {% for i in range(left_out)%} rxC{{i+div_out*4}},{% endfor-%}
            TX, TY, TZ);

}