import json
import numpy as np

def find_col_out(col_sel,chl_cnt,reverse,num_channel):
    """This is a recursive function
    to find the available PLIO ports"""
    if col_sel>44 or col_sel<6:
        return 0,0
    cnt_pos=col_sel-6
    chl_num=chl_cnt[cnt_pos]
    if chl_num < num_channel:
        return col_sel,chl_num
    else:
        col_sel=col_sel+reverse
        return (find_col_out(col_sel,chl_cnt,reverse,num_channel))

def find_col_in(col_sel,chl_cnt,chl_lhs_flag,reverse,op):#op: 0->lhs, 1->rhs
    """This is a recursive function
    to find the available PLIO ports"""
    if col_sel>44 or col_sel<6:
        return 0,0
    cnt_pos=col_sel-6
    chl_num=chl_cnt[cnt_pos]
    lhs_flag=chl_lhs_flag[cnt_pos]
    if lhs_flag==0:
        if op==0:
            num_channel=2
        else:
            num_channel=3
    else:
        if op==0:
            num_channel=2
        else:
            num_channel=2

    if chl_num < num_channel:
        return col_sel,chl_num
    else:
        col_sel=col_sel+reverse
        return (find_col_in(col_sel,chl_cnt,chl_lhs_flag,reverse,op))

