-- add target
target("demo")

    -- add the dependent target
    add_deps("vm86")

    -- make as a binary
    set_kind("binary")

    -- add defines
    add_defines("__tb_prefix__=\"vm86\"")

    -- set the object files directory
    set_objectdir("$(buildir)/.objs")

    -- add links directory
    add_linkdirs("$(buildir)")

    -- add includes directory
    add_includedirs("$(buildir)")
    add_includedirs("$(buildir)/vm86")

    -- add links
    add_links("vm86")

    -- add packages
    add_packages("tbox", "base")

    -- add the source files
    add_files("*.c") 

