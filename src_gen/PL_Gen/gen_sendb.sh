if [ "$#" -eq 7 ] 
then
    dir_name=$1;
    data_type=$2;
    NUM_TXB=$3;
    AXI_WIDTH_B=$4;
    mm_k=$5;
    NUM_PACK_IN=$6;
    pipe_length=$7;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_sendb Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi

if [ ${data_type} == "int16" ] && [ ${AXI_WIDTH_B} == 512 ] && [ $((${mm_k}%32)) != 0 ]
then
echo \
"
template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE*PACKET_NUM_IN],">> ./${dir_name}/kernel/dma.cpp;

echo -n "           ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${NUM_TXB};i++));
do
echo -n "axis_stream& txB${i}, ">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"bool enable){">> ./${dir_name}/kernel/dma.cpp;

echo \
"
#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        data_t data;
        data_t da;
    
        int index_i=0;
        int index_j=0;
        for (int k = 0; k < PACKET_NUM_IN*Y*X*Z; k++) {">> ./${dir_name}/kernel/dma.cpp;

if [ ${pipe_length} -gt ${NUM_PACK_IN} ]    
then
echo \
"
            unsigned int ID=packet_id[index_j];

            int tile=tile_B[index_i];

            if(( index_i==0 || index_j==(PACKET_NUM_IN-1)) && (index_i<PIPELINE_LEN-PACKET_NUM_IN)){
                index_i=index_j+1+index_i;
                index_j=0;
            }
            else if ((index_j==(PACKET_NUM_IN-1))&& (index_i>=PIPELINE_LEN-PACKET_NUM_IN)){
                index_j=PACKET_NUM_IN+1-PIPELINE_LEN+index_i;
                index_i=PIPELINE_LEN-1;
            }
            else{
                index_i=index_i-1;
                index_j=index_j+1;
            }">> ./${dir_name}/kernel/dma.cpp;
else
echo \
"
            unsigned int ID=packet_id[k];
            int tile=tile_B[k];">> ./${dir_name}/kernel/dma.cpp;
fi

echo \
"
            ap_uint<32> header=generateHeader(PKTTYPE,ID);
            int position=ID*(W1/NUM_PER_TRA);
    
            data=b_buf[tile][position];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            
            tmp.data  =  da;
            tmp.keep  =  -1;
            tmp.last  =  0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done    

echo \
"
            for (int i = 1; i < RIGHT_SIZE; i++){
            #pragma HLS PIPELINE II = 1
                int posb=position+(i/(W1/NUM_PER_TRA))*(W1*PACKET_NUM_IN/NUM_PER_TRA)+(i%(W1/NUM_PER_TRA));
    
                data=b_buf[tile][posb];
                data_temp[i%2][0]=data(31,0);
                data_temp[i%2][1]=data(63,32);
                data_temp[i%2][2]=data(95,64);
                data_temp[i%2][3]=data(127,96);
                da(31,0)   = data_temp[(i+1)%2][3];
                da(63,32)  = data_temp[i%2][0];
                da(95,64)  = data_temp[i%2][1];
                da(127,96) = data_temp[i%2][2];
                
                tmp.data   = da;
                tmp.keep   = -1;
                tmp.last   = 0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "                txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"
            }
    
            da(31,0)   = data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            
    
            tmp.data =  da;
            tmp.keep  = 0x000f;
            tmp.last  = 1;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"          
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${data_type} == "int8" ] && [ ${AXI_WIDTH_B} == 512 ]
then
echo \
"
template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][PACKET_NUM_IN][RIGHT_SIZE],">> ./${dir_name}/kernel/dma.cpp;

echo -n "           ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${NUM_TXB};i++));
do
echo -n "axis_stream& txB${i}, ">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"bool enable){">> ./${dir_name}/kernel/dma.cpp;

