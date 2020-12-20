add_library(
    KLL 
    "kll/kll.hpp" 
    "kll/kll.tpp"
)

add_library(
    GLOBAL_GEN 
    "global_generator/global_generator.h" 
    "global_generator/global_generator.cpp"
)

add_library(COMMUM_HEADER
    "commum_header/commum_header.h"
)

set_target_properties(KLL PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(GLOBAL_GEN PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(COMMUM_HEADER PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(KLL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GLOBAL_GEN PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(COMMUM_HEADER PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")