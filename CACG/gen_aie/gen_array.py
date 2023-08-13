def gen_toph(environment,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,pos_col,pos_row,height,layer,aie_folder):
    template = environment.get_template("aie_top.h")
    file_tmp = "aie_top_L"+ str(layer) + ".h"
    filename = aie_folder / file_tmp
    content = template.render(
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        PACK_IN=PACK_IN,
        PACK_OUT=PACK_OUT,
        layer=layer,
        pos_col=pos_col,
        pos_row=pos_row,
        height=height
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_topc(environment,A,B,C,A_BRO,C_BRO,layer,port_width,freq,aie_folder):
    template = environment.get_template("aie_top.cpp")
    filename = aie_folder / f"aie_top.cpp"
    content = template.render(
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        layer=layer,
        port_width=port_width,
        freq=freq
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")