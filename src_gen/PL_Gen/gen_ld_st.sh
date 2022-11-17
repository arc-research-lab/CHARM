if [ "$#" -eq 7 ] 
then
    dir_name=$1;
    data_type=$2;
    AXI_WIDTH_A=$3;
    AXI_WIDTH_B=$4;
    AXI_WIDTH_C=$5;
    mm_k=$6;
    NUM_PACK_IN=$7;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_ld_sd Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi

if [ ${data_type} == "fp32" ] || [ ${data_type} == "int32" ]
then
if [ ${AXI_WIDTH_A} == 512 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A;a++){
                        for(int i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*4+(k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(127,0);
                            a_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                            a_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                            a_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
elif [ ${AXI_WIDTH_A} == 256 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A;a++){
                        for(int i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*2+(k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(127,0);
                            a_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_B} == 512 ]
then
echo \
"void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM_IN)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                                b_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                                b_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${AXI_WIDTH_B} == 256 ]
then
echo \
"void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*2+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM_IN)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_C} == 512 ]
then
echo \
"void storeC(axis_stream_C& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int aie_pack=c+a*C;
                                int pos0=n*4+w2*(H1/NUM_PER_TRA);
                                int pos1=aie_pack%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3=aie_pack/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                temp_data(127,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+1];
                                temp_data(383,256)=c_buf[pos3][pos2][pos1][pos0+2];
                                temp_data(511,384)=c_buf[pos3][pos2][pos1][pos0+3];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for (int i = 0; i < OUT_SIZE/4; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        c_buf[a][x][j][i*4+0]=0; 
                        c_buf[a][x][j][i*4+1]=0;
                        c_buf[a][x][j][i*4+2]=0; 
                        c_buf[a][x][j][i*4+3]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${AXI_WIDTH_C} == 256 ]
then
echo \
"void storeC(axis_stream_C& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int aie_pack=c+a*C;
                                int pos0=n*2+w2*(H1/NUM_PER_TRA);
                                int pos1=aie_pack%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3=aie_pack/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                temp_data(127,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+1];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

elif [ ${data_type} == "int16" ]
then
if [ ${AXI_WIDTH_A} == 512 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A/2;a++){
                        for(int i=0;i<(H1*2/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int temp= (k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=2*a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            if(i==0){
                                a_buf[pos2][pos1][temp]=temp_data(127,0);
                                a_buf[pos2][pos1][temp+1]=temp_data(255,128);
                                a_buf[pos2][pos1][temp+2]=temp_data(383,256);
                                a_buf[pos2][pos1][temp+3]=temp_data(511,384);
                            }
                            else if(i==1){
                                a_buf[pos2][pos1][temp+4]=temp_data(127,0);
                                a_buf[pos2][pos1][temp+5]=temp_data(255,128);
                                a_buf[pos2+1][pos1][temp+0]=temp_data(383,256);
                                a_buf[pos2+1][pos1][temp+1]=temp_data(511,384);
                            }
                            else{
                                a_buf[pos2+1][pos1][temp+2]=temp_data(127,0);
                                a_buf[pos2+1][pos1][temp+3]=temp_data(255,128);
                                a_buf[pos2+1][pos1][temp+4]=temp_data(383,256);
                                a_buf[pos2+1][pos1][temp+5]=temp_data(511,384);                       
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
elif [ ${AXI_WIDTH_A} == 256 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A;a++){
                        for(int i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*2+(k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(127,0);
                            a_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                        }
                    }
                }
            }
        }
    }
}
">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_B} == 512 ] && [ $((${mm_k}%32)) == 0 ]
then
echo \
"
void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM_IN)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                                b_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                                b_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                            }
                        }
                    }
                }
            }
        }
    }
}
">> ./${dir_name}/kernel/dma.cpp;