def plio_placement(HW_Config,layer):
    dictionary = dict()
    dictionary['NodeConstraints'] = dict()

    A=HW_Config[0]
    B=HW_Config[1]
    C=HW_Config[2]
    A_BRO=HW_Config[3]
    C_BRO=HW_Config[4]
    PACK_IN=HW_Config[5]
    PACK_OUT=HW_Config[6]
    pos_col=HW_Config[7]
    pos_row=HW_Config[8]
    height=HW_Config[9]
    
    c_a_bro=C//A_BRO
    chl_cnt_in=np.zeros((39))
    chl_lhs_flag=np.zeros((39))
    chl_cnt_out=np.zeros((39))
    #num_chl_in=3
    num_chl_out=2
    #LHS PLIO Placement
    for i in range(A):
        for j in range(C//A_BRO):
            for k in range((B//PACK_IN)):
                port_num = k+j*(B//PACK_IN)+i*(C//A_BRO)*(B//PACK_IN)
                port_name='LHS_in' + str(port_num) +'_L' + str(layer)
                dictionary['NodeConstraints'][port_name] = dict()
                dictionary['NodeConstraints'][port_name]['shim'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['column'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = dict()
                if c_a_bro%2==0 and height%2==0:
                    if (j//(c_a_bro/2))==1:
                        if pos_row%2==0:
                            reverse=-1
                            offset=1
                        else:
                            reverse=1
                            offset=0
                    else:
                        if pos_row%2==0:
                            reverse=1
                            offset=0
                        else:
                            reverse=-1
                            offset=1    
                    cur_col=pos_col+reverse*k*PACK_IN+(j%(c_a_bro/2))*B+offset*(B-1)+i*B*(C//height)
                else:
                    reverse=1
                    cur_col=pos_col+k*PACK_IN+j*B+i*B*(C//height)
                col_sel=np.clip(cur_col, 6, 44).astype(int)#PLIOs lie in col 6-44
                col_sel_new,chl_num=find_col_in(col_sel,chl_cnt_in,chl_lhs_flag,reverse,0)
                if col_sel_new==0:
                    reverse=-reverse
                    col_sel_new,chl_num=find_col_in(col_sel,chl_cnt_in,chl_lhs_flag,reverse,0)
                    if col_sel_new==0:
                        print('\n\n\nLHS PLIO failed to place\n\n\n')
                        break
                chl_sel=((chl_num+1)*2)%8 #0,1,2,3 -->2,4,6,0
                dictionary['NodeConstraints'][port_name]['shim']['column'] = int(col_sel_new)
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = int(chl_sel)
                chl_cnt_in[col_sel_new-6]=chl_cnt_in[col_sel_new-6]+1
                chl_lhs_flag[col_sel_new-6]=1

    #RHS PLIO Placement
    for j in range(C):
        for i in range(A//C_BRO):
            for k in range((B//PACK_IN)):
                port_num = k+i*(B//PACK_IN)+j*(A//C_BRO)*(B//PACK_IN)
                port_name='RHS_in' + str(port_num) +'_L' + str(layer)
                dictionary['NodeConstraints'][port_name] = dict()
                dictionary['NodeConstraints'][port_name]['shim'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['column'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = dict()
                if c_a_bro%2==0 and height%2==0:
                    if pos_row%2==0:
                        if j//(C//2)==0:
                            reverse=1
                            offset=0
                        else:
                            reverse=-1
                            offset=1
                    else:
                        if j//(C//2)==0:
                            reverse=-1
                            offset=1
                        else:
                            reverse=1
                            offset=0
                else:
                    if ((j+pos_row)%2)==0:
                        reverse=1
                        offset=0
                    else:
                        reverse=-1
                        offset=1
                cur_col=pos_col+reverse*k*PACK_IN+i*B*(C//height)*C_BRO+offset*(B*(C//height)*C_BRO-1)
                col_sel=np.clip(cur_col, 6, 44).astype(int)#PLIOs lie in col 6-44
                col_sel_new,chl_num=find_col_in(col_sel,chl_cnt_in,chl_lhs_flag,reverse,1)
                if col_sel_new==0:
                    reverse=-reverse
                    col_sel_new,chl_num=find_col_in(col_sel,chl_cnt_in,chl_lhs_flag,reverse,1)
                    if col_sel_new==0:
                        print('\n\n\nRHS PLIO failed to place\n\n\n')
                        break
                chl_sel=((chl_num+1)*2)%8 #0,1,2,3 -->2,4,6,0
                dictionary['NodeConstraints'][port_name]['shim']['column'] = int(col_sel_new)
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = int(chl_sel)
                chl_cnt_in[col_sel_new-6]=chl_cnt_in[col_sel_new-6]+1

    #Out PLIO Placement
    if  height%PACK_OUT==0:
        for i in range(A):
            for j in range(C//PACK_OUT):
                port_num = j+i*(C//PACK_OUT)
                port_name='out' + str(port_num) +'_L' + str(layer)
                dictionary['NodeConstraints'][port_name] = dict()
                dictionary['NodeConstraints'][port_name]['shim'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['column'] = dict()
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = dict()
                if c_a_bro%2==0 and height%2==0:
                    if (C//PACK_OUT)==1:
                        if pos_row%2==0:
                            reverse=1
                            offset=1
                        else:
                            reverse=-1
                            offset=0
                    elif (j//((C//PACK_OUT)/2))==0:
                        if pos_row%2==0:
                            reverse=1
                            offset=1
                        else:
                            reverse=-1
                            offset=0
                    else:
                        if pos_row%2==0:
                            reverse=-1
                            offset=0
                        else:
                            reverse=1
                            offset=1
                    cur_col=pos_col+offset*(B-1)+i*B*(C//height)
                else:
                    reverse=1
                    cur_col=pos_col+i*B*(C//height)
                col_sel=np.clip(cur_col, 6, 44).astype(int)#PLIOs lie in col 6-44
                col_sel_new,chl_num=find_col_out(col_sel,chl_cnt_out,reverse,num_chl_out)
                if col_sel_new==0:
                    reverse=-reverse
                    col_sel_new,chl_num=find_col_out(col_sel,chl_cnt_out,reverse,num_chl_out)
                    if col_sel_new==0:
                        print('\n\n\nOUT PLIO failed to place\n\n\n')
                        break
                chl_sel=((chl_num+1)*2)%8 #0,1,2,3 -->2,4,6,0
                dictionary['NodeConstraints'][port_name]['shim']['column'] = int(col_sel_new)
                dictionary['NodeConstraints'][port_name]['shim']['channel'] = int(chl_sel)
                chl_cnt_out[col_sel_new-6]=chl_cnt_out[col_sel_new-6]+1


    json_object=json.dumps(dictionary,indent=4)
    return json_object



        