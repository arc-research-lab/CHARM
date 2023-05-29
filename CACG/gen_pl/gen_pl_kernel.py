from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from .gen_krl import*

def gen_pl_top(prj_dir,template_dir,pl_dir,hw_config,BUFF_SEL):
    kernel_type_num=1
    size=hw_config.shape[1]
    h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z,data_type=hw_config[0,:]
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
    
    gen_para(environment[0],h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z,data_type,krl_folder)
    gen_ddr (environment[0], krl_folder)
    gen_send(environment[0],A,C,A_BRO,C_BRO,krl_folder)
    gen_compute(environment[0],A,B,C,A_BRO,C_BRO,BUFF_SEL,krl_folder)
    gen_krl_top(environment[0],A,B,C,A_BRO,C_BRO,krl_folder)
    gen_conn(environment[0],A,B,C,A_BRO,C_BRO,layer,prj_folder)