elif [ ${AXI_WIDTH_B} == 512 ] && [ $((${mm_k}%32)) != 0 ] [ $((${NUM_PACK_IN}%2)) == 0 ]
then
echo \
"
void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for (int b=0;b<(B/PACKET_NUM_IN);b++){
                            for (int m=0;m<(W1*PACKET_NUM_IN/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1*PACKET_NUM_IN/NUM_PER_TRA);
                                int pos1=z*Y+y;
                                int pos2=b*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                                b_buf[pos2][pos1][pos0+2]=temp_data(383,256);
                                b_buf[pos2][pos1][pos0+3]=temp_data(511,384);
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
elif [ ${AXI_WIDTH_B} == 256 ]
then
echo \
"
void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*2+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM_IN)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+1]=temp_data(255,128);
                            }
                        }
                    }
                }
            }
        }
    }
}   
">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_C} == 512 ]
then
echo \
"
void storeC(axis_stream_C& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A/2;a++){
                            for (int n=0; n<H1*2/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                            #pragma HLS dependence variable=c_buf type=intra false
                                int temp=c+(2*a)*C; 
                                int pos0=w2*(H1/NUM_PER_TRA);
                                int pos1_0=temp%PACKET_NUM_OUT;
                                int pos1_1=(temp+C)%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3_0=temp/PACKET_NUM_OUT;
                                int pos3_1=(temp+C)/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                if(n==0){
                                    temp_data(127,0)  =c_buf[pos3_0][pos2][pos1_0][pos0];
                                    temp_data(255,128)=c_buf[pos3_0][pos2][pos1_0][pos0+1];
                                    temp_data(383,256)=c_buf[pos3_0][pos2][pos1_0][pos0+2];
                                    temp_data(511,384)=c_buf[pos3_0][pos2][pos1_0][pos0+3];
                                }
                                else if(n==1){
                                    temp_data(127,0)  =c_buf[pos3_0][pos2][pos1_0][pos0+4];
                                    temp_data(255,128)=c_buf[pos3_0][pos2][pos1_0][pos0+5];
                                    temp_data(383,256)=c_buf[pos3_1][pos2][pos1_1][pos0];
                                    temp_data(511,384)=c_buf[pos3_1][pos2][pos1_1][pos0+1];
                                }
                                else{
                                    temp_data(127,0)  =c_buf[pos3_1][pos2][pos1_1][pos0+2];
                                    temp_data(255,128)=c_buf[pos3_1][pos2][pos1_1][pos0+3];
                                    temp_data(383,256)=c_buf[pos3_1][pos2][pos1_1][pos0+4];
                                    temp_data(511,384)=c_buf[pos3_1][pos2][pos1_1][pos0+5];                     
                                }
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for (int i = 0; i < OUT_SIZE/4; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        c_buf[a][x][j][i*4+0]=0; 
                        c_buf[a][x][j][i*4+1]=0;
                        c_buf[a][x][j][i*4+2]=0; 
                        c_buf[a][x][j][i*4+3]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp; 
elif [ ${AXI_WIDTH_C} == 256 ]
then
echo \
"
void storeC(axis_stream_C& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int aie_pack=c+a*C;
                                int pos0=n*2+w2*(H1/NUM_PER_TRA);
                                int pos1=aie_pack%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3=aie_pack/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                temp_data(127,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(255,128)=c_buf[pos3][pos2][pos1][pos0+1];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

elif [ ${data_type} == "int8" ]
then
if [ ${AXI_WIDTH_A} == 512 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<BUFF_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE_BUFF*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A;a++){
                        for(int i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*512+(k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_BUFF);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(63,0);
                            a_buf[pos2][pos1][pos0+64]=temp_data(127,64);
                            a_buf[pos2][pos1][pos0+128]=temp_data(191,128);
                            a_buf[pos2][pos1][pos0+192]=temp_data(255,192);
                            a_buf[pos2][pos1][pos0+256]=temp_data(319,256);
                            a_buf[pos2][pos1][pos0+320]=temp_data(383,320);
                            a_buf[pos2][pos1][pos0+384]=temp_data(447,384);
                            a_buf[pos2][pos1][pos0+448]=temp_data(511,448);
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
elif [ ${AXI_WIDTH_A} == 256 ]
then
echo \
"
void loadA(axis_stream_A& dataA_in, ap_uint<BUFF_WIDTH> a_buf[A*(B/PACKET_NUM_IN)][X*Y][LEFT_SIZE_BUFF*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int y=0;y<Y;y++){
            for(int k=0;k<W1*B;k++){
                for(int x=0;x<X;x++){
                    for(int a=0;a<A;a++){
                        for(int i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*256+(k%(W1*PACKET_NUM_IN))*(H1/NUM_PER_BUFF);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM_IN));
                            ap_uint<AXI_WIDTH_A> temp_data=dataA_in.read();
                            a_buf[pos2][pos1][pos0]=temp_data(63,0);
                            a_buf[pos2][pos1][pos0+64]=temp_data(127,64);
                            a_buf[pos2][pos1][pos0+128]=temp_data(191,128);
                            a_buf[pos2][pos1][pos0+192]=temp_data(255,192);
                        }
                    }
                }
            }
        }
    }
}
">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_B} == 512 ]
then
echo \
"void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*16+(b%PACKET_NUM_IN)*RIGHT_SIZE+w2%4+(w2/4)*16;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+4]=temp_data(255,128);
                                b_buf[pos2][pos1][pos0+8]=temp_data(383,256);
                                b_buf[pos2][pos1][pos0+12]=temp_data(511,384);
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${AXI_WIDTH_B} == 256 ]
then
echo \
"void loadB(axis_stream_B& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM_IN)*C][Y*Z][RIGHT_SIZE*PACKET_NUM_IN],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/B_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*8+(b%PACKET_NUM_IN)*RIGHT_SIZE+w2%4+(w2/4)*16;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM_IN)*C+c;
                                ap_uint<AXI_WIDTH_B> temp_data=dataB_in.read();
                                b_buf[pos2][pos1][pos0]=temp_data(127,0);
                                b_buf[pos2][pos1][pos0+4]=temp_data(255,128);
                            }
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

if [ ${AXI_WIDTH_C} == 512 ]
then
echo \
"void storeC(axis_stream_C& dataC_out, ap_uint<BUFF_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE_BUFF], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int aie_pack=c+a*C;
                                int pos0=n*512+w2;
                                int pos1=aie_pack%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3=aie_pack/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                temp_data(63,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(127,64)=c_buf[pos3][pos2][pos1][pos0+64];
                                temp_data(191,128)=c_buf[pos3][pos2][pos1][pos0+128];
                                temp_data(255,192)=c_buf[pos3][pos2][pos1][pos0+192];
                                temp_data(319,256)=c_buf[pos3][pos2][pos1][pos0+256];
                                temp_data(383,320)=c_buf[pos3][pos2][pos1][pos0+320];
                                temp_data(447,384)=c_buf[pos3][pos2][pos1][pos0+384];
                                temp_data(511,448)=c_buf[pos3][pos2][pos1][pos0+448];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for (int i = 0; i < OUT_SIZE_BUFF/8; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                        c_buf[a][x][j][i]=0; 
                        c_buf[a][x][j][i+64]=0;
                        c_buf[a][x][j][i+128]=0; 
                        c_buf[a][x][j][i+192]=0;
                        c_buf[a][x][j][i+256]=0; 
                        c_buf[a][x][j][i+320]=0;
                        c_buf[a][x][j][i+384]=0; 
                        c_buf[a][x][j][i+448]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${AXI_WIDTH_C} == 256 ]
then
echo \
"void storeC(axis_stream_C& dataC_out, ap_uint<BUFF_WIDTH> c_buf[A*C/PACKET_NUM_OUT][X*Z][PACKET_NUM_OUT][OUT_SIZE_BUFF], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int aie_pack=c+a*C;
                                int pos0=n*256+w2;
                                int pos1=aie_pack%PACKET_NUM_OUT;
                                int pos2=x+z*X;
                                int pos3=aie_pack/PACKET_NUM_OUT;
                                ap_uint<AXI_WIDTH_C> temp_data;
                                temp_data(63,0)=c_buf[pos3][pos2][pos1][pos0];
                                temp_data(127,64)=c_buf[pos3][pos2][pos1][pos0+64];
                                temp_data(191,128)=c_buf[pos3][pos2][pos1][pos0+128];
                                temp_data(255,192)=c_buf[pos3][pos2][pos1][pos0+192];
                                dataC_out.write(temp_data);
                            }
                        }
                    }
                }
            }
        }
        for(int x = 0; x < X*Z; x++){
            for(int j=0;j<PACKET_NUM_OUT;j++){
                for(int w2=0;w2<W2;w2++){
                    for (int n=0; n<H1/C_PER_TRA;n++){
                    #pragma HLS PIPELINE II = 1
                        int pos0=n*256+w2;
                        for(int a = 0; a < (A*C/PACKET_NUM_OUT); a++){
                            c_buf[a][x][j][pos0]=0; 
                            c_buf[a][x][j][pos0+64]=0;
                            c_buf[a][x][j][pos0+128]=0; 
                            c_buf[a][x][j][pos0+192]=0;
                        }
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi

fi