import os
import sys
import subprocess
import numpy as np
cur_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(cur_dir)
from charm import* 

#Set the design project path
prj_dir= cur_dir + '/prj_try'

#Define the left-hand-side(A) and right-hide-side(B) operands

A=np.random.rand(3072, 1024).astype(np.float32)
B=np.random.rand(1024, 1024).astype(np.float32)

#Create the object of the class charm
automm=charm(prj_dir)

#Launch charm dse to find optimized hardware configuration
Versal_config=automm.cdse(A,B)
print(Versal_config)
