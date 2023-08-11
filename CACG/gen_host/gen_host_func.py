def gen_host(environment,device,h1,w1,w2,A,B,C,A_BRO,C_BRO,PACK_IN,PACK_OUT,X,Y,Z,data_type_str,host_folder):
    template = environment.get_template("host.cpp")
    file_tmp = "host.cpp"
    filename = host_folder / file_tmp
    content = template.render(
        device=device,
        h1=h1,
        w1=w1,
        w2=w2,
        A=A,
        B=B,
        C=C,
        PACK_IN=PACK_IN,
        PACK_OUT=PACK_OUT,
        X=X,
        Y=Y,
        Z=Z,
        data_type=data_type_str,
        NUM_TXL=(C//A_BRO),
        NUM_TXR=(A//C_BRO)
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")