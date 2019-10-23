-- add target
target("vm86")

    -- make as a static library
    set_kind("static")

    -- add defines
    add_defines("__tb_prefix__=\"vm86\"")

    -- set the auto-generated config.h
    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("vm86.config.h.in")

    -- add includes directory
    add_includedirs("..", {public = true})
    add_includedirs("$(buildir)/$(plat)/$(arch)/$(mode)", {public = true})

    -- add the header files for installing
    add_headerfiles("../(vm86/**.h)|**/impl/**.h")
    add_headerfiles("$(buildir)/$(plat)/$(arch)/$(mode)/vm86.config.h", {prefixdir = "vm86"})

    -- add packages
    add_packages("tbox")

    -- add the common source files
    add_files("*.c") 

