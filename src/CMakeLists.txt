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
    UTILS
    "utils/utils.h"
    "utils/utils.cpp"
)

add_library(
    BD_DEBUG
    "bd_debug/bd_debug.h"   
)

add_library(
    BD_TIMER
    "bd_timer/bd_timer.h"
    "bd_timer/bd_timer.cpp"
)

add_library(
    STREAM_MAKER
    "stream_maker/stream_maker.h"
    "stream_maker/stream_maker.cpp"
)

add_library(
    CONFIG
    "config/config.h"
    "config/config.cpp"
)

add_library(
    MEMORY_TRACKER
    "memory_tracker/memory_tracker.h"
    "memory_tracker/memory_tracker.cpp"
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
    GK 
    "gk/gk.hpp"
)

add_library(
    Q_DIGEST
    "q_digest/q_digest.h"
    "q_digest/q_digest.cpp"
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

add_library(
    SKETCH_FACTORY
    "sketch_factory/sketch_factory.hpp"
)

add_library(
    KLL_FACTORY
    "kll_factory/kll_factory.hpp"
)

add_library(
    Q_DIGEST_FACTORY
    "q_digest_factory/q_digest_factory.h"
)

add_library(
    GK_FACTORY
    "gk_factory/gk_factory.hpp"
)

add_library(
    DCS_FACTORY
    "dcs_factory/dcs_factory.h"
)

set(BD_LIBS

    GLOBAL_GEN
    COMMUM_HEADER
    UTILS
    BD_DEBUG
    BD_TIMER
    STREAM_MAKER
    CONFIG
    MEMORY_TRACKER
    K_WISE_FAMILY
    COUNT_SKETCH
    QUANTILE_SKETCH
    SKETCH_FACTORY
    KLL
    KLL_FACTORY
    DCS
    DCS_FACTORY
    GK
    GK_FACTORY
    Q_DIGEST
    Q_DIGEST_FACTORY
    AABB
    POINT
    QUADTREE
    QUANTILE_QUADTREE
)


foreach(BD_LIB ${BD_LIBS})
    set_target_properties(${BD_LIB} PROPERTIES LINKER_LANGUAGE CXX)
endforeach()

target_link_libraries(K_WISE_FAMILY PUBLIC GLOBAL_GEN)

target_link_libraries(STREAM_MAKER PUBLIC COMMUM_HEADER)
target_link_libraries(STREAM_MAKER PUBLIC GLOBAL_GEN)

target_link_libraries(COUNT_SKETCH PUBLIC K_WISE_FAMILY)
target_link_libraries(COUNT_SKETCH PUBLIC COMMUM_HEADER)
target_link_libraries(COUNT_SKETCH PUBLIC GLOBAL_GEN)

target_link_libraries(KLL PUBLIC COMMUM_HEADER)
target_link_libraries(KLL PUBLIC GLOBAL_GEN)

target_link_libraries(DCS PUBLIC QUANTILE_SKETCH)
target_link_libraries(DCS PUBLIC COMMUM_HEADER)
target_link_libraries(DCS PUBLIC GLOBAL_GEN)
target_link_libraries(DCS PUBLIC COUNT_SKETCH)

target_link_libraries(GK PUBLIC QUANTILE_SKETCH)
target_link_libraries(GK PUBLIC COMMUM_HEADER)
target_link_libraries(GK PUBLIC GLOBAL_GEN)

target_link_libraries(Q_DIGEST PUBLIC QUANTILE_SKETCH)
target_link_libraries(Q_DIGEST PUBLIC COMMUM_HEADER)
target_link_libraries(Q_DIGEST PUBLIC GLOBAL_GEN)

target_link_libraries(POINT PUBLIC COMMUM_HEADER)

target_link_libraries(AABB PUBLIC POINT)

target_link_libraries(QUADTREE PUBLIC AABB)

target_link_libraries(QUANTILE_QUADTREE PUBLIC QUANTILE_SKETCH)
target_link_libraries(QUANTILE_QUADTREE PUBLIC AABB)

target_link_libraries(KLL_FACTORY PUBLIC SKETCH_FACTORY)
target_link_libraries(KLL_FACTORY PUBLIC KLL)

target_link_libraries(Q_DIGEST_FACTORY PUBLIC SKETCH_FACTORY)
target_link_libraries(Q_DIGEST_FACTORY PUBLIC Q_DIGEST)

target_link_libraries(GK_FACTORY PUBLIC SKETCH_FACTORY)
target_link_libraries(GK_FACTORY PUBLIC GK)

target_link_libraries(DCS_FACTORY PUBLIC SKETCH_FACTORY)
target_link_libraries(DCS_FACTORY PUBLIC DCS)