from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
import sys
sys.path.append('/home/pittzhou/Jinming/Pitts_CHARM/CACG/gen_host')
from gen_host_func import*


prj_dir= '/home/pittzhou/Jinming/Pitts_CHARM/CACG/prj_new'
template_dir = "/home/pittzhou/Jinming/Pitts_CHARM/templates/host"
host_dir= prj_dir + '/host'
subprocess.run(['mkdir','-p' ,f'{host_dir}'])


#########################
h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z=32,32,32,6,3,4,2,3,8,1,2
#########################

kernel_type_num=1
template_path=[None]*kernel_type_num
template_name=[None]*kernel_type_num
environment=[None]*kernel_type_num
### Define template PATH
for i in range (kernel_type_num):
    template_path[i] = template_dir+"/kernel"+ str(i)
    environment[i] = Environment(loader=FileSystemLoader(template_path[i]))

host_folder= Path(host_dir)

gen_host(environment[0],h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z,host_folder)