def gen_make(environment,num_layer,device,prj_dir):
    file_name = 'Makefile_' + device
    template = environment.get_template(file_name)
    file_tmp = "Makefile"
    filename = prj_dir / file_tmp

    content = template.render(
        num_layer=num_layer
    )
    with open(filename, mode="w", encoding="utf-8") as message:
        message.write(content)
        print(f"... wrote {filename}")