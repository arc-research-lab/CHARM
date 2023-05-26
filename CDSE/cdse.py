import numpy as np
import math
from broadcast_tuning import *
from buffer_sel import *

DATA_TYPE=4

MODEL_IN=np.array([
     # M,     K,    N,   #layer
    [ 4096,  4096, 4096,   1],
]) 
total_ops = np.sum(np.multiply(np.multiply(np.multiply(MODEL_IN[:,0],MODEL_IN[:,1]),MODEL_IN[:,2]),MODEL_IN[:,3]))*2

################ Hardware Constraints ################
portion=1
DDR_BANK=1*portion
AIE_NUM=400*portion
BRAM=(967-100)*portion #100 for AXI bound consumpssion
URAM=(463-43)*portion
PLIO_IN=156*portion
PLIO_OUT=117*portion


################ Hardware Setting ################
#AXI Port Width in Bytes from PL <-> DDR
AXI_WIDTH_A=512//8 
AXI_WIDTH_B=512//8
AXI_WIDTH_C=512//8

#Single AIE Workload Settings
if DATA_TYPE==1:
    H1=64
    W1=64
    W2=64
    mac=128
    PAC_LR = 2
    PAC_OUT= 2
    EFF_SINGLE=0.80
elif DATA_TYPE==2:
    H1=48
    W1=32
    W2=48
    mac=32
    PAC_LR = 3
    PAC_OUT= 2
    EFF_SINGLE=0.80
elif DATA_TYPE==4:
    H1=32
    W1=32
    W2=32
    mac=8
    PAC_LR = 1
    PAC_OUT= 1
    EFF_SINGLE=0.80

#Buffer Configurations
RAM_TYPE_A=2 #"1":1P "2":T2P
RAM_TYPE_B=2
RAM_TYPE_C=2
DBUFF_L=2    #"1":single buffer "2":double buffer
DBUFF_R=2
DBUFF_O=2

#PLIO Width Configurations
if DATA_TYPE==1:
    AXIS_WIDTH_A=64//8   # PLIO Port Width from PL <-> AIE
    AXIS_WIDTH_B=128//8
    AXIS_WIDTH_C=64//8
    PART_A=AXI_WIDTH_A//(AXIS_WIDTH_A*RAM_TYPE_A)
    PART_B=AXI_WIDTH_B//(AXIS_WIDTH_B*RAM_TYPE_B)
    PART_C=AXI_WIDTH_C*2//(AXIS_WIDTH_C*RAM_TYPE_C)
else:
    AXIS_WIDTH_A=128//8  # PLIO Port Width from PL <-> AIE
    AXIS_WIDTH_B=128//8
    AXIS_WIDTH_C=128//8
    PART_A=AXI_WIDTH_A//(AXIS_WIDTH_A*RAM_TYPE_A)
    PART_B=AXI_WIDTH_B//(AXIS_WIDTH_B*RAM_TYPE_B)
    PART_C=AXI_WIDTH_C//(AXIS_WIDTH_C*RAM_TYPE_C)

#Num of Elements during PL <-> AIE Transmission
NUM_PER_PORT_A=AXIS_WIDTH_A//DATA_TYPE
NUM_PER_PORT_B=AXIS_WIDTH_B//DATA_TYPE
NUM_PER_PORT_C=AXIS_WIDTH_C//DATA_TYPE

#Data Transmission During One Iteration of Graph
LEFT_SIZE =H1*W1//NUM_PER_PORT_A
RIGHT_SIZE=W1*W2//NUM_PER_PORT_B
OUT_SIZE  =H1*W2//NUM_PER_PORT_C

# PL Frequency
freq_rate=220/250

# One-Time Profling of DDR Bandwidth
BW_L_S=(12*DDR_BANK)*freq_rate
BW_R_S=(12*DDR_BANK)*freq_rate
BW_O_S=(8.5*DDR_BANK)*freq_rate

BW_L_DR=(8*DDR_BANK)*freq_rate
BW_R_DL=(8*DDR_BANK)*freq_rate

BW_L_DO=(8*DDR_BANK)*freq_rate
BW_R_DO=(8*DDR_BANK)*freq_rate
BW_O_D=(6*DDR_BANK)*freq_rate

BW_L_T=(7*DDR_BANK)*freq_rate
BW_R_T=(7*DDR_BANK)*freq_rate
BW_O_T=(7*DDR_BANK)*freq_rate


###################### Initialization ###########################
best_time=1e30
cnt_choice=0
cnt_best=0
num_term=17
sample_num=MODEL_IN.shape[0]
num_design_best=50    #At most 50 choices reserved for the current best
num_design_choice=50  #At most 50 choices reserved for the near best point
temp_cycle=np.zeros([1,sample_num])
config=np.zeros([num_design_best+num_design_choice,num_term+sample_num-1])


############################ DSE Kernel0 ###############################

