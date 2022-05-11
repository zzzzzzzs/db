add_rules("mode.debug", "mode.release")
-- 优化级别
-- set_optimize("faster")
-- 设置c代码标准：c99， c++代码标准：c++11
set_languages("c11", "cxx11")

toolchain("gcc")
    set_kind("standalone")
toolchain_end()

target("db")
    set_kind("binary")
    add_includedirs("./inc/")
    add_files("src/*.c")
    
