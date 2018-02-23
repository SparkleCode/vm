project(sparklecodemain)

add_library(sparklecodemain chunk.c memory.c debug.c)

set_property(TARGET sparklecodemain PROPERTY C_STANDARD 99)