if [ "$#" -eq 2 ] 
then
    dir_name=$1;
    data_type=$2;
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
echo \
"
void loadA(axis_stream_512& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(ap_uint<32> y=0;y<Y;y++){
            for(ap_uint<32> k=0;k<W1*B;k++){
                for(ap_uint<32> x=0;x<X;x++){
                    for(ap_uint<32> a=0;a<A;a++){
                        for(ap_uint<32> i=0;i<(H1/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int pos0=i*4+(k%(W1*PACKET_NUM))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=a+A*(k/(W1*PACKET_NUM));
                            ap_uint<512> temp_data=dataA_in.read();
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
}

void loadB(axis_stream_512& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for(int b=0;b<B;b++){
                            for (int m=0;m<(W1/A_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1/NUM_PER_TRA)+(b%PACKET_NUM)*RIGHT_SIZE;
                                int pos1=z*Y+y;
                                int pos2=(b/PACKET_NUM)*C+c;
                                ap_uint<512> temp_data=dataB_in.read();
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

void storeC(axis_stream_256& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=n*2+w2*(H1/NUM_PER_TRA);
                                int pos1=c%PACKET_NUM;
                                int pos2=x+z*X;
                                int pos3=a*(C/PACKET_NUM)+(c/PACKET_NUM);
                                ap_uint<AXI_WIDTH_256> temp_data;
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
            for(int j=0;j<PACKET_NUM;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${data_type} == "int16" ]
then
echo \
"
void loadA(axis_stream_512& dataA_in, ap_uint<PLIO_WIDTH> a_buf[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(ap_uint<32> y=0;y<Y;y++){
            for(ap_uint<32> k=0;k<W1*B;k++){
                for(ap_uint<32> x=0;x<X;x++){
                    for(ap_uint<32> a=0;a<A/2;a++){
                        for(ap_uint<32> i=0;i<(H1*2/A_PER_TRA);i++){
                        #pragma HLS PIPELINE II = 1
                            int temp= (k%(W1*PACKET_NUM))*(H1/NUM_PER_TRA);
                            int pos1=x*Y+y;
                            int pos2=2*a+A*(k/(W1*PACKET_NUM));
                            ap_uint<512> temp_data=dataA_in.read();
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
}

void loadB(axis_stream_512& dataB_in, ap_uint<PLIO_WIDTH> b_buf[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM],bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int y=0;y<Y;y++){
                        for (int b=0;b<(B/PACKET_NUM);b++){
                            for (int m=0;m<(W1*PACKET_NUM/A_PER_TRA);m++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=m*4+w2*(W1*PACKET_NUM/NUM_PER_TRA);
                                int pos1=z*Y+y;
                                int pos2=b*C+c;
                                ap_uint<512> temp_data=dataB_in.read();
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

void storeC(axis_stream_256& dataC_out, ap_uint<PLIO_WIDTH> c_buf[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE], bool enable){
#pragma HLS inline off
    if(enable){
        for(int z=0;z<Z;z++){
            for(int c=0;c<C;c++){
                for(int w2=0;w2<W2;w2++){
                    for(int x=0;x<X;x++){
                        for (int a=0;a<A;a++){
                            for (int n=0; n<H1/C_PER_TRA;n++){
                            #pragma HLS PIPELINE II = 1
                                int pos0=n*2+w2*(H1/NUM_PER_TRA);
                                int pos1=c%PACKET_NUM;
                                int pos2=x+z*X;
                                int pos3=a*(C/PACKET_NUM)+(c/PACKET_NUM);
                                ap_uint<AXI_WIDTH_256> temp_data;
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
            for(int j=0;j<PACKET_NUM;j++){
                for (int i = 0; i < OUT_SIZE/2; i++){
                #pragma HLS PIPELINE II = 1
                    for(int a = 0; a < (A*C/PACKET_NUM); a++){
                        c_buf[a][x][j][i*2+0]=0; 
                        c_buf[a][x][j][i*2+1]=0;
                    }
                }
            }
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;
fi