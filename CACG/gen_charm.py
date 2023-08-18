from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
import sys
from .gen_pl import *
from .gen_aie import *
from .gen_host import *
from .gen_make import *

def gen_charm_top(prj_dir,final_config,device):
    
    
    Versal_HW=np.array(final_config[:,0:15]).astype(int)
    placement=np.array(final_config[:,15:19]).astype(int)
    BUFF_SEL=np.array(final_config[:,19:22])
    data_type=Versal_HW[0,13]
    num_layer=Versal_HW.shape[0]
    
    AIE_Conf=np.zeros([num_layer,12]).astype(int)
    AIE_Conf[:,0:10]= Versal_HW[:,0:10].astype(int)
    AIE_Conf[:,10:12]= Versal_HW[:,13:15].astype(int)

    PL_Conf=np.zeros([num_layer,15]).astype(int)
    PL_Conf[:,:]= Versal_HW[:,0:15].astype(int)

    Host_Conf=np.zeros([num_layer,13]).astype(int)
    Host_Conf[:,:]= Versal_HW[:,0:13].astype(int)

    ### Create Paths
    #########################
    template_dir = os.path.dirname(os.path.abspath(__file__)) + "/../templates/"
    template_dir_aie = template_dir + "aie/"
    template_dir_pl = template_dir + "pl_kernel/"
    template_dir_host = template_dir + "host/"

    # ### Copy Makefile
    copy_makefile = Path(template_dir +"/Makefile_" + device)
    subprocess.run(['cp', '-r', copy_makefile, f'{prj_dir+"/Makefile"}'])

    ### Code Gen
    #########################
    gen_aie_top(prj_dir,template_dir_aie,AIE_Conf,placement)
    gen_pl_top(prj_dir,template_dir_pl,PL_Conf,BUFF_SEL)
    gen_host_top(prj_dir,template_dir_host,Host_Conf,data_type,device)
    gen_make_top(prj_dir,template_dir,num_layer,device)




