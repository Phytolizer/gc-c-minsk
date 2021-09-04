add_library(common SHARED Object.c VariableStore.c)
target_link_libraries(common PUBLIC sds)
