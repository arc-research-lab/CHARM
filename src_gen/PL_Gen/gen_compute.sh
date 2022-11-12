if [ "$#" -eq 12 ] 
then
    dir_name=$1;
    port_row_in=$2;
    port_col_in=$3;
    port_out=$4;
    l_buff=$5;
    r_buff=$6;
    o_buff=$7;
    NUM_TXA=$8;
    NUM_TXB=$9;
    A=${10};
    C=${11};
    NUM_PACK=${12};
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_compute Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi

if (( ${l_buff} == 0 ))
then
    L_buffer="BRAM";
else
    L_buffer="URAM";
fi
if (( ${r_buff} == 0 ))
then
    R_buffer="BRAM";
else
    R_buffer="URAM";
fi
if (( ${o_buff} == 0 ))
then
    O_buffer="BRAM";
else
    O_buffer="URAM";
fi

if [ $((${A}%2)) == 0 ]
then
	FACTOR_A=4;
else
	FACTOR_A=2;
fi

if [ $((${NUM_PACK}%2)) == 0 ]
then
	FACTOR_B=4;
else
	FACTOR_B=2;
fi

if [ $((${NUM_PACK}%2)) != 0 ] && [ $((${A}%2)) == 0 ] && [ ${C} -ge ${NUM_PACK} ]
then
	FACTOR_C=2;
else
	FACTOR_C=1;
fi


echo \
"
void compute(axis_stream_A& dataA_out, axis_stream_B& dataB_out, axis_stream_C& dataC_in,const int TX, const int TY, const int TZ,">> ./${dir_name}/kernel/dma.cpp;

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
"{
    ap_uint<PLIO_WIDTH> buff0_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_A type=RAM_1P impl=${L_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=${FACTOR_A} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1

    ap_uint<PLIO_WIDTH> buff1_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_A type=RAM_1P impl=${L_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=${FACTOR_A} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_B type=RAM_1P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=${FACTOR_B} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_B type=RAM_1P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=${FACTOR_B} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1

    ap_uint<PLIO_WIDTH> buff0_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_C cyclic factor=${FACTOR_C} dim=4
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1

    ap_uint<PLIO_WIDTH> buff1_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_C cyclic factor=${FACTOR_C} dim=4
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1">> ./${dir_name}/kernel/dma.cpp;

if [ $((${NUM_PACK}%2)) != 0 ] && [ $((${A}%2)) == 0 ] && [ ${C} -ge ${NUM_PACK} ]
then
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM;j++){
            for (int i = 0; i < OUT_SIZE/4; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM); a++){
                    buff0_C[a][x][j][i*4+0]=0; 
                    buff0_C[a][x][j][i*4+1]=0;
                    buff0_C[a][x][j][i*4+2]=0; 
                    buff0_C[a][x][j][i*4+3]=0;
                    buff1_C[a][x][j][i*4+0]=0; 
                    buff1_C[a][x][j][i*4+1]=0; 
                    buff1_C[a][x][j][i*4+2]=0; 
                    buff1_C[a][x][j][i*4+3]=0;
                }
            }
        }
    }">> ./${dir_name}/kernel/dma.cpp;
else
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM;j++){
            for (int i = 0; i < OUT_SIZE/2; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM); a++){
                    buff0_C[a][x][j][i*2+0]=0; 
                    buff0_C[a][x][j][i*2+1]=0;
                    buff1_C[a][x][j][i*2+0]=0; 
                    buff1_C[a][x][j][i*2+1]=0; 
                }
            }
        }
    }">> ./${dir_name}/kernel/dma.cpp;
fi

echo \
"
    for (int rd=0; rd<Total_rd+2;rd++){
        int c_flg=0,s_flg=0;
        if(rd>0){
            c_flg=((rd-1)/TY)%2;
        }
        if(rd>1){
            s_flg=(rd-2)%TY;
        }">> ./${dir_name}/kernel/dma.cpp;

echo "\
        if(rd%2==0&&c_flg==0){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);          
">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${A};i++));
do  
    echo -n "            sendA<${i}>(buff1_A[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXA};j++));
    do  
        let port_a=${j}+${i}*${NUM_TXA};
        echo -n "txA_${port_a},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${C};i++));
do  
    echo -n "            sendB<${i}>(buff1_B[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXB};j++));
    do  
        let port_b=${j}+${i}*${NUM_TXB};
        echo -n "txB_${port_b},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${port_out};i++));
do  
    echo "            reshapeC<${i}>(buff0_C[${i}],rxC_${i},rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }">> ./${dir_name}/kernel/dma.cpp;

################################
echo "\
        else if(rd%2==1&&c_flg==0){
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);          
">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${A};i++));
do  
    echo -n "            sendA<${i}>(buff0_A[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXA};j++));
    do  
        let port_a=${j}+${i}*${NUM_TXA};
        echo -n "txA_${port_a},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${C};i++));
do  
    echo -n "            sendB<${i}>(buff0_B[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXB};j++));
    do  
        let port_b=${j}+${i}*${NUM_TXB};
        echo -n "txB_${port_b},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${port_out};i++));
do  
    echo "            reshapeC<${i}>(buff0_C[${i}],rxC_${i},rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"            storeC(dataC_in, buff1_C, rd>TY&&s_flg==(TY-1));
        }">> ./${dir_name}/kernel/dma.cpp;

################################
echo "\
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);          
">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${A};i++));
do  
    echo -n "            sendA<${i}>(buff1_A[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXA};j++));
    do  
        let port_a=${j}+${i}*${NUM_TXA};
        echo -n "txA_${port_a},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${C};i++));
do  
    echo -n "            sendB<${i}>(buff1_B[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXB};j++));
    do  
        let port_b=${j}+${i}*${NUM_TXB};
        echo -n "txB_${port_b},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${port_out};i++));
do  
    echo "            reshapeC<${i}>(buff1_C[${i}],rxC_${i},rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }">> ./${dir_name}/kernel/dma.cpp;

################################
echo "\
        else{ //if(rd%2==1&&c_flg==1)
            loadA(dataA_out,buff1_A,rd<Total_rd);
            loadB(dataB_out,buff1_B,rd<Total_rd);          
">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${A};i++));
do  
    echo -n "            sendA<${i}>(buff0_A[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXA};j++));
    do  
        let port_a=${j}+${i}*${NUM_TXA};
        echo -n "txA_${port_a},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${C};i++));
do  
    echo -n "            sendB<${i}>(buff0_B[${i}],">> ./${dir_name}/kernel/dma.cpp;
    for ((j=0;j<${NUM_TXB};j++));
    do  
        let port_b=${j}+${i}*${NUM_TXB};
        echo -n "txB_${port_b},">> ./${dir_name}/kernel/dma.cpp;
    done
    echo -n "rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
    echo "">> ./${dir_name}/kernel/dma.cpp;
done
echo "">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${port_out};i++));
do  
    echo "            reshapeC<${i}>(buff1_C[${i}],rxC_${i},rd>0&&rd<Total_rd+1);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"            storeC(dataC_in, buff0_C, rd>TY&&s_flg==(TY-1));
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;