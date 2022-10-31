if [ "$#" -eq 2 ] 
then
  	input=$1;
    dir_name=$2;
else
    echo ""
    echo "******************************************"
	echo "Please Specify the PATH of config file and output file";
    echo "An example is:"
    echo "./gen_dma.sh ../../config_files/input.cfg ."
    echo "******************************************"
    echo ""
    exit
fi

if [ -f ./${dir_name}/kernel/dma.cpp ]
then
    echo ""
    echo "******************************************"
    echo "File ./${dir_name}/kernel/dma.cpp exists." 
    echo "******************************************"
    echo ""
    exit;
fi

for ((n=1;n<=21;n++));
do
	read -r line
	if (( ${n} == 2 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		data_type="${Value[0]}"; 
	elif (( ${n} == 4 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		kernel_type="${Value[0]}";
    elif (( ${n} == 9 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		NUM_PACK="${Value[0]}";
    elif (( ${n} == 10 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		A="${Value[0]}";
    elif (( ${n} == 11 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		B="${Value[0]}";
 	elif (( ${n} == 12 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C="${Value[0]}";
    elif (( ${n} == 13 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		R_BRO="${Value[0]}";
    elif (( ${n} == 14 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		C_BRO="${Value[0]}";
    elif (( ${n} == 16 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		X="${Value[0]}";
    elif (( ${n} == 17 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Y="${Value[0]}";
    elif (( ${n} == 18 ))
	then
		IFS=':' read -ra Key <<< "$line";
		value_temp="${Key[1]}"; 
		unset IFS
		IFS=';' read -ra Value <<< "$value_temp";
		Z="${Value[0]}";
    elif (( ${n} == 19 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        l_buff="${Value[0]}";
    elif (( ${n} == 20 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        r_buff="${Value[0]}";
    elif (( ${n} == 21 ))
    then
        IFS=':' read -ra Key <<< "$line";
        value_temp="${Key[1]}"; 
        unset IFS
        IFS=';' read -ra Value <<< "$value_temp";
        o_buff="${Value[0]}";
 	fi
done < "$input"

let NUM_TXA=${C}/${R_BRO};
let NUM_TXB=${A}/${C_BRO};
let port_row_in=${A}*${B}*${C}/${NUM_PACK}/${R_BRO};
let port_col_in=${C}*${B}*${A}/${NUM_PACK}/${C_BRO};
let port_out=${A}*${C}/${NUM_PACK};
let pipe_length=${X}*${Y}*${Z};
let array_size=${pipe_length}*${NUM_PACK};

declare -A arrA_tile;
declare -A arrB_tile;

for ((i=0;i<${NUM_PACK};i++));
do  
    for ((l=0;l<${pipe_length}+${NUM_PACK}-1;l++));
    do  
        arrA_tile[${i},${l}]=-1;
    done
    for ((k=0;k<${Z};k++));
    do  
        for ((j=0;j<${X}*${Y};j++));
        do  
            let pos=${i}+${j}+${k}*${X}*${Y};
            arrA_tile[${i},${pos}]=${j};
        done
    done
done

for ((i=0;i<${NUM_PACK};i++));
do  
    for ((l=0;l<${pipe_length}+${NUM_PACK}-1;l++));
    do  
        arrB_tile[${i},${l}]=-1;
    done
    for ((z=0;z<${Z};z++));
    do 
        for ((x=0;x<${X};x++));
        do  
            for ((y=0;y<${Y};y++));
            do  
                let pos=${i}+${y}+${x}*${Y}+${z}*${X}*${Y};
                let data=${y}+${z}*${Y};
                arrB_tile[${i},${pos}]=${data};
            done
        done
    done
done


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

mkdir -p ${dir_name}/kernel
if [ ${B} == 4 ] 
then
echo \
"
#include <stdint.h>
#include \"packet_sender.hpp\"
">> ./${dir_name}/kernel/dma.cpp;

echo -n "static const unsigned int tile_A[${array_size}]={">> ./${dir_name}/kernel/dma.cpp;

for ((l=0;l<${pipe_length}+${NUM_PACK}-1;l++));
do  
    for ((i=0;i<${NUM_PACK};i++));
    do  
        let temp1=${NUM_PACK}-1;
        let temp2=${pipe_length}+${NUM_PACK}-2;
        if [ ${i} == ${temp1} ] && [ ${l} == ${temp2} ]
        then
            echo "${arrA_tile[${i},${l}]}};">> ./${dir_name}/kernel/dma.cpp;
        elif [ ${arrA_tile[${i},${l}]} != -1 ] 
        then
            echo -n "${arrA_tile[${i},${l}]},">> ./${dir_name}/kernel/dma.cpp;
        fi
    done
done

echo -n "static const unsigned int tile_B[${array_size}]={">> ./${dir_name}/kernel/dma.cpp;
for ((l=0;l<${pipe_length}+${NUM_PACK}-1;l++));
do  
    for ((i=0;i<${NUM_PACK};i++));
    do  
        let temp1=${NUM_PACK}-1;
        let temp2=${pipe_length}+${NUM_PACK}-2;
        if [ ${i} == ${temp1} ] && [ ${l} == ${temp2} ]
        then
            echo "${arrB_tile[${i},${l}]}};">> ./${dir_name}/kernel/dma.cpp;
        elif [ ${arrB_tile[${i},${l}]} != -1 ] 
        then
            echo -n "${arrB_tile[${i},${l}]},">> ./${dir_name}/kernel/dma.cpp;
        fi
    done
done

echo -n "static const unsigned int packet_id[${array_size}]={">> ./${dir_name}/kernel/dma.cpp;
for ((l=0;l<${pipe_length}+${NUM_PACK}-1;l++));
do  
    for ((i=0;i<${NUM_PACK};i++));
    do  
        let temp1=${NUM_PACK}-1;
        let temp2=${pipe_length}+${NUM_PACK}-2;
        if [ ${i} == ${temp1} ] && [ ${l} == ${temp2} ]
        then
            echo -n "${i}};">> ./${dir_name}/kernel/dma.cpp;
        elif [ ${arrB_tile[${i},${l}]} != -1 ] 
        then
            echo -n "${i},">> ./${dir_name}/kernel/dma.cpp;
        fi
    done
done

echo \
"

ap_uint<32> generateHeader(unsigned int pktType, unsigned int ID){
#pragma HLS inline
    ap_uint<32> header=0;
    header(4,0)=ID;
    header(11,5)=0;
    header(14,12)=pktType;
    header[15]=0;
    header(20,16)=-1;//source row
    header(27,21)=-1;//source column
    header(30,28)=0;
    header[31]=header(30,0).xor_reduce()?(ap_uint<1>)0:(ap_uint<1>)1;
    return header;
}

unsigned int getPacketId(ap_uint<32> header){
#pragma HLS inline
    ap_uint<32> ID=0;
    ID(4,0)=header(4,0);
    return ID;
}

void address_A_ddr(axis_stream_32& addrA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    for(ap_uint<32> tx=0;tx<TX;tx++){
        for(ap_uint<32> tz=0;tz<TZ;tz++){
            for(ap_uint<32> ty=0;ty<TY;ty++){
                ap_uint<32> pos;
                for(ap_uint<32> j=0;j<K;j++){
                    for(ap_uint<32> i=0;i<M/A_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(M/A_PER_TRA)*TX+ty*K*(M/A_PER_TRA)*TX+tx*(M/A_PER_TRA);
                        addrA_out.write(pos);
                    }
                }
            }
        }
    }
}


void loadA_ddr(ap_uint<AXI_WIDTH_512>* ina, axis_stream_32& addrA_in,axis_stream_512& dataA_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    ap_uint<32> bound=boundA*TX*TY*TZ;
    for(ap_uint<32> i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<32> addr = addrA_in.read();
        temp_data=ina[addr];
        dataA_out.write(temp_data);        
    }
}

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

void address_B_ddr(axis_stream_32& addrB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off    
    for(ap_uint<32> tx=0;tx<TX;tx++){
        for(ap_uint<32> tz=0;tz<TZ;tz++){
            for(ap_uint<32> ty=0;ty<TY;ty++){
                ap_uint<32> pos;
                for(ap_uint<32> j=0;j<N;j++){
                    for(ap_uint<32> i=0;i<K/A_PER_TRA;i++){
                    #pragma HLS PIPELINE II = 1
                        pos=i+j*(K/A_PER_TRA)*TY+ty*(K/A_PER_TRA)+tz*N*(K/A_PER_TRA)*TY;
                        addrB_out.write(pos);
                    }
                }
            }
        }
    }
}


void loadB_ddr(ap_uint<AXI_WIDTH_512>* inb, axis_stream_32& addrB_in,axis_stream_512& dataB_out,const int TX,const int TY,const int TZ) {
#pragma HLS inline off
    ap_uint<512> temp_data;
    ap_uint<32> bound=boundB*TX*TY*TZ;
    for(ap_uint<32> i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<32> addr = addrB_in.read();
        temp_data=inb[addr];
        dataB_out.write(temp_data);
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


void address_C_ddr(axis_stream_32& addrC_out,const int TX,const int TZ) {
#pragma HLS inline off
    for(ap_uint<32> tx=0;tx<TX;tx++){
        for(ap_uint<32> tz=0;tz<TZ;tz++){
            ap_uint<32> pos;
            for(ap_uint<32> j=0;j<N;j++){
                for(ap_uint<32> i=0;i<M/C_PER_TRA;i++){
                #pragma HLS PIPELINE II = 1
                    pos=i+j*(M/C_PER_TRA)*TX+tx*(M/C_PER_TRA)+tz*N*(M/C_PER_TRA)*TX;
                    addrC_out.write(pos);
                }
            }
        }
    }
}

void storeC_ddr(ap_uint<AXI_WIDTH_256>* outc, axis_stream_32& addrC_in,axis_stream_256& dataC_in,const int TX,const int TZ) { 
#pragma HLS inline off
    ap_uint<32> bound=boundC*TX*TZ;
    for(ap_uint<32> i=0;i<bound;i++){
    #pragma HLS PIPELINE II = 1
        ap_uint<256> temp_data=dataC_in.read();
        ap_uint<32> addr = addrC_in.read();
        outc[addr]=temp_data;
            
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

echo \
"
template<int NC>
void sendA(ap_uint<PLIO_WIDTH> a_buf[X*Y][LEFT_SIZE*PACKET_NUM],">> ./${dir_name}/kernel/dma.cpp;

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
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        axis_pkt tmp;
        data_t data;
        data_t da;
        for (int k = 0; k < X*PACKET_NUM*Y*Z; k++) {    
            unsigned int ID=packet_id[k];
            int tile=tile_A[k];

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
                int posa=ID*LEFT_SIZE+i;
    
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

echo \
"
template<int NC>
void sendB(ap_uint<PLIO_WIDTH> b_buf[Y*Z][RIGHT_SIZE*PACKET_NUM],">> ./${dir_name}/kernel/dma.cpp;

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
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        data_t data;
        data_t da;
    
    
        for (int k = 0; k < PACKET_NUM*Y*X*Z; k++) {
            unsigned int ID=packet_id[k];
            int tile=tile_B[k];
    
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
                int posb=ID*RIGHT_SIZE+i;   
    
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

if [ ${data_type} == "fp32" ]
then
echo \
"
template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM][OUT_SIZE],axis_stream& rxC, bool enable){   
#pragma HLS inline off
    if (enable){
        
        axis_pkt tmp; 
        int cnt[4];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
        float data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0

        fp_int intfp0;
        fp_int intfp1;
        fp_int intfp2;
        fp_int intfp3;
        fp_int ifp_temp0;
        fp_int ifp_temp1;
        fp_int ifp_temp2;
        fp_int ifp_temp3;   
    for(int i=0;i<PACKET_NUM;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM;j++){
                        ap_uint<32> header;
                        tmp=rxC.read();
                        
                        header=tmp.data(31,0);
                        
                        ifp_temp1.uintval=tmp.data(63,32);
                        ifp_temp2.uintval=tmp.data(95,64);
                        ifp_temp3.uintval=tmp.data(127,96);
            
            
                        data_temp[0][1]=ifp_temp1.data_cbuff;
                        data_temp[0][2]=ifp_temp2.data_cbuff;
                        data_temp[0][3]=ifp_temp3.data_cbuff;
                        
                        unsigned int ID=getPacketId(header);
    
                        unsigned int tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;
    
    
    
                        for(int i=0;i<OUT_SIZE;i++){
                        #pragma HLS PIPELINE II = 1
                            tmp=rxC.read();
                            

                            ifp_temp0.uintval=tmp.data(31,0);
                            ifp_temp1.uintval=tmp.data(63,32);
                            ifp_temp2.uintval=tmp.data(95,64);
                            ifp_temp3.uintval=tmp.data(127,96);

                            data_temp[(i+1)%2][0]=ifp_temp0.data_cbuff;
                            data_temp[(i+1)%2][1]=ifp_temp1.data_cbuff;
                            data_temp[(i+1)%2][2]=ifp_temp2.data_cbuff;
                            data_temp[(i+1)%2][3]=ifp_temp3.data_cbuff;
                            
                            intfp0.uintval=c_buf[tile_x][ID][i](31,0);
                            intfp1.uintval=c_buf[tile_x][ID][i](63,32);
                            intfp2.uintval=c_buf[tile_x][ID][i](95,64);
                            intfp3.uintval=c_buf[tile_x][ID][i](127,96);

                            intfp0.data_cbuff = data_temp[i%2][1] + intfp0.data_cbuff ;
                            intfp1.data_cbuff = data_temp[i%2][2] + intfp1.data_cbuff ;
                            intfp2.data_cbuff = data_temp[i%2][3] + intfp2.data_cbuff ;
                            intfp3.data_cbuff = data_temp[(i+1)%2][0] + intfp3.data_cbuff;

                            c_buf[tile_x][ID][i](31,0)   =  intfp0.uintval;  
                            c_buf[tile_x][ID][i](63,32)  =  intfp1.uintval;
                            c_buf[tile_x][ID][i](95,64)  =  intfp2.uintval;
                            c_buf[tile_x][ID][i](127,96) =  intfp3.uintval;
                            
                        }
                    }
                }
            } 
        }
    }
    
}">> ./${dir_name}/kernel/dma.cpp;
elif [ ${data_type} == "int32" ]
then
echo \
"
template<int NC>
void reshapeC(ap_uint<PLIO_WIDTH> c_buf[X*Z][PACKET_NUM][OUT_SIZE],axis_stream& rxC, bool enable){   
#pragma HLS inline off
    if (enable){
        
        axis_pkt tmp; 
        int cnt[4];
        #pragma HLS ARRAY_PARTITION variable=cnt complete dim=0
        data_t1 data_temp[2][4];
        #pragma HLS ARRAY_PARTITION variable=data_temp complete dim=0
        for(int i=0;i<PACKET_NUM;i++){
        #pragma HLS unroll
            cnt[i]=0;
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for(int j=0;j<PACKET_NUM;j++){
                    for (int i = 0; i < OUT_SIZE; i++){
                    #pragma HLS PIPELINE II = 1
                        c_buf[x+z*X][j][i]=0; 
                    }
                }
            }
        }
        for(int z = 0; z < Z; z++){
            for(int x = 0; x < X; x++){
                for (int n = 0; n < Y; n++){
                    for(int j=0;j<PACKET_NUM;j++){
                        ap_uint<32> header;
                        tmp=rxC.read();
                        
                        header=tmp.data(31,0);
                        data_temp[0][1]=tmp.data(63,32);
                        data_temp[0][2]=tmp.data(95,64);
                        data_temp[0][3]=tmp.data(127,96);
                        
                        unsigned int ID=getPacketId(header);
    
                        unsigned int tile_x=cnt[ID]/Y;
                        cnt[ID]=cnt[ID]+1;
    
    
    
                        for(int i=0;i<OUT_SIZE;i++){
                        #pragma HLS PIPELINE II = 1
                            tmp=rxC.read();
            
                            data_temp[(i+1)%2][0]=tmp.data(31,0);
                            data_temp[(i+1)%2][1]=tmp.data(63,32);
                            data_temp[(i+1)%2][2]=tmp.data(95,64);
                            data_temp[(i+1)%2][3]=tmp.data(127,96);
        
                            c_buf[tile_x][ID][i](31,0)  = data_temp[i%2][1] + c_buf[tile_x][ID][i](31,0)  ;
                            c_buf[tile_x][ID][i](63,32) = data_temp[i%2][2] + c_buf[tile_x][ID][i](63,32) ;
                            c_buf[tile_x][ID][i](95,64) = data_temp[i%2][3] + c_buf[tile_x][ID][i](95,64) ;
                            c_buf[tile_x][ID][i](127,96)= data_temp[(i+1)%2][0] + c_buf[tile_x][ID][i](127,96);
                            
                        }
                    }
                }
            } 
        }
    }
    
}
"
fi

echo \
"
void compute(axis_stream_512& dataA_out, axis_stream_512& dataB_out, axis_stream_256& dataC_in,const int TX, const int TY, const int TZ,">> ./${dir_name}/kernel/dma.cpp;

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
    #pragma HLS ARRAY_PARTITION variable=buff0_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_A complete dim=1

    ap_uint<PLIO_WIDTH> buff1_A[A*(B/PACKET_NUM)][X*Y][LEFT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_A type=RAM_1P impl=${L_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_A cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_A complete dim=1

    ap_uint<PLIO_WIDTH> buff0_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff0_B type=RAM_1P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff0_B complete dim=1

    ap_uint<PLIO_WIDTH> buff1_B[(B/PACKET_NUM)*C][Y*Z][RIGHT_SIZE*PACKET_NUM];
    #pragma HLS bind_storage variable=buff1_B type=RAM_1P impl=${R_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_B cyclic factor=4 dim=3
    #pragma HLS ARRAY_PARTITION variable=buff1_B complete dim=1

    ap_uint<PLIO_WIDTH> buff0_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff0_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff0_C complete dim=1

    ap_uint<PLIO_WIDTH> buff1_C[A*C/PACKET_NUM][X*Z][PACKET_NUM][OUT_SIZE];
    #pragma HLS bind_storage variable=buff1_C type=RAM_T2P impl=${O_buffer}
    #pragma HLS ARRAY_PARTITION variable=buff1_C complete dim=1

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
    }

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


###################################
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

fi