for c in range(1, 8+1):      ##Row Constaint
    print("DSE Processes ------------ " + str(12.5*c) + "%")
    for b in range(1, 50+1): ##Col Constaint
        for a in range(1, AIE_NUM//(c*b)+1):  
            if (b>8 and b%4!=0) or b>16:
                continue
            ############ Determine A_BRO and C_BRO ###########
            A_BRO=c
            C_BRO, height = broadC_factor(a,b,c)

            ############ Calculate PLIO ###########
            plio_in_lhs= a*b*c//A_BRO
            plio_in_rhs= c*b*a//C_BRO
            plio_in=plio_in_lhs + plio_in_rhs
            plio_out=a*c
            if plio_in>PLIO_IN or plio_out>PLIO_OUT:
                continue

            ############ Verify Placement ###########
            length=placement_verify(a,b,c,height)
            if length > 50:
                continue
            
            for x in range(1, 16+1):
                for y in range(1, 16+1): 
                    for z in range(1, 16+1):
                        bram_use,uram_use,buf_index=buff_count_0(BRAM,URAM,PART_A,PART_B,PART_C,LEFT_SIZE,RIGHT_SIZE,OUT_SIZE,a,b,c,x,y,z,DBUFF_L,DBUFF_R,DBUFF_O,RAM_TYPE_A,RAM_TYPE_B,RAM_TYPE_C)
                        if (bram_use>BRAM or uram_use>URAM):
                            continue

                        for large_t in range(sample_num):
                            TILEL_SIZE=a*b*LEFT_SIZE*DATA_TYPE*NUM_PER_PORT_A
                            TILER_SIZE=b*c*RIGHT_SIZE*DATA_TYPE*NUM_PER_PORT_B
                            TILEO_SIZE=a*c*OUT_SIZE*DATA_TYPE*NUM_PER_PORT_C
                            load_L_S  = math.ceil(TILEL_SIZE*x*y/BW_L_S)
                            load_L_DR = math.ceil(TILEL_SIZE*x*y/BW_L_DR)
                            load_L_DO = math.ceil(TILEL_SIZE*x*y/BW_L_DO)
                            load_L_T  = math.ceil(TILEL_SIZE*x*y/BW_L_T)
                            load_R_S  = math.ceil(TILER_SIZE*y*z/BW_R_S)
                            load_R_DL = math.ceil(TILER_SIZE*y*z/BW_R_DL)
                            load_R_DO = math.ceil(TILER_SIZE*y*z/BW_R_DO)
                            load_R_T  = math.ceil(TILER_SIZE*y*z/BW_R_T)
                            store_O_S = math.ceil(TILEO_SIZE*x*z/BW_O_S)
                            store_O_D = math.ceil(TILEO_SIZE*x*z/BW_O_D)
                            store_O_T = math.ceil(TILEO_SIZE*x*z/BW_O_T)
                            COMP_CYCLE=math.ceil(max([(H1*W1//4),(W1*W2//4),((H1*W1*W2/mac)/EFF_SINGLE)]))*x*y*z

                            M=MODEL_IN[large_t,0]
                            K=MODEL_IN[large_t,1]
                            N=MODEL_IN[large_t,2]
                            X_TILE=math.ceil(M/(x*a*H1))
                            Y_TILE=math.ceil(K/(y*b*W1))
                            Z_TILE=math.ceil(N/(z*c*W2))

                            if Y_TILE==1 and X_TILE==1 and Z_TILE==1:
                                temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+COMP_CYCLE+store_O_S
                            elif Y_TILE==1:
                                temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+max([load_L_DR,load_R_DL,COMP_CYCLE])+max([COMP_CYCLE,store_O_S])+max([load_L_T,load_R_T,COMP_CYCLE,store_O_T])*(X_TILE*Y_TILE*Z_TILE-2)+store_O_S
                            else:
                                temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+max([load_L_DR,load_R_DL,COMP_CYCLE])*((X_TILE*Y_TILE*Z_TILE-1)-(X_TILE*Z_TILE-1))+max([load_L_T,load_R_T,COMP_CYCLE,store_O_T])*(X_TILE*Z_TILE-1)+COMP_CYCLE+store_O_S

                            temp0_cycle=np.multiply(temp_cycle,np.transpose(MODEL_IN[:,3]))
                            total_cycle=np.sum(temp0_cycle)

                            if(total_cycle*0.85<=best_time): # Search design near the best time
                                if total_cycle<best_time:   # If it is the current best
                                    best_time=total_cycle
                                    index=cnt_best%num_design_best
                                    cnt_best=cnt_best+1
                                else:
                                    index=num_design_best+cnt_choice%num_design_choice
                                    cnt_choice=cnt_choice+1
                                
                                config[index,0]=total_ops/total_cycle
                                config[index,1]=a
                                config[index,2]=b
                                config[index,3]=c
                                config[index,4]=x
                                config[index,5]=y
                                config[index,6]=z
                                config[index,7]=A_BRO
                                config[index,8]=C_BRO
                                config[index,9]=height
                                config[index,10]=length
                                config[index,11]=plio_in
                                config[index,12]=plio_out
                                config[index,13]=a*b*c
                                config[index,14]=bram_use
                                config[index,15]=uram_use
                                config[index,16]=buf_index
                                config[index,num_term:num_term+sample_num-1]=temp0_cycle[:]
                                # print("\n\n")
                                # print("Index is " + str(index) + ":" )
                                # print(config[index,:])
                                # print("\n\n")


config = config[config[:,0].argsort()[::-1]]


print(config[0,:])
            

            
            


        
            