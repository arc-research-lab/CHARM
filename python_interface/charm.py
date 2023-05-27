import numpy as np
import os
import sys
directory = os.path.dirname(os.path.abspath(__file__)) + "/../"
sys.path.append(directory)
from CACG import*
from CDSE import*

class charm:
    def __init__(self, prj_dir='.'):  
        self.prj_dir = prj_dir  
        
    def cdse(self,A,B):
        final_config=cdse_top(A,B)
        return final_config
    
    def cacg(self,final_config):
        gen_charm_top(self.prj_dir,final_config)