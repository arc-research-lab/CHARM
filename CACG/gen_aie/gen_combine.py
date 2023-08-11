def gen_combine(environment,port_width,freq,L_list,HW_Conf,Port_Conf_All,Port_Conf_Pre,port_total,aie_folder):
    template = environment.get_template("aie_top.cpp")
    file_tmp = "aie_top_all.cpp"
    filename = aie_folder / file_tmp
    content = template.render(
        port_width=port_width,
        freq=freq,
        L_list=L_list,
        HW_Conf=HW_Conf,
        Port_Conf=Port_Conf_All,
        Port_Conf_Pre=Port_Conf_Pre,
        port_total=port_total
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")