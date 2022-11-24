if [ "$#" -eq 7 ] 
then
    dir_name=$1;
    NUM_PACK_IN=$2;
    pipe_length=$3;
    array_size=$4;
    X=$5;
    Y=$6;
    Z=$7;
else
    echo ""
    echo "******************************************"
	echo ""
    echo "PL_Gen/gen_header Missing Arguments"
    echo ""
    echo "******************************************"
    echo ""
    exit
fi



declare -A arrA_tile;
declare -A arrB_tile;

if [ ${pipe_length} -gt ${NUM_PACK_IN} ]
then
for ((i=0;i<${Z};i++));
do  
    for ((j=0;j<${X}*${Y};j++));
    do  
        let pos=${j}+${i}*${X}*${Y};
        arrA_tile[${pos}]=${j};
    done
done

for ((k=0;k<${Z};k++));
do  
    for ((i=0;i<${X};i++));
    do  
        for ((j=0;j<${Y};j++));
        do  
            let pos=${j}+${i}*${Y}+${k}*${X}*${Y};
            let data=${j}+${k}*${Y};
            arrB_tile[${pos}]=${data};
        done
    done
done

echo -n "static const unsigned int tile_A[${pipe_length}]={">> ./${dir_name}/kernel/dma.cpp;

for ((l=0;l<${pipe_length};l++));
do  
    if [ ${l} == $((${pipe_length}-1)) ]
    then
        echo "${arrA_tile[${l}]}};">> ./${dir_name}/kernel/dma.cpp;
    else
        echo -n "${arrA_tile[${l}]},">> ./${dir_name}/kernel/dma.cpp;
    fi
done

echo -n "static const unsigned int tile_B[${pipe_length}]={">> ./${dir_name}/kernel/dma.cpp;
for ((l=0;l<${pipe_length};l++));
do  
    if [ ${l} == $((${pipe_length}-1)) ]
    then
        echo "${arrB_tile[${l}]}};">> ./${dir_name}/kernel/dma.cpp;
    else
        echo -n "${arrB_tile[${l}]},">> ./${dir_name}/kernel/dma.cpp;
    fi
done

echo -n "static const unsigned int packet_id[${NUM_PACK_IN}]={">> ./${dir_name}/kernel/dma.cpp;
 
for ((i=0;i<${NUM_PACK_IN};i++));
do  
    let temp1=${NUM_PACK_IN}-1;
    if [ ${i} == ${temp1} ]
    then
        echo -n "${i}};">> ./${dir_name}/kernel/dma.cpp;
    else
        echo -n "${i},">> ./${dir_name}/kernel/dma.cpp;
    fi
done


else
for ((i=0;i<${NUM_PACK_IN};i++));
do  
    for ((l=0;l<${pipe_length}+${NUM_PACK_IN}-1;l++));
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

for ((i=0;i<${NUM_PACK_IN};i++));
do  
    for ((l=0;l<${pipe_length}+${NUM_PACK_IN}-1;l++));
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

echo -n "static const unsigned int tile_A[${array_size}]={">> ./${dir_name}/kernel/dma.cpp;

for ((l=0;l<${pipe_length}+${NUM_PACK_IN}-1;l++));
do  
    for ((i=0;i<${NUM_PACK_IN};i++));
    do  
        let temp1=${NUM_PACK_IN}-1;
        let temp2=${pipe_length}+${NUM_PACK_IN}-2;
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
for ((l=0;l<${pipe_length}+${NUM_PACK_IN}-1;l++));
do  
    for ((i=0;i<${NUM_PACK_IN};i++));
    do  
        let temp1=${NUM_PACK_IN}-1;
        let temp2=${pipe_length}+${NUM_PACK_IN}-2;
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
for ((l=0;l<${pipe_length}+${NUM_PACK_IN}-1;l++));
do  
    for ((i=0;i<${NUM_PACK_IN};i++));
    do  
        let temp1=${NUM_PACK_IN}-1;
        let temp2=${pipe_length}+${NUM_PACK_IN}-2;
        if [ ${i} == ${temp1} ] && [ ${l} == ${temp2} ]
        then
            echo -n "${i}};">> ./${dir_name}/kernel/dma.cpp;
        elif [ ${arrB_tile[${i},${l}]} != -1 ] 
        then
            echo -n "${i},">> ./${dir_name}/kernel/dma.cpp;
        fi
    done
done
fi