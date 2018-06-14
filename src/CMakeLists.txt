project(sparklecodemain)

add_library(sparklecodemain chunk.c memory.c debug.c value.c vm.c compiler.c scanner.c)

set_property(TARGET sparklecodemain PROPERTY C_STANDARD 99)