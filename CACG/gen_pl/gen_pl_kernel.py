from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from .gen_krl import*

def gen_pl_top(prj_dir,template_dir,pl_dir,hw_config,BUFF_SEL):
    kernel_type_num=2
    size=hw_config.shape[1]
    h1,w1,w2,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,X,Y,Z,data_type,kernel_type=hw_config[0,:]
    subprocess.run(['mkdir','-p' ,f'{pl_dir}'])
    template_path=[None]*kernel_type_num
    template_name=[None]*kernel_type_num
    environment=[None]*kernel_type_num
    ### Define template PATH
    for i in range (kernel_type_num):
        template_path[i] = template_dir+"/kernel"+ str(i)
        environment[i] = Environment(loader=FileSystemLoader(template_path[i]))
    
    layer=0
    krl_folder = Path(pl_dir)
    prj_folder = Path(prj_dir)
    BUFF_SEL_TEXT=['BRAM','BRAM','BRAM']
    for i in range(3):
        if BUFF_SEL[0,i]==0:
            BUFF_SEL_TEXT[i]='BRAM'
        else:
            BUFF_SEL_TEXT[i]='URAM'
    
    gen_para(environment[kernel_type],h1,w1,w2,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,X,Y,Z,data_type,krl_folder)
    gen_ddr (environment[kernel_type], krl_folder)
    gen_send(environment[kernel_type],A,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,krl_folder)
    gen_compute(environment[kernel_type],A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,BUFF_SEL_TEXT,krl_folder)
    gen_krl_top(environment[kernel_type],A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,krl_folder)
    gen_conn(environment[kernel_type],A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,layer,prj_folder)