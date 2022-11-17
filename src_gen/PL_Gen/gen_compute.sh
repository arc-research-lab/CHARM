if [ "$#" -eq 16 ]
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
    AXI_WIDTH_A=${12};
    AXI_WIDTH_B=${13};
    AXI_WIDTH_C=${14};
    BUFF_WIDTH=${15};
    data_type=${16};
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

if [ ${data_type} == "int8" ]
then
    PARTITION="block"
    if [ ${AXI_WIDTH_A} == 512 ]
    then
        FACTOR_A=4;
    elif [ ${AXI_WIDTH_A} == 256 ]
    then
        FACTOR_A=2;
    fi

    if [ ${AXI_WIDTH_B} == 512 ]
    then
        FACTOR_B=2;
    elif [ ${AXI_WIDTH_B} == 256 ]
    then
        FACTOR_B=1;
    fi

    if [ ${AXI_WIDTH_C} == 512 ]
    then
        FACTOR_C=4;
    elif [ ${AXI_WIDTH_C} == 256 ]
    then
        FACTOR_C=2;
    fi
else
    PARTITION="cyclic"
    if [ ${AXI_WIDTH_A} == 512 ]
    then
        FACTOR_A=2;
    elif [ ${AXI_WIDTH_A} == 256 ]
    then
        FACTOR_A=1;
    fi

    if [ ${AXI_WIDTH_B} == 512 ]
    then
        FACTOR_B=2;
    elif [ ${AXI_WIDTH_B} == 256 ]
    then
        FACTOR_B=1;
    fi

    if [ ${AXI_WIDTH_C} == 512 ]
    then
        FACTOR_C=2;
    elif [ ${AXI_WIDTH_C} == 256 ]
    then
        FACTOR_C=1;
    fi
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
    ap_uint<BUFF_WIDTH> buff0_A[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE_BUFF*PACKET_NUM_IN];
    #pragma HLS bind_storage variable=buff0_A type=RAM_T2P impl=${L_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_A ${PARTITION} factor=${FACTOR_A} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1

    ap_uint<BUFF_WIDTH> buff1_A[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE_BUFF*PACKET_NUM_IN];
    #pragma HLS bind_storage variable=buff1_A type=RAM_T2P impl=${L_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_A ${PARTITION} factor=${FACTOR_A} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN];
    #pragma HLS bind_storage variable=buff0_B type=RAM_T2P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_B ${PARTITION} factor=${FACTOR_B} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN];
    #pragma HLS bind_storage variable=buff1_B type=RAM_T2P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_B ${PARTITION} factor=${FACTOR_B} dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1

    ap_uint<BUFF_WIDTH> buff0_C[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE_BUFF];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_C ${PARTITION} factor=${FACTOR_C} dim=4
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1

    ap_uint<BUFF_WIDTH> buff1_C[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE_BUFF];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_C ${PARTITION} factor=${FACTOR_C} dim=4
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1">> ./${dir_name}/kernel/dma.cpp;

if [ ${data_type} == "int8" ]
then
if [ ${AXI_WIDTH_C} == 512 ]
then
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM_OUT;j++){
            for (int i = 0; i < OUT_SIZE/8; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                    buff0_C[a][x][j][i]=0; 
                    buff0_C[a][x][j][i+64]=0;
                    buff0_C[a][x][j][i+128]=0; 
                    buff0_C[a][x][j][i+192]=0;
                    buff1_C[a][x][j][i+256]=0; 
                    buff1_C[a][x][j][i+320]=0; 
                    buff1_C[a][x][j][i+384]=0; 
                    buff1_C[a][x][j][i+448]=0;
                }
            }
        }
    }">> ./${dir_name}/kernel/dma.cpp;
elif [ ${AXI_WIDTH_C} == 256 ]
then
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM_OUT;j++){
            for(int w2=0;w2<W2;w2++){
                    for (int n=0; n<H1/C_PER_TRA;n++){
                #pragma HLS PIPELINE II = 1
                    int pos0=n*256+w2;
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        buff0_C[a][x][j][pos0]=0; 
                        buff0_C[a][x][j][pos0+64]=0;
                        buff0_C[a][x][j][pos0+128]=0; 
                        buff0_C[a][x][j][pos0+192]=0;
                    }
                }
            }
        }
    }">> ./${dir_name}/kernel/dma.cpp;
fi

else
if [ ${AXI_WIDTH_C} == 512 ]
then
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM_OUT;j++){
            for (int i = 0; i < OUT_SIZE/4; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
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
elif [ ${AXI_WIDTH_C} == 256 ]
then
echo \
"
    const int Total_rd=TX*TY*TZ;
    for(int x = 0; x < X*Z; x++){
        for(int j=0;j<PACKET_NUM_OUT;j++){
            for (int i = 0; i < OUT_SIZE/2; i++){
            #pragma HLS PIPELINE II = 1
                for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                    buff0_C[a][x][j][i*2+0]=0; 
                    buff0_C[a][x][j][i*2+1]=0;
                    buff1_C[a][x][j][i*2+0]=0; 
                    buff1_C[a][x][j][i*2+1]=0; 
                }
            }
        }
    }">> ./${dir_name}/kernel/dma.cpp;
fi
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


for ((i=0;i<${port_row_in}/${NUM_TXA};i++));
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

for ((i=0;i<${port_col_in}/${NUM_TXB};i++));
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


for ((i=0;i<${port_row_in}/${NUM_TXA};i++));
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

for ((i=0;i<${port_col_in}/${NUM_TXB};i++));
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
        else if(rd%2==0&&c_flg==1){
            loadA(dataA_out,buff0_A,rd<Total_rd);
            loadB(dataB_out,buff0_B,rd<Total_rd);          
">> ./${dir_name}/kernel/dma.cpp;


for ((i=0;i<${port_row_in}/${NUM_TXA};i++));
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

for ((i=0;i<${port_col_in}/${NUM_TXB};i++));
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


for ((i=0;i<${port_row_in}/${NUM_TXA};i++));
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

for ((i=0;i<${port_col_in}/${NUM_TXB};i++));
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
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;