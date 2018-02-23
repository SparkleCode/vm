cmake_minimum_required(VERSION 3.9)

project(sparklecode)

include_directories(src/include)
add_subdirectory(src)

add_executable(sparklecode src/main.c)
target_link_libraries(sparklecode sparklecodemain)
set_property(TARGET sparklecode PROPERTY C_STANDARD 99)