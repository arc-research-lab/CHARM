def gen_para(environment,h1,w1,w2,A,B,C,A_BRO,C_BRO,X,Y,Z,data_type,krl_folder):
    template = environment.get_template("dma.hpp")
    file_tmp = "dma.hpp"
    filename = krl_folder / file_tmp
    content = template.render(
        h1=h1,
        w1=w1,
        w2=w2,
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        X=X,
        Y=Y,
        Z=Z,
        data_type=data_type
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_ddr(environment, krl_folder):
    template = environment.get_template("ddr.cpp")
    file_tmp = "dma.cpp"
    filename = krl_folder / file_tmp
    content = template.render(
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")


def gen_send(environment,A,C,A_BRO,C_BRO,krl_folder):
    template = environment.get_template("send_receive.cpp")
    file_tmp = "dma.cpp"
    filename = krl_folder / file_tmp
    content = template.render(
        NUM_TXL=(C//A_BRO),
        NUM_TXR=(A//C_BRO)
    )
    with open(filename, mode="a", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_compute(environment,A,B,C,A_BRO,C_BRO,BUFF_SEL,krl_folder):
    template = environment.get_template("compute.cpp")
    file_tmp = "dma.cpp"
    filename = krl_folder / file_tmp
    content = template.render(
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        NUM_TXL=(C//A_BRO),
        NUM_TXR=(A//C_BRO),
        BUFF_SEL=BUFF_SEL
    )
    with open(filename, mode="a", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_krl_top(environment,A,B,C,A_BRO,C_BRO,krl_folder):
    template = environment.get_template("dma.cpp")
    file_tmp = "dma.cpp"
    filename = krl_folder / file_tmp
    content = template.render(
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        NUM_TXL=(C//A_BRO),
        NUM_TXR=(A//C_BRO)
    )
    with open(filename, mode="a", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_conn(environment,A,B,C,A_BRO,C_BRO,layer,prj_folder):
    template = environment.get_template("conn.cfg")
    file_tmp = "conn.cfg"
    filename = prj_folder / file_tmp
    content = template.render(
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        NUM_TXL=(C//A_BRO),
        NUM_TXR=(A//C_BRO),
        layer=layer
    )
    with open(filename, mode="a", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")