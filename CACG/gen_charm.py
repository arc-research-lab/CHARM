from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from .gen_pl import *
from .gen_aie import *
from .gen_host import *

def gen_charm_top(prj_dir,final_config,device):
    
    Versal_HW=np.array(final_config[0]).astype(int)
    placement=np.array(final_config[1]).astype(int)
    BUFF_SEL=np.array(final_config[2])

    AIE_Conf=np.zeros([1,9]).astype(int)
    AIE_Conf[0,:]= np.append(Versal_HW[0,0:8],Versal_HW[0,12]).astype(int)

    PL_Conf=np.zeros([1,12]).astype(int)
    PL_Conf[0,:]= Versal_HW[0,0:12].astype(int)

    Host_Conf=np.zeros([1,11]).astype(int)
    Host_Conf[0,:]= Versal_HW[0,0:11].astype(int)

    ### Create Paths
    #########################
    template_dir = os.path.dirname(os.path.abspath(__file__)) + "/../templates/"
    template_dir_aie = template_dir + "aie/"
    template_dir_pl = template_dir + "pl_kernel/"
    template_dir_host = template_dir + "host/"
    pl_dir= prj_dir + '/kernel'

    # ### Copy Makefile
    copy_makefile = Path(template_dir +"/Makefile_" + device)
    subprocess.run(['cp', '-r', copy_makefile, f'{prj_dir+"/Makefile"}'])

    ### Code Gen
    #########################
    gen_aie_top(prj_dir,template_dir_aie,AIE_Conf,placement)
    gen_pl_top(prj_dir,template_dir_pl,pl_dir,PL_Conf,BUFF_SEL)
    gen_host_top(prj_dir,template_dir_host,Host_Conf,device)




