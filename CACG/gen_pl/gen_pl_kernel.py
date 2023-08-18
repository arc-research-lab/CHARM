from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from .gen_krl import*

def gen_pl_top(prj_dir,template_dir,hw_config,BUFF_SEL):
    kernel_type_num=8
    num_krl=hw_config.shape[0]
    pl_dir= prj_dir + '/kernel'
    gen_conn_top(prj_dir,template_dir,hw_config)
    for acc in range(num_krl):
        h1,w1,w2,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,X,Y,Z,data_type,kernel_type=hw_config[acc,:]
        pl_dir_krl=pl_dir + str(acc)
        subprocess.run(['mkdir','-p' ,f'{pl_dir_krl}'])
        template_path=[None]*kernel_type_num
        environment=[None]*kernel_type_num
        ### Define template PATH
        for i in range (kernel_type_num):
            template_path[i] = template_dir+"/kernel"+ str(i)
            environment[i] = Environment(loader=FileSystemLoader(template_path[i]))

        krl_folder = Path(pl_dir_krl)
        
        BUFF_SEL_TEXT=['BRAM','BRAM','BRAM']
        for i in range(3):
            if BUFF_SEL[0,i]==0:
                BUFF_SEL_TEXT[i]='BRAM'
            else:
                BUFF_SEL_TEXT[i]='URAM'

        gen_para(environment[kernel_type],h1,w1,w2,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,X,Y,Z,data_type,acc,krl_folder)
        gen_ddr (environment[kernel_type],acc,krl_folder)
        gen_send(environment[kernel_type],A,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,acc,krl_folder)
        gen_compute(environment[kernel_type],A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,BUFF_SEL_TEXT,acc,krl_folder)
        gen_krl_top(environment[kernel_type],A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,acc,krl_folder)

def gen_conn_top(prj_dir,template_dir,hw_config):
    prj_folder = Path(prj_dir)
    Conn_Array_temp = hw_config[:,3:10]
    num_layer = hw_config.shape[0]
    Conn_Array=np.zeros((num_layer,7)).astype(int)
    Conn_Array[:,0:3]=Conn_Array_temp[:,0:3]
    Conn_Array[:,3]=np.floor_divide(Conn_Array_temp[:,2],Conn_Array_temp[:,3])#NUM_TXL
    Conn_Array[:,4]=np.floor_divide(Conn_Array_temp[:,0],Conn_Array_temp[:,4])#NUM_TXR
    Conn_Array[:,5:7]=Conn_Array_temp[:,5:7]
    
    ### Define template PATH
    environment = Environment(loader=FileSystemLoader(template_dir))
    gen_conn(environment,Conn_Array,num_layer,prj_folder)