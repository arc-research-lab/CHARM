import os
import sys
import subprocess
import numpy as np
cur_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(cur_dir)
from charm import* 

model='MLP'
acc='one_spe'
DATA_TYPE=2

# h1,   w1,   w2,   A,   B,   C,  A_BRO, C_BRO,  PACK_IN, PACK_OUT, X,   Y,   Z,  data_type  kernel_type

#Single GEMM
if model=='GEMM':
    if DATA_TYPE==1:
        Versal_config=np.array([[64,64,64,8,4,6,3,2,2,2,4,1,8,1,7,0,9,0,6,0,0,1]]) # INT8
        #Versal_config=np.array([[64,64,64,12,4,4,2,2,2,2,4,1,8,1,7,0,1,0,4,0,0,1]]) # INT8
    elif DATA_TYPE==2:
        Versal_config=np.array([[48,32,48,12,3,8,4,2,3,2,3,2,4,2,5,0,7,0,8,0,0,1]]) # INT16
    elif DATA_TYPE==4:
        Versal_config=np.array([[32,32,32,12,4,8,4,2,4,4,4,1,4,4,1,0,1,0,8,0,0,1]]) # FP32

#8 unified 8u
elif acc=='8_unified':
    if DATA_TYPE==1:
        Versal_config=np.array([
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,6,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,10,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,14,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,18,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,22,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,26,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,30,0,6,1,1,0],
            [64,64,64,3,2,2,2,3,2,2,2,4,4,1,7,0,34,0,6,1,1,0],
            ]) # INT8
    elif DATA_TYPE==2:
        Versal_config=np.array([
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,1,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,7,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,13,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,19,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,25,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,31,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,37,0,4,1,0,1],
            [48,32,48,2,3,4,2,2,3,2,4,1,3,2,5,0,43,0,4,1,0,1],
            ]) # INT16

if acc=='one_spe':
    #One_spe BERT
    if model=='BERT':
        if DATA_TYPE==1:
            Versal_config=np.array([[64,64,64,8,4,4,2,2,2,2,1,1,2,1,7,0,9,0,4,0,1,0]]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([[48,32,48,12,3,6,3,2,3,2,1,1,2,2,5,0,8,0,6,1,1,0]]) # INT16

    #One_spe ViT
    elif model=='ViT':
        if DATA_TYPE==1:
            Versal_config=np.array([[64,64,64,4,4,4,2,2,2,2,1,1,1,1,7,0,17,0,4,0,0,0]]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([[48,32,48,4,6,4,2,2,3,2,1,1,1,2,5,0,13,0,4,0,0,0]]) # INT16

    #One_spe NCF
    elif model=='NCF':
        if DATA_TYPE==1:
            Versal_config=np.array([[64,64,64,12,4,4,2,2,2,2,1,4,1,1,7,0,1,0,4,1,1,0]]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([[48,32,48,16,3,6,3,2,3,2,2,1,4,2,5,0,1,0,6,0,0,1]]) # INT16
    
    #One_spe MLP
    elif model=='MLP':
        if DATA_TYPE==1:
            Versal_config=np.array([[64,64,64,12,4,4,2,2,2,2,4,1,8,1,7,0,1,0,4,0,0,1]]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([[48,32,48,16,3,6,3,2,3,2,2,1,5,2,5,0,1,0,6,0,0,1]]) # INT16
            
elif acc=='two_div':
    #two_div BERT
    if model=='BERT':
        if DATA_TYPE==1:
            Versal_config=np.array([
                [64,64,64,8,4,4,2,2,2,2,2,4,4,1,7,0,6,0,4,1,1,0],
                [64,64,64,4,2,2,1,2,2,2,1,1,1,1,7,0,39,0,4,1,1,0],
                ]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([
                [48,32,48,4,3,8,4,2,3,2,8,4,3,2,5,0,8,0,8,1,0,1],
                [48,32,48,4,3,2,2,2,3,2,1,1,1,2,5,0,26,0,2,1,0,1],
                ]) # INT16

    #two_div ViT
    elif model=='ViT':
        if DATA_TYPE==1:
            Versal_config=np.array([
                [64,64,64,8,4,4,2,2,2,2,2,4,4,1,7,0,6,0,4,1,1,0],
                [64,64,64,1,2,2,1,1,2,2,1,1,1,1,7,0,39,0,2,1,1,0],
                ]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([
                [48,32,48,4,3,8,4,2,3,2,8,4,3,2,5,0,8,0,8,1,0,1],
                [48,32,48,2,3,2,2,2,3,2,1,1,1,2,5,0,26,0,2,1,0,1],
                ]) # INT16
    
    #two_div NCF
    elif model=='NCF':
        if DATA_TYPE==1:
            Versal_config=np.array([
                [64,64,64,8,4,4,2,2,2,2,3,2,8,1,7,0,6,0,4,0,1,1],
                [64,64,64,4,2,4,2,2,2,2,2,4,2,1,7,0,39,4,4,1,1,0],
                ]) # INT8
        elif DATA_TYPE==2:
            Versal_config=np.array([
                [48,32,48,8,3,8,4,2,3,2,4,1,3,2,5,0,6,0,8,0,0,1],
                [48,32,48,4,3,8,4,2,3,2,8,1,1,2,5,0,32,0,8,0,0,1],
                ]) # INT16
    
    #two_div MLP
    elif model=='MLP':
        if DATA_TYPE==1:
            Versal_config=np.array([
                [64,64,64,8,4,4,2,2,2,2,3,2,8,1,7,0,6,0,4,0,1,1],
                [64,64,64,3,4,4,2,3,2,2,4,1,4,1,7,0,32,4,4,0,0,1],
                ]) # INT8

AIE_NAME= str(Versal_config[0,3]) + str(Versal_config[0,4]) + str(Versal_config[0,5])
BUF_NAME= str(Versal_config[0,10]) + str(Versal_config[0,11]) + str(Versal_config[0,12])
#Set the design project path
if DATA_TYPE==1:
    prj_dir= cur_dir + '/int8_' +  acc +'/' + model + '/int8_' + AIE_NAME + '_' + BUF_NAME
elif DATA_TYPE==2:
    prj_dir= cur_dir + '/int16_' +  acc +'/' + model + '/int16_' + AIE_NAME + '_' + BUF_NAME
elif DATA_TYPE==4:
    prj_dir= cur_dir + '/fp32_' +  acc +'/' + model + '/fp32_' + AIE_NAME + '_' + BUF_NAME
    
#Create the object of the class charm
subprocess.run(['mkdir','-p' ,f'{prj_dir}'])
automm=charm(prj_dir)

#Launch charm automatic code generator to emit the code for AIE, PL and Host CPU
device='vck190' # Supported devices are vck190 and vck5000
automm.cacg(Versal_config,device)