def gen_host(environment,device,Host_cfg,data_type_str,num_layer,host_folder):
    template = environment.get_template("host.cpp")
    file_tmp = "host.cpp"
    filename = host_folder / file_tmp
    if data_type_str=='float':
        format='%f'
    else:
        format='%d'
    content = template.render(
        device=device,
        Host_cfg=Host_cfg,
        data_type=data_type_str,
        format=format,
        num_layer=num_layer
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")