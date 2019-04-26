cmake_minimum_required(VERSION 3.9)

project(sparklecode)

include_directories(src/include)

add_executable(sparklecode
    src/chunk.c 
    src/memory.c
    src/debug.c
    src/value.c
    src/vm.c
    src/compiler.c
    src/scanner.c
    src/main.c)
set_property(TARGET sparklecode PROPERTY C_STANDARD 99)