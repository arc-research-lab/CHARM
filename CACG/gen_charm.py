from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from gen_pl import *
from gen_aie import *
from gen_host import *

prj_dir= '/home/pittzhou/Jinming/Pitts_CHARM/CACG/prj_try'
subprocess.run(['mkdir','-p' ,f'{prj_dir}'])

### Create Paths
#########################
template_dir = os.path.dirname(os.path.abspath(__file__)) + "/../templates/"
template_dir_aie = template_dir + "aie/"
template_dir_pl = template_dir + "pl_kernel/"
template_dir_host = template_dir + "host/"
pl_dir= prj_dir + '/kernel'

### Input Arguments
#########################

Versal_HW=np.array([
    # h1,   w1,   w2,   A,   B,   C,  A_BRO, C_BRO,   X,   Y,   Z,  data_type  kernel_type
    [ 32,   32,   32,   2,   4,   2,    2,     2,     1,   1,   1,     32,         0],
]) 


AIE_Conf=np.array([[ 0,   0,   0,   0,   0,   0,   0,   0,   0],])
AIE_Conf[0,:] = np.append(Versal_HW[0,0:8],Versal_HW[0,12])

placement=np.array([
   #layer  col  row height 
    [ 0,   12,   2,   6 ],
])

PL_Conf=np.array([[ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0],])
PL_Conf[0,:] = Versal_HW[0,0:12]
BUFF_SEL=["BRAM","BRAM","BRAM"]


Host_Conf=np.array([[ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],])
Host_Conf[0,:]= Versal_HW[0,0:11]


# ### Copy Makefile
copy_makefile = Path(template_dir +"/Makefile")
subprocess.run(['cp', '-r', copy_makefile, f'{prj_dir}'])

### Code Gen
#########################
gen_aie_top(prj_dir,template_dir_aie,AIE_Conf,placement)
gen_pl_top(prj_dir,template_dir_pl,pl_dir,PL_Conf,BUFF_SEL)
gen_host_top(prj_dir,template_dir_host,Host_Conf)




