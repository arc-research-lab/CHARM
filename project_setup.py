import os
import sys
import subprocess
import numpy as np
cur_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(cur_dir)
from python_interface import* 

prj_dir= cur_dir + '/prj_try1'
subprocess.run(['mkdir','-p' ,f'{prj_dir}'])

A=np.random.rand(4096, 4096).astype(np.float32)
B=np.random.rand(4096, 4096).astype(np.float32)

automm=charm(prj_dir)

Versal_config=automm.cdse(A,B)

automm.cacg(Versal_config,'VCK5000')
