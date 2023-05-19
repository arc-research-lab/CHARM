from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
import sys
sys.path.append('/home/pittzhou/Jinming/Pitts_CHARM/CACG/gen_pl')
from gen_krl import*

prj_dir= '/home/pittzhou/Jinming/Pitts_CHARM/CACG/prj_view'
template_dir = "/home/pittzhou/Jinming/Pitts_CHARM/templates/pl_kernel"
pl_dir= prj_dir + '/kernel'
subprocess.run(['mkdir','-p' ,f'{pl_dir}'])


#########################
h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z,data_type=32,32,32,6,3,4,2,3,8,1,2,32
#########################

BUFF_SEL=["BRAM","BRAM","BRAM"]


kernel_type_num=1

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