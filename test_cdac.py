import numpy as np
import math
import sys
from CDAC import *

num_acc=1
model='bert'
DATA_TYPE=1
run_mode='mul'
part=1

if model=='bert':
    MODEL_IN=np.array([
    [3072,1024,3072,1],   
    [3072,1024,1024,1],  
    [3072,1024,4096,1],
    [3072,4096,1024,1],
    [512,64,512,96],  
    [512,512,64,96],  
    ])
elif model=='vit':
    MODEL_IN=np.array([
    [3072,3024,1024,1],
    [3072,1024,3072,1],   
    [3072,1024,1024,1],  
    [3072,1024,4096,1],
    [3072,4096,1024,1],
    [64,64,64,768],  
    [64,64,64,768],  
    ])
elif model=='ncf':
    MODEL_IN=np.array([
    [3072,4096,2048,1],   
    [3072,2048,1024,1],  
    [3072,1024,512,1],
    [3072,512,256,1],
    [3072,256,128,1],
    [3072,128,64,1],  
    [3072,64,32,1], 
    [3072,32,16,1],   
    [3072,32,1,1],
    ])  
elif model=='mlp':
    MODEL_IN=np.array([
    [3072,2048,4096,1],   
    [3072,4096,4096,1],  
    [3072,4096,4096,1],
    [3072,4096,1024,1],
    ]) 
else:
    MODEL_IN=np.array([
    [3072,4096,4096,1],   
    ]) 

num_layer=MODEL_IN.shape[0]
if run_mode=='mul':
    part_final, final_config, layer_cycle=cdac_top(MODEL_IN,DATA_TYPE,num_acc)
    # assign_map=np.zeros((num_layer,num_acc)).astype(int)
    # for acc in range(num_acc):
    #     assign_map[part_final[acc],acc]=1
    # print(assign_map)
        
        
elif run_mode=='sin':
    config,throughput,cycle=cdse1_top(MODEL_IN,DATA_TYPE,part)
    print(config)
    print(throughput)