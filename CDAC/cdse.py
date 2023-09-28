import numpy as np
import math
import sys
import os
from .broadcast_tuning import *
from .buffer_sel import *


def cdse_top(MODEL_IN,HW_Part,DATA_TYPE):

    ################ Hardware Constraints ################
    force_assign=0
    DDR_BANK=HW_Part[0]
    AIE_NUM=math.floor(HW_Part[1])
    PLIO_IN=math.floor(HW_Part[2])
    PLIO_OUT=math.floor(HW_Part[3])
    BRAM=math.floor(HW_Part[4])
    URAM=math.floor(HW_Part[5])
    
    ################ Hardware Setting ################
    #AXI Port Width in Bytes from PL <-> DDR
    AXI_WIDTH_A=512 
    AXI_WIDTH_B=512
    AXI_WIDTH_C=512

    #Single AIE Workload Settings
    if DATA_TYPE==1:
        H1,W1,W2=[64,64,64]
        PI,PK,PJ=[8,64,4]
        mac=128
        COMPUTE_CYCLE=(PI*PK*PJ//mac+4)*(H1//PI)*(W1//PK)*(W2//PJ)+40 #60: Acc_stall
        if AIE_NUM<=50:
            PACK_IN = 1
            PACK_OUT= 1
            kernel_type=6
        else:
            PACK_IN = 2
            PACK_OUT= 2
            kernel_type=7
    elif DATA_TYPE==2:
        H1,W1,W2=[48,32,48]
        PI,PK,PJ=[16,32,2]
        mac=32
        COMPUTE_CYCLE=(PI*PK*PJ//mac+4)*(H1//PI)*(W1//PK)*(W2//PJ)+40 #50: Acc_stall
        if AIE_NUM<=50:
            PACK_IN = 1
            PACK_OUT= 1
            kernel_type=4
        else:
            PACK_IN = 3
            PACK_OUT= 2
            kernel_type=5
    elif DATA_TYPE==4:
        H1,W1,W2=[32,32,32]
        PI,PK,PJ=[8,8,2]
        mac=8
        COMPUTE_CYCLE=(PI*PK*PJ//mac+1)*(H1//PI)*(W1//PK)*(W2//PJ)+40 #40: Acc_stall
        if AIE_NUM<=100:
            PACK_IN = 1
            PACK_OUT= 1
            kernel_type=0
        else:
            PACK_IN = 4
            PACK_OUT= 4
            kernel_type=1
    
    #Buffer Configurations
    RAM_TYPE_A=2 #"1":1P "2":T2P
    RAM_TYPE_B=2
    RAM_TYPE_C=2
    DBUFF_L=2    #"1":single buffer "2":double buffer
    DBUFF_R=2
    DBUFF_O=2

    #PLIO Width Configurations
    if DATA_TYPE==1:
        AXIS_WIDTH_A=64     # PLIO Port Width from PL <-> AIE
        AXIS_WIDTH_B=128
        AXIS_WIDTH_C=64
        PART_A=AXI_WIDTH_A//(AXIS_WIDTH_A*RAM_TYPE_A)
        PART_B=AXI_WIDTH_B//(AXIS_WIDTH_B*RAM_TYPE_B)
        PART_C=AXI_WIDTH_C*2//(AXIS_WIDTH_C*RAM_TYPE_C)
    else:
        AXIS_WIDTH_A=128   # PLIO Port Width from PL <-> AIE
        AXIS_WIDTH_B=128
        AXIS_WIDTH_C=128
        PART_A=AXI_WIDTH_A//(AXIS_WIDTH_A*RAM_TYPE_A)
        PART_B=AXI_WIDTH_B//(AXIS_WIDTH_B*RAM_TYPE_B)
        PART_C=AXI_WIDTH_C//(AXIS_WIDTH_C*RAM_TYPE_C)

    #Num of Elements during PL <-> AIE Transmission
    NUM_PER_PORT_A=AXIS_WIDTH_A//(DATA_TYPE*8)
    NUM_PER_PORT_B=AXIS_WIDTH_B//(DATA_TYPE*8)
    NUM_PER_PORT_C=AXIS_WIDTH_C//(DATA_TYPE*8)

    #Data Transmission During One Iteration of Graph
    LEFT_SIZE =H1*W1//NUM_PER_PORT_A
    RIGHT_SIZE=W1*W2//NUM_PER_PORT_B
    OUT_SIZE  =H1*W2//NUM_PER_PORT_C

    # PL Frequency
    freq_rate=230/250

    # One-Time Profling of DDR Bandwidth
    BW_L_S=(14*DDR_BANK)*freq_rate
    BW_R_S=(14*DDR_BANK)*freq_rate
    BW_O_S=(11*DDR_BANK)*freq_rate

    BW_L_DR=(11*DDR_BANK)*freq_rate
    BW_R_DL=(11*DDR_BANK)*freq_rate

    BW_L_DO=(11*DDR_BANK)*freq_rate
    BW_R_DO=(11*DDR_BANK)*freq_rate
    BW_O_D=(11*DDR_BANK)*freq_rate

    BW_L_T=(8*DDR_BANK)*freq_rate
    BW_R_T=(8*DDR_BANK)*freq_rate
    BW_O_T=(8*DDR_BANK)*freq_rate


    ###################### Initialization ###########################
    best_time=1e30
    cnt_choice=0
    cnt_best=0
    num_term=17
    sample_num=MODEL_IN.shape[0]
    num_design_best=50    #At most 50 choices reserved for the current best
    num_design_choice=50  #At most 50 choices reserved for the near best point
    temp_cycle=np.zeros([1,sample_num])
    config=np.ones([num_design_best+num_design_choice,num_term+sample_num])*1e30

    A,B,C,X,Y,Z,buf_sel=[12,4,8,4,1,4,1]
    ############################ DSE Kernel0 ###############################

    for c in range(1, 8+1):      ##Row Constaint
        for b in range(1, PACK_IN*2+1): ##Col Constaint
            for a in range(1, AIE_NUM//(b*c)+2):
                if (force_assign==1) and ((a!=A) or (b!=B) or (c!=C)):
                    continue
                if kernel_type%2==1:
                    A_BRO = c//2
                    height=min(c,8)
                    if  a%2==0:
                        C_BRO=2
                    elif a%3==0:
                        C_BRO=3
                    else:
                        C_BRO=1
                    if (b%PACK_IN!=0) or (c%PACK_OUT!=0):
                        continue
                else:
                    A_BRO= c
                    C_BRO, height = broadC_factor(a,b,c)
                    if ((b>8 and b%4!=0) or b>16):
                        continue
                ############ Determine A_BRO and C_BRO ###########
                
                

                ############ Calculate PLIO ###########
                plio_in_lhs= a*(b//PACK_IN)*(c//A_BRO)
                plio_in_rhs= c*(b//PACK_IN)*(a//C_BRO)
                plio_in=plio_in_lhs + plio_in_rhs
                plio_out=a*(c//PACK_OUT)
                if plio_in>PLIO_IN or plio_out>PLIO_OUT:
                    continue

                ############ Verify Placement ###########
                length=placement_verify(a,b,c,height)
                if length > 50:
                    continue

                for x in range(1, 16):
                    for y in range(1, 16): 
                        for z in range(1, 16):
                            if (force_assign==1) and ((x!=X) or (y!=Y) or (z!=Z)):
                                continue
                            bram_use,uram_use,buf_index=buff_count_0(BRAM,URAM,PART_A,PART_B,PART_C,PACK_IN,PACK_OUT,LEFT_SIZE,RIGHT_SIZE,OUT_SIZE,a,b,c,x,y,z,DBUFF_L,DBUFF_R,DBUFF_O,RAM_TYPE_A,RAM_TYPE_B,RAM_TYPE_C,DATA_TYPE,force_assign,buf_sel)
                            if (bram_use>BRAM or uram_use>URAM):
                                break

                            
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
                            AIE_CYCLE=math.ceil(max([(H1*W1*DATA_TYPE//4),(W1*W2*DATA_TYPE//4),COMPUTE_CYCLE]))*x*y*z+(H1*W1*DATA_TYPE//4)*PACK_IN+(H1*W2*DATA_TYPE//4)*PACK_OUT   
                            
                            for large_t in range(sample_num): 
                                M=MODEL_IN[large_t,0]
                                K=MODEL_IN[large_t,1]
                                N=MODEL_IN[large_t,2]
                                X_TILE=math.ceil(M/(x*a*H1))
                                Y_TILE=math.ceil(K/(y*b*W1))
                                Z_TILE=math.ceil(N/(z*c*W2))

                                if Y_TILE==1 and X_TILE==1 and Z_TILE==1:
                                    temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+AIE_CYCLE+store_O_S
                                elif Y_TILE==1:
                                    temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+max([load_L_DR,load_R_DL,AIE_CYCLE])+max([AIE_CYCLE,store_O_S])+max([load_L_T,load_R_T,AIE_CYCLE,store_O_T])*(X_TILE*Y_TILE*Z_TILE-2)+store_O_S
                                else:
                                    temp_cycle[0,large_t]=max([load_L_DR,load_R_DL])+max([load_L_DR,load_R_DL,AIE_CYCLE])*((X_TILE*Y_TILE*Z_TILE-1)-(X_TILE*Z_TILE-1))+max([load_L_T,load_R_T,AIE_CYCLE,store_O_T])*(X_TILE*Z_TILE-1)+AIE_CYCLE+store_O_S

                            temp0_cycle=np.multiply(temp_cycle,np.transpose(MODEL_IN[:,3]))
                            total_cycle=np.sum(temp0_cycle)
                                
                            if(total_cycle*0.85<=best_time): # Search design near the best time
                                if total_cycle<best_time:   # If it is the current best
                                    best_time=total_cycle
                                    index=cnt_best%num_design_best
                                    cnt_best=cnt_best+1
                                else:
                                    index=num_design_best+(cnt_choice%num_design_choice)
                                    cnt_choice=cnt_choice+1
                                    
                                config[index,0]=total_cycle
                                config[index,1]=a
                                config[index,2]=b
                                config[index,3]=c
                                config[index,4]=A_BRO
                                config[index,5]=C_BRO
                                config[index,6]=x
                                config[index,7]=y
                                config[index,8]=z
                                config[index,9]=length
                                config[index,10]=height
                                config[index,11]=plio_in
                                config[index,12]=plio_out
                                config[index,13]=a*b*c
                                config[index,14]=bram_use
                                config[index,15]=uram_use
                                config[index,16]=buf_index
                                config[index,num_term:num_term+sample_num]=temp0_cycle[:].copy()

    config = config[config[:,0].argsort()]
    best_cycle=config[0,0]
    Versal_HW_temp=config[0,:].copy()
    Versal_HW=np.zeros([1,15]) # h1,   w1,   w2,   A,   B,   C,  A_BRO, C_BRO,  PACK_IN, PACK_OUT, X,   Y,   Z,  data_type  kernel_type
    Versal_HW[0,0:3]=[H1,W1,W2]
    Versal_HW[0,3:8]=Versal_HW_temp[1:6].copy()
    Versal_HW[0,8]=PACK_IN
    Versal_HW[0,9]=PACK_OUT
    Versal_HW[0,10:13]=Versal_HW_temp[6:9].copy()
    Versal_HW[0,13]=DATA_TYPE
    Versal_HW[0,14]=kernel_type  

    placement=np.zeros([1,4]) #layer,col,row,height 
    col=(50-Versal_HW_temp[9])//2
    row=0
    placement[0,1:4]=[col,row,Versal_HW_temp[10]] 

    BUFF_SEL=np.zeros([1,3])
    buff_sel_temp=np.binary_repr(Versal_HW_temp[16].astype(int), width=3)
    for i in range(3):
        if buff_sel_temp[i]=="0":
            BUFF_SEL[0,i]=0
        else:
            BUFF_SEL[0,i]=1         
    
    final_temp=np.concatenate((Versal_HW, placement), axis=1)
    final_config=np.concatenate((final_temp,BUFF_SEL),axis=1)
    bram_use,uram_use,buf_index=buff_count_0(BRAM,URAM,PART_A,PART_B,PART_C,PACK_IN,PACK_OUT,LEFT_SIZE,RIGHT_SIZE,OUT_SIZE,Versal_HW[0,3],Versal_HW[0,4],Versal_HW[0,5],Versal_HW[0,10],Versal_HW[0,11],Versal_HW[0,12],DBUFF_L,DBUFF_R,DBUFF_O,RAM_TYPE_A,RAM_TYPE_B,RAM_TYPE_C,DATA_TYPE,force_assign,buf_sel) 
    HW_Used=[config[0,13],config[0,11],config[0,12],bram_use,uram_use]#AIE,PLIO_IN,PLIO_OUT,BRAM_URAM
    return final_config,best_cycle,config[0,num_term:num_term+sample_num],HW_Used
        
            