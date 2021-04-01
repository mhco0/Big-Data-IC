add_library(
    QSBD_GLOBAL_GEN 
    "global_generator/global_generator.h" 
    "global_generator/global_generator.cpp"
)

add_library(
    QSBD_COMMUM_HEADER
    "commum_header/commum_header.h"
)

add_library(
    QSBD_UTILS
    "utils/utils.h"
    "utils/utils.cpp"
)

add_library(
    QSBD_DEBUG
    "qsbd_debug/qsbd_debug.h"   
)

add_library(
    QSBD_TIMER
    "timer/timer.h"
    "timer/timer.cpp"
)

add_library(
    QSBD_STREAM_MAKER
    "stream_maker/stream_maker.h"
    "stream_maker/stream_maker.cpp"
)

add_library(
    QSBD_CONFIG
    "config/config.h"
    "config/config.cpp"
)

add_library(
    QSBD_MEMORY_TRACKER
    "memory_tracker/memory_tracker.h"
    "memory_tracker/memory_tracker.cpp"
)

add_library(
    QSBD_K_WISE_FAMILY
    "k_wise_family/k_wise_family.h"
    "k_wise_family/k_wise_family.cpp"    
)

add_library(
    QSBD_COUNT_SKETCH
    "count_sketch/count_sketch.h"
    "count_sketch/count_sketch.cpp"
)

add_library(
    QSBD_KLL 
    "kll/kll.hpp" 
    "kll/kll.cpp"
)

add_library(
    QSBD_DCS
    "dcs/dcs.h"
    "dcs/dcs.cpp"
)

add_library(
    QSBD_GK 
    "gk/gk.hpp"
)

add_library(
    QSBD_Q_DIGEST
    "q_digest/q_digest.h"
    "q_digest/q_digest.cpp"
)

add_library(
    QSBD_POINT
    "point/point.hpp"
    "point/point.cpp"
)

add_library(
    QSBD_AABB
    "aabb/aabb.h"
    "aabb/aabb.cpp"
)

add_library(
    QSBD_QUADTREE
    "quadtree/quadtree.h"
    "quadtree/quadtree.cpp"
)

add_library(
    QSBD_QUANTILE_SKETCH
    "quantile_sketch/quantile_sketch.hpp"
)

add_library(
    QSBD_QUANTILE_QUADTREE
    "quantile_quadtree/quantile_quadtree.hpp"
)

add_library(
    QSBD_SKETCH_FACTORY
    "sketch_factory/sketch_factory.hpp"
)

add_library(
    QSBD_KLL_FACTORY
    "kll_factory/kll_factory.hpp"
)

add_library(
    QSBD_Q_DIGEST_FACTORY
    "q_digest_factory/q_digest_factory.h"
)

add_library(
    QSBD_GK_FACTORY
    "gk_factory/gk_factory.hpp"
)

add_library(
    QSBD_DCS_FACTORY
    "dcs_factory/dcs_factory.h"
)

add_library(
    QSBD_LOG
    "logger/logger.h"
    "logger/logger.cpp"
)

set(QSBD_LIBS

    QSBD_GLOBAL_GEN
    QSBD_COMMUM_HEADER
    QSBD_UTILS
    QSBD_DEBUG
    QSBD_TIMER
    QSBD_STREAM_MAKER
    QSBD_CONFIG
    QSBD_MEMORY_TRACKER
    QSBD_K_WISE_FAMILY
    QSBD_COUNT_SKETCH
    QSBD_QUANTILE_SKETCH
    QSBD_SKETCH_FACTORY
    QSBD_KLL
    QSBD_KLL_FACTORY
    QSBD_DCS
    QSBD_DCS_FACTORY
    QSBD_GK
    QSBD_GK_FACTORY
    QSBD_Q_DIGEST
    QSBD_Q_DIGEST_FACTORY
    QSBD_AABB
    QSBD_POINT
    QSBD_QUADTREE
    QSBD_QUANTILE_QUADTREE
    QSBD_LOG
)


foreach(QSBD_LIB ${QSBD_LIBS})
    set_target_properties(${QSBD_LIB} PROPERTIES LINKER_LANGUAGE CXX)
endforeach()

target_link_libraries(QSBD_K_WISE_FAMILY PUBLIC QSBD_GLOBAL_GEN)
target_link_libraries(QSBD_K_WISE_FAMILY PUBLIC QSBD_UTILS)

target_link_libraries(QSBD_STREAM_MAKER PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_STREAM_MAKER PUBLIC QSBD_GLOBAL_GEN)

target_link_libraries(QSBD_COUNT_SKETCH PUBLIC QSBD_K_WISE_FAMILY)
target_link_libraries(QSBD_COUNT_SKETCH PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_COUNT_SKETCH PUBLIC QSBD_GLOBAL_GEN)

target_link_libraries(QSBD_KLL PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_KLL PUBLIC QSBD_GLOBAL_GEN)

target_link_libraries(QSBD_DCS PUBLIC QSBD_QUANTILE_SKETCH)
target_link_libraries(QSBD_DCS PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_DCS PUBLIC QSBD_GLOBAL_GEN)
target_link_libraries(QSBD_DCS PUBLIC QSBD_COUNT_SKETCH)

target_link_libraries(QSBD_GK PUBLIC QSBD_QUANTILE_SKETCH)
target_link_libraries(QSBD_GK PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_GK PUBLIC QSBD_GLOBAL_GEN)
target_link_libraries(QSBD_GK PUBLIC QSBD_DEBUG)

target_link_libraries(QSBD_Q_DIGEST PUBLIC QSBD_QUANTILE_SKETCH)
target_link_libraries(QSBD_Q_DIGEST PUBLIC QSBD_COMMUM_HEADER)
target_link_libraries(QSBD_Q_DIGEST PUBLIC QSBD_GLOBAL_GEN)

target_link_libraries(QSBD_POINT PUBLIC QSBD_COMMUM_HEADER)

target_link_libraries(QSBD_AABB PUBLIC QSBD_POINT)

target_link_libraries(QSBD_QUADTREE PUBLIC QSBD_AABB)

target_link_libraries(QSBD_QUANTILE_QUADTREE PUBLIC QSBD_QUANTILE_SKETCH)
target_link_libraries(QSBD_QUANTILE_QUADTREE PUBLIC QSBD_AABB)

target_link_libraries(QSBD_KLL_FACTORY PUBLIC QSBD_SKETCH_FACTORY)
target_link_libraries(QSBD_KLL_FACTORY PUBLIC QSBD_KLL)

target_link_libraries(QSBD_Q_DIGEST_FACTORY PUBLIC QSBD_SKETCH_FACTORY)
target_link_libraries(QSBD_Q_DIGEST_FACTORY PUBLIC QSBD_Q_DIGEST)

target_link_libraries(QSBD_GK_FACTORY PUBLIC QSBD_SKETCH_FACTORY)
target_link_libraries(QSBD_GK_FACTORY PUBLIC QSBD_GK)

target_link_libraries(QSBD_DCS_FACTORY PUBLIC QSBD_SKETCH_FACTORY)
target_link_libraries(QSBD_DCS_FACTORY PUBLIC QSBD_DCS)

target_link_libraries(QSBD_LOG PUBLIC QSBD_DEBUG)

target_link_libraries(QSBD_MEMORY_TRACKER PUBLIC nlohmann_json)