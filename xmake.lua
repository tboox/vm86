-- project
set_project("vm86")

-- version
set_version("1.0.1", {build = "%Y%m%d%H%M"})
set_xmakever("2.2.7")

-- set warning all as error
set_warnings("all", "error")

-- set language: c99, c++11
set_languages("c99", "cxx11")

-- disable some compiler errors
add_cxflags("-Wno-error=deprecated-declarations", "-fno-strict-aliasing", "-Wno-error=nullability-completeness")

-- set the default architecture, only support 32bits now
if is_host("windows") then
    set_config("arch", "x86")
elseif is_plat("iphoneos") then
    set_config("arch", "armv7")
else
    set_config("arch", "i386")
end

-- apply debug and release modes
add_rules("mode.debug", "mode.release")

-- for the windows platform (msvc)
if is_plat("windows") then 
    if is_mode("release") then
        add_cxflags("-MT") 
    elseif is_mode("debug") then
        add_cxflags("-MTd") 
    end
    add_syslinks("ws2_32") 
    add_ldflags("-nodefaultlib:msvcrt.lib")
elseif is_plat("android") then
    add_syslinks("m", "c") 
elseif is_plat("mingw") then
    add_syslinks("ws2_32", "pthread", "m")
else 
    add_syslinks("pthread", "dl", "m", "c") 
end

-- add option: demo
option("demo")
    set_default(true)
    set_showmenu(true)
    set_category("option")
    set_description("Enable or disable the demo module")

-- add requires
add_requires("tbox 1.6.3")

-- add projects
includes("src/vm86") 
if has_config("demo") then includes("src/demo") end
