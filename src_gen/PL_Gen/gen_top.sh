if [ "$#" -eq 4 ] 
then
    dir_name=$1;
    port_row_in=$2;
    port_col_in=$3;
    port_out=$4;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_top Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi

echo \
"
void dma(ap_uint<AXI_WIDTH_512>* ina, ap_uint<AXI_WIDTH_512>* inb, ap_uint<AXI_WIDTH_256>* out0, const int TX, const int TY, const int TZ,">> ./${dir_name}/kernel/dma.cpp;

echo -n "             ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${port_row_in};i++));
do  
    echo -n "axis_stream& txA_${i}, ">> ./${dir_name}/kernel/dma.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/kernel/dma.cpp;
        echo -n "             ">> ./${dir_name}/kernel/dma.cpp;
    fi
done

for ((i=0;i<${port_col_in};i++));
do  
    echo -n "axis_stream& txB_${i}, ">> ./${dir_name}/kernel/dma.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/kernel/dma.cpp;
        echo -n "             ">> ./${dir_name}/kernel/dma.cpp;
    fi
done

for ((i=0;i<${port_out};i++));
do  
    if (( ${i} != ${port_out}-1 ))
    then
        echo -n "axis_stream& rxC_${i}, ">> ./${dir_name}/kernel/dma.cpp;
        if (( ${i}%4 == 3 ))
        then
            echo "">> ./${dir_name}/kernel/dma.cpp;
            echo -n "             ">> ./${dir_name}/kernel/dma.cpp;
        fi
    else
        echo -n "axis_stream& rxC_${i})">> ./${dir_name}/kernel/dma.cpp;
    fi
done

echo \
"
{
    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina max_read_burst_length=16 num_read_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=ina
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=inb max_read_burst_length=16 num_read_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=inb
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=out0 max_write_burst_length=16 num_write_outstanding=16
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=TX
    #pragma HLS interface s_axilite bundle=control port=TY
    #pragma HLS interface s_axilite bundle=control port=TZ">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${port_row_in};i++));
do  
    echo "    #pragma HLS interface axis port=txA_${i}">> ./${dir_name}/kernel/dma.cpp;
done

for ((i=0;i<${port_col_in};i++));
do  
    echo "    #pragma HLS interface axis port=txB_${i}">> ./${dir_name}/kernel/dma.cpp;  
done

for ((i=0;i<${port_out};i++));
do  
    echo "    #pragma HLS interface axis port=rxC_${i}">> ./${dir_name}/kernel/dma.cpp;      
done

echo \
"    #pragma HLS interface s_axilite bundle=control port=return
    
    #pragma HLS dataflow

    axis_stream_512 dataA_out;
    axis_stream_512 dataB_out;
    axis_stream_256 dataC_in;
    axis_stream_32 addrA_out;
    axis_stream_32 addrB_out;
    axis_stream_32 addrC_out;

    address_A_ddr(addrA_out,TX,TY,TZ);
    loadA_ddr(ina, addrA_out,dataA_out,TX,TY,TZ);

    address_B_ddr(addrB_out,TX,TY,TZ);
    loadB_ddr(inb,addrB_out,dataB_out,TX,TY,TZ);

    address_C_ddr(addrC_out,TX,TZ);
    storeC_ddr(out0,addrC_out,dataC_in,TX,TZ);

    compute(dataA_out, dataB_out, dataC_in, TX, TY, TZ,">> ./${dir_name}/kernel/dma.cpp;

echo -n "            ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${port_row_in};i++));
do  
    echo -n "txA_${i}, ">> ./${dir_name}/kernel/dma.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/kernel/dma.cpp;
        echo -n "            ">> ./${dir_name}/kernel/dma.cpp;
    fi
done

for ((i=0;i<${port_col_in};i++));
do  
    echo -n "txB_${i}, ">> ./${dir_name}/kernel/dma.cpp;
    if (( ${i}%4 == 3 ))
    then
        echo "">> ./${dir_name}/kernel/dma.cpp;
        echo -n "            ">> ./${dir_name}/kernel/dma.cpp;
    fi
done

for ((i=0;i<${port_out};i++));
do  
    if (( ${i} != ${port_out}-1 ))
    then
        echo -n "rxC_${i}, ">> ./${dir_name}/kernel/dma.cpp;
        if (( ${i}%4 == 3 ))
        then
            echo "">> ./${dir_name}/kernel/dma.cpp;
            echo -n "            ">> ./${dir_name}/kernel/dma.cpp;
        fi
    else
        echo -n "rxC_${i});">> ./${dir_name}/kernel/dma.cpp;
    fi
done

echo \
"
}">> ./${dir_name}/kernel/dma.cpp;