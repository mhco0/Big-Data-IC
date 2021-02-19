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

add_library(
    PMQ
    "pmq/pmq.h"
    "pmq/pmq.cpp"
)

add_library(
    POINT
    "point/point.hpp"
    "point/point.cpp"
)

add_library(
    AABB
    "aabb/aabb.h"
    "aabb/aabb.cpp"
)

add_library(
    QUADTREE
    "quadtree/quadtree.h"
    "quadtree/quadtree.cpp"
)

add_library(
    QUANTILE_SKETCH
    "quantile_sketch/quantile_sketch.hpp"
)

add_library(
    QUANTILE_QUADTREE
    "quantile_quadtree/quantile_quadtree.hpp"
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
set_target_properties(PMQ PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(AABB PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(POINT PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(QUADTREE PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(QUANTILE_SKETCH PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(QUANTILE_QUADTREE PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(K_WISE_FAMILY PUBLIC GLOBAL_GEN)

target_link_libraries(STREAM_MAKER PUBLIC COMMUM_HEADER)
target_link_libraries(STREAM_MAKER PUBLIC GLOBAL_GEN)

target_link_libraries(COUNT_SKETCH PUBLIC QUANTILE_SKETCH)
target_link_libraries(COUNT_SKETCH PUBLIC K_WISE_FAMILY)
target_link_libraries(COUNT_SKETCH PUBLIC COMMUM_HEADER)
target_link_libraries(COUNT_SKETCH PUBLIC GLOBAL_GEN)

target_link_libraries(DCS PUBLIC COUNT_SKETCH)

target_link_libraries(PBPLOTS_PBPLOTTER PUBLIC PBPLOTS_LIB)
target_link_libraries(PBPLOTS_PBPLOTTER PUBLIC PBPLOTS_SUPPORT_LIB)

target_link_libraries(POINT PUBLIC COMMUM_HEADER)

target_link_libraries(AABB PUBLIC POINT)

target_link_libraries(QUADTREE PUBLIC AABB)

target_link_libraries(QUANTILE_QUADTREE PUBLIC QUANTILE_SKETCH)
target_link_libraries(QUANTILE_QUADTREE PUBLIC AABB)