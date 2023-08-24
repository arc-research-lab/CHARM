task=$1;
if [ $task == "bert" ] || [ $task == "BERT" ]
then
    # M * K * N * #layer * 2
    let OPs=$(( 3072*1024*1024*4*2 + 3072*1024*4096*2*2 + 512*512*64*192*2 ));
    for ((n=1;n<=6;n++));
    do
    	read -r line
        if (( ${n} == 1 ))
    	then
    		Layer0_2=$line; 
        elif (( ${n} == 2 ))
    	then
    		Layer3=$line;
    	elif (( ${n} == 3 ))
    	then
    		Layer4=$line; 
    	elif (( ${n} == 4 ))
    	then
    		Layer5=$line; 
    	elif (( ${n} == 5 ))
    	then
    		Layer6=$line;
        elif (( ${n} == 6 ))
    	then
    		Layer7=$line;
     	fi
    done < "Figure7_8_temp_$task.log"
    Time=`echo ${Layer0_2} + ${Layer3} + ${Layer4} + ${Layer5} + ${Layer6}*96 + ${Layer7}*96 | bc`;
	# 100 Iteration, 8 Accs
    Throughput=`echo "scale=2; ${OPs}*100*8/${Time}/1000000000" | bc`;
    echo "Bert on 8 Duplicate Accs: ${Throughput} GOPs" >>Figure7_8_duplicate_${task};
elif [ $task == "vit" ] || [ $task == "VIT" ] || [ $task == "ViT" ]
then
    # M * K * N * #layer * 2
    let OPs=$(( 3072*3024*1024*1*2 + 3072*1024*1024*1*2 + 3072*1024*4096*2*2 + 3072*1024*3072*1*2 + 64*64*64*1536*2));
    for ((n=1;n<=6;n++));
    do
    	read -r line
        if (( ${n} == 1 ))
    	then
    		Layer0=$line; 
    	elif (( ${n} == 2 ))
    	then
    		Layer1=$line; 
    	elif (( ${n} == 3 ))
    	then
    		Layer2=$line; 
    	elif (( ${n} == 4 ))
    	then
    		Layer3=$line;
        elif (( ${n} == 5 ))
    	then
    		Layer4=$line;
        elif (( ${n} == 6 ))
    	then
    		Layer5=$line;
     	fi
    done < "Figure7_8_temp_$task.log"
    Time=`echo "scale=2; ${Layer0} + ${Layer1} + ${Layer2} + ${Layer3} + ${Layer4} + ${Layer5}*1536/10" | bc`;
	# 100 Iteration, 8 Accs
	Throughput=`echo "scale=2; ${OPs}*100*8/${Time}/1000000000" | bc`;
    echo "ViT on 8 Duplicate Accs: ${Throughput} GOPs" >>Figure7_8_duplicate_${task};
elif [ $task == "NCF" ] || [ $task == "ncf" ]
then
    # M * K * N * 2
    let OPs=$(( 3072*4096*2048*2 + 3072*2048*1024*2 + 3072*1024*512*2 + 3072*512*256*2 + 3072*256*128*2 + 3072*128*64*2 + 3072*64*32*2 + 3072*32*16*2 + 3072*32*1*2));
    for ((n=1;n<=9;n++));
    do
    	read -r line
        if (( ${n} == 1 ))
    	then
    		Layer0=$line; 
    	elif (( ${n} == 2 ))
    	then
    		Layer1=$line; 
    	elif (( ${n} == 3 ))
    	then
    		Layer2=$line; 
    	elif (( ${n} == 4 ))
    	then
    		Layer3=$line;
        elif (( ${n} == 5 ))
    	then
    		Layer4=$line;
        elif (( ${n} == 6 ))
    	then
    		Layer5=$line;
        elif (( ${n} == 7 ))
    	then
    		Layer6=$line;
        elif (( ${n} == 8 ))
    	then
    		Layer7=$line;
        elif (( ${n} == 9 ))
    	then
    		Layer8=$line;
     	fi
    done < "Figure7_8_temp_$task.log"
    Time=`echo ${Layer0} + ${Layer1} + ${Layer2} + ${Layer3} + ${Layer4} + ${Layer5} + ${Layer6} + ${Layer7} + ${Layer8}| bc`;
    # 100 Iteration, 8 Accs
	Throughput=`echo "scale=2; ${OPs}*8*100/${Time}/1000000000" | bc`;
    echo "NCF on 8 Duplicate Accs: ${Throughput} GOPs" >>Figure7_8_duplicate_${task};
elif [ $task == "MLP" ] || [ $task == "mlp" ]
then
    # M * K * N * #layer * 2
    let OPs=$(( 3072*2048*4096*1*2 + 3072*4096*4096*2*2 + 3072*4096*1024*2));
    for ((n=1;n<=4;n++));
    do
    	read -r line
        if (( ${n} == 1 ))
    	then
    		Layer0=$line; 
    	elif (( ${n} == 2 ))
    	then
    		Layer1=$line; 
    	elif (( ${n} == 3 ))
    	then
    		Layer2=$line; 
    	elif (( ${n} == 4 ))
    	then
    		Layer3=$line;
     	fi
    done < "Figure7_8_temp_$task.log"
    Time=`echo ${Layer0} + ${Layer1} + ${Layer2} + ${Layer3} | bc`;
	# 100 Iteration, 8 Accs
    Throughput=`echo "scale=2; ${OPs}*10*8/${Time}/1000000000" | bc`;
    echo "MLP on 8 Duplicate Accs: ${Throughput} GOPs" >>Figure7_8_duplicate_${task};
fi