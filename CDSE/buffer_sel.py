import numpy as np
import math


def buff_count_0(BRAM,URAM,PART_A,PART_B,PART_C,PACK_IN,PACK_OUT,LEFT_SIZE,RIGHT_SIZE,OUT_SIZE,a,b,c,x,y,z,left_buf,right_buf,out_buf,RAM_TYPE_A,RAM_TYPE_B,RAM_TYPE_C,DATA_TYPE,force_assign,index_assign=0):
    if DATA_TYPE==1:
        fact=2
    else:
        fact=1
    if RAM_TYPE_A==1:
        bank_Lb=(a*math.ceil(b/PACK_IN))*(PART_A)*math.ceil((128/fact)/72)*math.ceil(x*y*LEFT_SIZE*PACK_IN/PART_A/512)
        bank_Lu=(a*math.ceil(b/PACK_IN))*(PART_A)*math.ceil((128/fact)/72)*math.ceil(x*y*LEFT_SIZE*PACK_IN/PART_A/4096)
    elif RAM_TYPE_A==2:
        bank_Lb=(a*math.ceil(b/PACK_IN))*(PART_A)*math.ceil((128/fact)/36)*math.ceil(x*y*LEFT_SIZE*PACK_IN/PART_A/1024)
        bank_Lu=(a*math.ceil(b/PACK_IN))*(PART_A)*math.ceil((128/fact)/72)*math.ceil(x*y*LEFT_SIZE*PACK_IN/PART_A/4096)

    if RAM_TYPE_B==1:
        bank_Rb=(c*math.ceil(b/PACK_IN))*(PART_B)*math.ceil(128/72)*math.ceil(y*z*RIGHT_SIZE*PACK_IN/PART_B/512)
        bank_Ru=(c*math.ceil(b/PACK_IN))*(PART_B)*math.ceil(128/72)*math.ceil(y*z*RIGHT_SIZE*PACK_IN/PART_B/4096)
    elif RAM_TYPE_B==2:
        bank_Rb=(c*math.ceil(b/PACK_IN))*(PART_B)*math.ceil(128/36)*math.ceil(y*z*RIGHT_SIZE*PACK_IN/PART_B/1024)
        bank_Ru=(c*math.ceil(b/PACK_IN))*(PART_B)*math.ceil(128/72)*math.ceil(y*z*RIGHT_SIZE*PACK_IN/PART_B/4096)

    if RAM_TYPE_C==1:
        bank_Ob=(a*math.ceil(c/PACK_OUT))*(PART_C)*math.ceil((128/fact)/72)*math.ceil(x*z*OUT_SIZE*PACK_OUT/PART_C/512)
        bank_Ou=(a*math.ceil(c/PACK_OUT))*(PART_C)*math.ceil((128/fact)/72)*math.ceil(x*z*OUT_SIZE*PACK_OUT/PART_C/4096)
    elif RAM_TYPE_C==2:
        bank_Ob=(a*math.ceil(c/PACK_OUT))*(PART_C)*math.ceil((128/fact)/36)*math.ceil(x*z*OUT_SIZE*PACK_OUT/PART_C/1024)
        bank_Ou=(a*math.ceil(c/PACK_OUT))*(PART_C)*math.ceil((128/fact)/72)*math.ceil(x*z*OUT_SIZE*PACK_OUT/PART_C/4096)

    on_chip_bram= np.zeros([8])
    on_chip_uram= np.zeros([8])
    
    on_chip_bram[0]=math.ceil(bank_Lb*left_buf+bank_Rb*right_buf+bank_Ob*out_buf)# [0,0,0]
    on_chip_uram[0]=0

    on_chip_bram[1]=math.ceil(bank_Lb*left_buf+bank_Rb*right_buf) # [0,0,1]
    on_chip_uram[1]=math.ceil(bank_Ou*out_buf)

    on_chip_bram[2]=math.ceil(bank_Lb*left_buf+bank_Ob*out_buf)  # [0,1,0]
    on_chip_uram[2]=math.ceil(bank_Ru*right_buf)

    on_chip_bram[3]=math.ceil(bank_Lb*left_buf)    # [0,1,1]
    on_chip_uram[3]=math.ceil(bank_Ru*right_buf+bank_Ou*out_buf)

    on_chip_bram[4]=math.ceil(bank_Rb*right_buf+bank_Ob*out_buf)  # [1,0,0]
    on_chip_uram[4]=math.ceil(bank_Lu*left_buf)

    on_chip_bram[5]=math.ceil(bank_Rb*right_buf)   # [1,0,1]
    on_chip_uram[5]=math.ceil(bank_Lu*left_buf+bank_Ou*out_buf)

    on_chip_bram[6]=math.ceil(bank_Ob*out_buf)     # [1,1,0]
    on_chip_uram[6]=math.ceil(bank_Lu*left_buf+bank_Ru*right_buf)

    on_chip_bram[7]=0 # [1,1,1]
    on_chip_uram[7]=math.ceil(bank_Lu*left_buf+bank_Ru*right_buf+bank_Ou*out_buf)

    on_chip_flag=np.logical_and((on_chip_bram<=BRAM),(on_chip_uram<=URAM))
    on_chip_total=on_chip_bram+on_chip_uram*8+(1-on_chip_flag)*5000
    if force_assign==1:
        buf_index=index_assign
    else:    
        buf_index=np.argmin(on_chip_total)
    bram_use=on_chip_bram[buf_index]
    uram_use=on_chip_uram[buf_index]

    return bram_use,uram_use,buf_index