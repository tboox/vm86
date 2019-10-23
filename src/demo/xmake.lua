-- add target
target("demo")

    -- add the dependent target
    add_deps("vm86")

    -- make as a binary
    set_kind("binary")

    -- add defines
    add_defines("__tb_prefix__=\"vm86\"")

    -- add packages
    add_packages("tbox")

    -- add the source files
    add_files("*.c") 

