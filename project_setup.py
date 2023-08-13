import os
import sys
import subprocess
import numpy as np
cur_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(cur_dir)
from charm import* 

#Set the design project path
prj_dir= cur_dir + '/prj_try'
subprocess.run(['mkdir','-p' ,f'{prj_dir}'])

#Define the left-hand-side(A) and right-hide-side(B) operands
A=np.random.rand(8192, 8192).astype(np.float32)
B=np.random.rand(8192, 8192).astype(np.float32)

#Create the object of the class charm
automm=charm(prj_dir)

#Launch charm dse to find optimized hardware configuration
Versal_config=automm.cdse(A,B)

#Launch charm automatic code generator to emit the code for AIE, PL and Host CPU
device='vck190' # Supported devices are vck190 and vck5000
automm.cacg(Versal_config,device)

# #Run Vitis Compilation Flow
# automm.build()