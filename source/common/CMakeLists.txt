add_library(common SHARED Hashing.c Object.c)
target_link_libraries(common PUBLIC sds)
