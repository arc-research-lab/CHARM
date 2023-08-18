from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import numpy as np
import subprocess
import os
from .gen_make_func import*

def gen_make_top(prj_dir,template_dir,layer,device):
    make_dir = Path(prj_dir)
    ### Define template PATH
    environment = Environment(loader=FileSystemLoader(template_dir))
    gen_make(environment,layer,device,make_dir)