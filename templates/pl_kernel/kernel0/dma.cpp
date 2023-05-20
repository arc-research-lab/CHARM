void dma(ap_uint<AXI_WIDTH_A>* ina, ap_uint<AXI_WIDTH_B>* inb, ap_uint<AXI_WIDTH_C>* out0,
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
    {% for i in range(B) -%} 
    {% set num=(i+j*B+k*NUM_TXL*B) -%} 
    #pragma HLS interface axis port=txA{{num}}
    {% endfor-%}
    {% endfor-%}
    {% endfor-%}

    {% for k in range(C) -%}
    {% for j in range(NUM_TXR) -%}
    {% for i in range(B) -%} 
    {% set num=(i+j*B+k*NUM_TXR*B) -%} 
    #pragma HLS interface axis port=txB{{num}}
    {% endfor-%}
    {% endfor-%}
    {% endfor-%}

    {% for j in range(A) -%}
    {% for i in range(C) -%}
    {% set num=(i+j*C) -%} 
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
            {% for k in range(A) -%}
            {% for j in range(NUM_TXL) -%}
            {% for i in range(B) -%} {% set num=(i+j*B+k*NUM_TXL*B) -%} txA{{num}}, {% endfor%}
            {% endfor-%}
            {% endfor-%}

            {% for k in range(C) -%}
            {% for j in range(NUM_TXR) -%}
            {% for i in range(B) -%} {% set num=(i+j*B+k*NUM_TXR*B) -%} txB{{num}}, {% endfor%}
            {% endfor-%}
            {% endfor-%}

            {% for j in range(A) -%}
            {% for i in range(C) -%} {% set num=(i+j*C) -%} rxC{{num}}, {% endfor%}
            {% endfor-%}
            TX, TY, TZ);

}