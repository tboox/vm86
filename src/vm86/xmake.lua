-- add target
target("vm86")

    -- make as a static library
    set_kind("static")

    -- add defines
    add_defines("__tb_prefix__=\"vm86\"")

    -- set the auto-generated config.h
    set_config_h("$(buildir)/vm86/vm86.config.h")
    set_config_h_prefix("VM86_CONFIG")

    -- set the object files directory
    set_objectdir("$(buildir)/.objs")

    -- add includes directory
    add_includedirs("$(buildir)")
    add_includedirs("$(buildir)/vm86")

    -- add the header files for installing
    add_headers("../(vm86/**.h)|**/impl/**.h")

    -- add packages
    add_packages("tbox", "base")

    -- add the common source files
    add_files("*.c") 

