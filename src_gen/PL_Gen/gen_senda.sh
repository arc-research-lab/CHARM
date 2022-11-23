if [ "$#" -eq 3 ] 
then
    dir_name=$1;
    data_type=$2;
    NUM_TXA=$3;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_senda Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi


if [ ${data_type} == "fp32" ] || [ ${data_type} == "int32" ] || [ ${data_type} == "int16" ]
then
echo \
"
template<int NC>
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][LEFT_SIZE*PACKET_NUM_IN],">> ./${dir_name}/kernel/dma.cpp;

echo -n "           ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${NUM_TXA};i++));
do
echo -n "axis_stream& txA${i}, ">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"bool enable){">> ./${dir_name}/kernel/dma.cpp;

echo \
"
#pragma HLS inline off
    if(enable){
        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        axis_pkt tmp;
        data_t data;
        data_t da;
        int index_i=0;
        int index_j=0;
        for (int k = 0; k < X*PACKET_NUM_IN*Y*Z; k++) {    
            unsigned int ID=packet_id[k];
            
            int tile=tile_A[index_i];

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
            int position=ID*LEFT_SIZE;
    
            data=a_buf[tile][position];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            tmp.data  =  da;
            tmp.keep  = -1;
            tmp.last  = 0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXA};i++));
do
echo "            txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
            for (int i = 1; i < LEFT_SIZE; i++){ 
            #pragma HLS PIPELINE II = 1
                int posa=position+i;
    
                data=a_buf[tile][posa];
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

for ((i=0;i<${NUM_TXA};i++));
do
echo "                txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
            }
    
            da(31,0)=data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            tmp.data  =  da; 
            tmp.keep  = 0x000f;
            tmp.last  = 1;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXA};i++));
do
echo "            txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
        }
    }
    
}">> ./${dir_name}/kernel/dma.cpp;

elif [ ${data_type} == "int8" ]
then
echo \
"
template<int NC>
void sendA(ap_uint<BUFF_WIDTH> a_buf[X*Y][PACKET_NUM_IN][LEFT_SIZE_BUFF],">> ./${dir_name}/kernel/dma.cpp;

echo -n "           ">> ./${dir_name}/kernel/dma.cpp;
for ((i=0;i<${NUM_TXA};i++));
do
echo -n "axis_stream& txA${i}, ">> ./${dir_name}/kernel/dma.cpp;
done

echo \
"bool enable){">> ./${dir_name}/kernel/dma.cpp;

echo \
"
#pragma HLS inline off
    if(enable){
        ap_uint<32> data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        axis_pkt tmp;
        data_t data;
        data_t da;
        int index_i=0;
        int index_j=0;
        for (int k = 0; k < X*PACKET_NUM_IN*Y*Z; k++) {    
            unsigned int ID=packet_id[k];
            
            int tile=tile_A[index_i];

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
    
            data(63,0)=a_buf[tile][ID][0];
            data(127,64)=a_buf[tile][ID][1];
            data_temp[0][0]=data(31,0);
            data_temp[0][1]=data(63,32);
            data_temp[0][2]=data(95,64);
            data_temp[0][3]=data(127,96);
            da(31,0)   = header;
            da(63,32)  = data_temp[0][0];
            da(95,64)  = data_temp[0][1];
            da(127,96) = data_temp[0][2];
            tmp.data  =  da;
            tmp.keep  = -1;
            tmp.last  = 0;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXA};i++));
do
echo "            txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
            for (int i = 1; i < LEFT_SIZE_BUFF/2; i++){ 
            #pragma HLS PIPELINE II = 1
    
                data(63,0)=a_buf[tile][ID][i*2];
                data(127,64)=a_buf[tile][ID][i*2+1];
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

for ((i=0;i<${NUM_TXA};i++));
do
echo "                txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
            }
    
            da(31,0)=data_temp[1][3];
            da(63,32)  = 0;
            da(95,64)  = 0;
            da(127,96) = 0;
            tmp.data  =  da; 
            tmp.keep  = 0x000f;
            tmp.last  = 1;">> ./${dir_name}/kernel/dma.cpp;

for ((i=0;i<${NUM_TXA};i++));
do
echo "            txA${i}.write(tmp);">> ./${dir_name}/kernel/dma.cpp;
done

echo \
" 
        }
    }
    
}">> ./${dir_name}/kernel/dma.cpp;
fi