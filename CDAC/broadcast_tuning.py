import numpy as np
import math

def find_factor(num,factor_max):
    for i in range(factor_max,0,-1):
        if num%i==0:
            factor=i
            break
    return factor

def broadC_factor(a,b,c):
    if c>4:
        height=c
        if b>12:
            C_BRO=1
        elif b>5:
            C_BRO=find_factor(a,2)
        elif b>2:
            C_BRO=find_factor(a,4)
        else:
            C_BRO=a
    elif (c==4 or c==3):
        if c==4:
            height=8
        else:
            height=6
        if b>12:
            C_BRO=find_factor(a,2)
        elif b>5:
            C_BRO=find_factor(a,4)   
        elif b>2:
            C_BRO=find_factor(a,8)  
        else:
            C_BRO=a
    elif c==2:
        height=8
        if b>12:
            C_BRO=find_factor(a,4)
        elif b>5:
            C_BRO=find_factor(a,8) 
        elif b>2:
            C_BRO=find_factor(a,16) 
        else:
            C_BRO=a
    else:
        height=8
        if b>12:
            C_BRO=find_factor(a,8)
        elif b>5:
            C_BRO=find_factor(a,16) 
        elif b>2:
            C_BRO=find_factor(a,32) 
        else:
            C_BRO=a
    return C_BRO,height

def placement_verify(a,b,c,height):
    col_full=(a*c)//height
    col_left=(a*c)%height
    length=col_full*b + (col_left!=0)*b
    return length