def gen_para(environment,h1,w1,w2,A,B,C,A_BRO,C_BRO,layer,aie_folder):
    template = environment.get_template("para.h")
    file_tmp = "para_L" + str(layer) + ".h"
    filename = aie_folder / file_tmp
    content = template.render(
        h1=h1,
        w1=w1,
        w2=w2,
        A=A,
        B=B,
        C=C,
        A_BRO=A_BRO,
        C_BRO=C_BRO,
        layer=layer
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

def gen_krnl(environment,w1,kernel_type,layer,aie_folder):
    for i in range(4):
        if kernel_type==3:
            bound_k=int(w1/2)-1
        else:
            bound_k=int(w1/16)-1
        kernel_name="mm" + str(kernel_type) + "_kernel" + str(i) + ".cc"
        template = environment.get_template(kernel_name)
        file_tmp="mm" + str(kernel_type) + "_kernel" + str(i) + "_L" + str(layer) + ".cc"
        filename = aie_folder / file_tmp
        content = template.render(
            bound_k=bound_k,
            layer=layer
        )
        with open(filename, mode="w", encoding="utf-8") as message:
            message.write(content)
            print(f"... wrote {filename}")

def gen_grah(environment,B,layer,aie_folder):
    graph_name= "aie_graph.h"
    template = environment.get_template(graph_name)
    file_tmp = "aie_graph_L" + str(layer) + ".h"
    filename = aie_folder / file_tmp
    content = template.render(
        B=B,
        layer=layer
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")

