#[[
    Flags used before:
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing
]]

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
set(QSBD_DIR_SOURCE "${CMAKE_SOURCE_DIR}/src")
set(THIRD_PARTY_DIR_SOURCE "${CMAKE_SOURCE_DIR}/thirdparty")


add_executable(coletor "coletor/coletor.cpp")
target_include_directories(coletor PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(coletor PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(coletor PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(comparator "comparator/comparator.cpp")
target_include_directories(comparator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(comparator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(comparator PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(profiler "profiler/profiler.cpp")
target_include_directories(profiler PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profiler PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profiler PRIVATE QSBD_QUANTILE_QUADTREE QSBD_GK_FACTORY QSBD_KLL_FACTORY QSBD_DCS_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_UTILS QSBD_LOG QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(build_streams "stream_creator/build_streams.cpp")
target_include_directories(build_streams PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(build_streams PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(build_streams PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(stream_creator "stream_creator/stream_creator.cpp")
target_include_directories(stream_creator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(stream_creator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(stream_creator PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_DEBUG)

add_executable(build_queries "query_creator/build_queries.cpp")
target_include_directories(build_queries PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(build_queries PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(build_queries PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(query_creator "query_creator/query_creator.cpp")
target_include_directories(query_creator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(query_creator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(query_creator PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_DEBUG)

add_executable(build_brute_force "brute_force/build_brute_force.cpp")
target_include_directories(build_brute_force PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(build_brute_force PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(build_brute_force PRIVATE QSBD_UTILS QSBD_DEBUG)

add_executable(brute_force "brute_force/brute_force.cpp")
target_include_directories(brute_force PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(brute_force PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(brute_force PRIVATE  nlohmann_json QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER QSBD_LOG QSBD_AABB)

add_executable(experiment_all "quantile_quadtree_experiment/experiment_all.cpp")
target_include_directories(experiment_all PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(experiment_all PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(experiment_all PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(quantile_quadtree_experiment "quantile_quadtree_experiment/quantile_quadtree_experiment.cpp")
target_include_directories(quantile_quadtree_experiment PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(quantile_quadtree_experiment PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(quantile_quadtree_experiment PRIVATE QSBD_TIMER QSBD_QUANTILE_QUADTREE QSBD_GK_FACTORY QSBD_KLL_FACTORY QSBD_DCS_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_UTILS QSBD_LOG QSBD_DEBUG QSBD_MEMORY_TRACKER)
