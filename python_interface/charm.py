import numpy as np
import os
import sys
directory = os.path.dirname(os.path.abspath(__file__)) + "/../"
sys.path.append(directory)
from CACG import*
from CDSE import*

prj_dir= '/home/pittzhou/Jinming/Pitts_CHARM/CACG/prj_try'
subprocess.run(['mkdir','-p' ,f'{prj_dir}'])

A=np.random.rand(4096, 4096).astype(np.float32)
B=np.random.rand(4096, 4096).astype(np.float32)

final_config=cdse_top(A,B)

gen_charm_top(prj_dir,final_config)