echo \
"
#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        data_t data;
        data_t da;
        int index_i=0;
        int index_j=0;
        for (int k = 0; k < PACKET_NUM_IN*Y*X*Z; k++) {">> ./${dir_name}/kernel/dma.cpp;

if [ ${pipe_length} -gt ${NUM_PACK_IN} ]    
then
echo \
"
            unsigned int ID=packet_id[index_j];
            int tile=tile_B[index_i];

            if(( index_i==0 || index_j==(PACKET_NUM_IN-1)) && (index_i<PIPELINE_LEN-PACKET_NUM_IN)){
                index_i=index_j+1+index_i;
                index_j=0;
            }
            else if ((index_j==(PACKET_NUM_IN-1))&& (index_i>=PIPELINE_LEN-PACKET_NUM_IN)){
                index_j=PACKET_NUM_IN+1-PIPELINE_LEN+index_i;
                index_i=PIPELINE_LEN-1;
            }
            else{
                index_i=index_i-1;
                index_j=index_j+1;
            }">> ./${dir_name}/kernel/dma.cpp;
else
echo \
"
            unsigned int ID=packet_id[k];
            int tile=tile_B[k];">> ./${dir_name}/kernel/dma.cpp;
fi
echo \
"
            ap_uint<32> header=generateHeader(PKTTYPE,ID);
    
            data=b_buf[tile][ID][0];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            
            tmp.data  =  da;
            tmp.keep  =  -1;
            tmp.last  =  0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done    

echo \
"
            for (int i = 1; i < RIGHT_SIZE; i++){
            #pragma HLS PIPELINE II = 1
    
                data=b_buf[tile][ID][i];
                data_temp[i%2][0]=data(31,0);
                data_temp[i%2][1]=data(63,32);
                data_temp[i%2][2]=data(95,64);
                data_temp[i%2][3]=data(127,96);
                da(31,0)   = data_temp[(i+1)%2][3];
                da(63,32)  = data_temp[i%2][0];
                da(95,64)  = data_temp[i%2][1];
                da(127,96) = data_temp[i%2][2];
                
                tmp.data   = da;
                tmp.keep   = -1;
                tmp.last   = 0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "                txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"
            }
    
            da(31,0)   = data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            
    
            tmp.data =  da;
            tmp.keep  = 0x000f;
            tmp.last  = 1;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"          
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

else
echo \
"
template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE*PACKET_NUM_IN],">> ./${dir_name}/kernel/dma.cpp;

echo -n "           ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${NUM_TXB};i++));
do
echo -n "axis_stream& txB${i}, ">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"bool enable){">> ./${dir_name}/kernel/dma.cpp;

echo \
"
#pragma HLS inline off
    if(enable){
        axis_pkt tmp;
        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        data_t data;
        data_t da;

        int index_i=0;
        int index_j=0;
    
        for (int k = 0; k < PACKET_NUM_IN*Y*X*Z; k++) {
            unsigned int ID=packet_id[index_j];

            int tile=tile_B[index_i];

            if(( index_i==0 || index_j==(PACKET_NUM_IN-1)) && (index_i<PIPELINE_LEN-PACKET_NUM_IN)){
                index_i=index_j+1+index_i;
                index_j=0;
            }
            else if ((index_j==(PACKET_NUM_IN-1))&& (index_i>=PIPELINE_LEN-PACKET_NUM_IN)){
                index_j=PACKET_NUM_IN+1-PIPELINE_LEN+index_i;
                index_i=PIPELINE_LEN-1;
            }
            else{
                index_i=index_i-1;
                index_j=index_j+1;
            }
    
            ap_uint<32> header=generateHeader(PKTTYPE,ID);
            int position=ID*RIGHT_SIZE;
    
            data=b_buf[tile][position];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            
            tmp.data  =  da;
            tmp.keep  =  -1;
            tmp.last  =  0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done    

echo \
"
            for (int i = 1; i < RIGHT_SIZE; i++){
            #pragma HLS PIPELINE II = 1
                int posb=position+i;   
    
                data=b_buf[tile][posb];
                data_temp[i%2][0]=data(31,0);
                data_temp[i%2][1]=data(63,32);
                data_temp[i%2][2]=data(95,64);
                data_temp[i%2][3]=data(127,96);
                da(31,0)   = data_temp[(i+1)%2][3];
                da(63,32)  = data_temp[i%2][0];
                da(95,64)  = data_temp[i%2][1];
                da(127,96) = data_temp[i%2][2];
                
                tmp.data   = da;
                tmp.keep   = -1;
                tmp.last   = 0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "                txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"
            }
    
            da(31,0)   = data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            
    
            tmp.data =  da;
            tmp.keep  = 0x000f;
            tmp.last  = 1;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXB};i++));
do
echo "            txB${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"          
        }
    }
}">> ./${dir_name}/kernel/dma.cpp;

fi