add_library(
    GLOBAL_GEN 
    "global_generator/global_generator.h" 
    "global_generator/global_generator.cpp"
)

add_library(
    COMMUM_HEADER
    "commum_header/commum_header.h"
)

add_library(
    STREAM_MAKER
    "stream_maker/stream_maker.h"
    "stream_maker/stream_maker.cpp"
)

add_library(
    K_WISE_FAMILY
    "k_wise_family/k_wise_family.h"
    "k_wise_family/k_wise_family.cpp"    
)

add_library(
    COUNT_SKETCH
    "count_sketch/count_sketch.h"
    "count_sketch/count_sketch.cpp"
)

add_library(
    KLL 
    "kll/kll.hpp" 
    "kll/kll.cpp"
)

add_library(
    DCS
    "dcs/dcs.h"
    "dcs/dcs.cpp"
)

add_library(
    PBPLOTS_LIB
    "pbplots/pbPlots.hpp"
    "pbplots/pbPlots.cpp"
)

add_library(
    PBPLOTS_SUPPORT_LIB
    "pbplots/supportLib.hpp"
    "pbplots/supportLib.cpp"
)

add_library(
    PBPLOTS_PBPLOTTER
    "pbplots/pbPlotter.h"
    "pbplots/pbPlotter.cpp"
)

set_target_properties(GLOBAL_GEN PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(COMMUM_HEADER PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(STREAM_MAKER PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(K_WISE_FAMILY PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(COUNT_SKETCH PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(KLL PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(DCS PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PBPLOTS_LIB PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PBPLOTS_SUPPORT_LIB PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PBPLOTS_PBPLOTTER PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(COUNT_SKETCH PUBLIC K_WISE_FAMILY)
target_link_libraries(COUNT_SKETCH PUBLIC COMMUM_HEADER)
target_link_libraries(COUNT_SKETCH PUBLIC GLOBAL_GEN)

target_link_libraries(DCS PUBLIC COUNT_SKETCH)

target_link_libraries(PBPLOTS_PBPLOTTER PUBLIC PBPLOTS_LIB)
target_link_libraries(PBPLOTS_PBPLOTTER PUBLIC PBPLOTS_SUPPORT_LIB)