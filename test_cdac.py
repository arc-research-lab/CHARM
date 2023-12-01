import numpy as np
import math
import sys
from CDAC import *

num_acc=2
model='new1'
DATA_TYPE=1
run_mode='mul'
part=1

batch=72
seq=192
heads=12
head_dim=64
embed_dim=heads*head_dim
mlp_ratio=4
mlp_dim=embed_dim*mlp_ratio

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
elif model=='new':
    MODEL_IN=np.array([
    [192*1,192,576,1],     
    [192*1,64,192,3],
    [192*1,192,64,3],
    [192*1,192,192,1],
    [192*1,192,768,1],
    [192*1,768,192,1],
    ]) 
elif model=='new1':
    MODEL_IN=np.array([
    [seq*batch,embed_dim,embed_dim*3,1],
    [seq*batch,head_dim,seq,heads],
    [seq*batch,seq,head_dim,heads],
    [seq*batch,embed_dim,embed_dim,1],
    [seq*batch,embed_dim,mlp_dim,1],
    [seq*batch,mlp_dim,embed_dim,1],
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