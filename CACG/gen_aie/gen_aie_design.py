from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
import sys
sys.path.append('/home/pittzhou/Jinming/Pitts_CHARM/CACG/gen_aie')
from gen_array import*
from gen_kernel import*
from gen_combine import*

### Input Arguments
Model_ViT=np.array([
  # h1/seq w1/K  w2/OC  A    B     C  A_BRO C_BRO k_type
    [ 32,   32,   32,   6,   3,    4,   2,   3,    0],
]) 

placement=np.array([
   #layer  col  row height 
    [ 0,   12,   2,   6 ],
])

kernel_type_num=1

num_layer=Model_ViT.shape[0]
port_width=128
freq=250
L_list=[0]
HW_Conf=Model_ViT[:,3:]

prj_dir= '/home/pittzhou/Jinming/Pitts_CHARM/CACG/prj_new'
template_dir = "/home/pittzhou/Jinming/Pitts_CHARM/templates/aie/"
aie_dir= prj_dir + '/aie'
subprocess.run(['mkdir','-p' ,f'{aie_dir}'])
data_dir = template_dir +"/data"

template_path=[None]*kernel_type_num
template_name=[None]*kernel_type_num
environment=[None]*kernel_type_num
### Define template PATH
for i in range (kernel_type_num):
    template_path[i] = template_dir+"kernel"+ str(i)
    template_name[i] = template_path[i]  + "/aie"
    environment[i] = Environment(loader=FileSystemLoader(template_name[i]))

environment_kernel=[None]*kernel_type_num
for i in range (kernel_type_num):
    environment_kernel[i] = Environment(loader=FileSystemLoader(template_name[i]))

environment_combine = Environment(loader=FileSystemLoader(template_dir))

### Copy Makefile and Data
copy_makefile = Path(template_dir +"/Makefile")
copy_data = Path(data_dir)
subprocess.run(['cp', '-r', copy_makefile, f'{prj_dir}'])
subprocess.run(['cp', '-r', copy_data, f'{prj_dir + "/data"}'])

Port_Conf_All = np.zeros((HW_Conf.shape[0],3)).astype(int)
for i in range(HW_Conf.shape[0]):
    Port_Conf_All[i][0]=HW_Conf[i][0]*(HW_Conf[i][2]//HW_Conf[i][3])*HW_Conf[i][1]  #lhs_port
    Port_Conf_All[i][1]=HW_Conf[i][2]*(HW_Conf[i][0]//HW_Conf[i][4])*HW_Conf[i][1]  #rhs_port
    Port_Conf_All[i][2]=HW_Conf[i][0]*HW_Conf[i][2]                                 #out_port



Port_Conf=Port_Conf_All[L_list,:]
port_total=np.sum(Port_Conf, 0)

Port_Conf_Temp = np.zeros((Port_Conf.shape[0],3)).astype(int)
for i in range(Port_Conf.shape[0]):
    if i!=0:
        Port_Conf_Temp[i][0]=Port_Conf_Temp[i-1][0]+Port_Conf[i-1][0]
        Port_Conf_Temp[i][1]=Port_Conf_Temp[i-1][1]+Port_Conf[i-1][1]
        Port_Conf_Temp[i][2]=Port_Conf_Temp[i-1][2]+Port_Conf[i-1][2]
Port_Conf_Pre = np.zeros((HW_Conf.shape[0],3)).astype(int)
Port_Conf_Pre[L_list,:]= Port_Conf_Temp

gen_combine(environment_combine,port_width,freq,L_list,HW_Conf,Port_Conf_All,Port_Conf_Pre,port_total,Path(aie_dir))

for i in L_list:
    layer=i
    h1 = Model_ViT[layer][0]
    w1 = Model_ViT[layer][1]
    w2 = Model_ViT[layer][2]
    A  = Model_ViT[layer][3]
    B  = Model_ViT[layer][4]
    C  = Model_ViT[layer][5]
    A_BRO = Model_ViT[layer][6]
    C_BRO = Model_ViT[layer][7]
    kernel_type=Model_ViT[layer][8]
    file_dir=aie_dir + "/layer" + str(layer)
    layer_name=str(layer)

    pos_col  = placement[i][1]
    pos_row  = placement[i][2]
    height   = placement[i][3]
    subprocess.run(['mkdir','-p' ,file_dir])
    aie_folder = Path(file_dir)
    gen_para(environment[kernel_type],h1,w1,w2,A,B,C,A_BRO,C_BRO,layer_name,aie_folder)
    gen_krnl(environment_kernel[kernel_type],w1,kernel_type,layer_name,aie_folder)
    gen_grah(environment[kernel_type],B,layer_name,aie_folder)
    gen_toph(environment[kernel_type],A,B,C,A_BRO,C_BRO,pos_col,pos_row,height,layer_name,aie_folder)

