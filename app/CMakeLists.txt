#[[
    Flags used before:
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing
    -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing

    -pg
-pg
-pg 
]]

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g  -O0 -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -O0 -Wall")
set(QSBD_DIR_SOURCE "${CMAKE_SOURCE_DIR}/src")
set(THIRD_PARTY_DIR_SOURCE "${CMAKE_SOURCE_DIR}/thirdparty")


add_executable(coletor "coletor/coletor.cpp")
target_include_directories(coletor PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(coletor PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(coletor PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(data_points "coletor/data_points.cpp")
target_include_directories(data_points PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(data_points PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(data_points PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(comparator "comparator/comparator.cpp")
target_include_directories(comparator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(comparator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(comparator PRIVATE nlohmann_json QSBD_UTILS QSBD_DEBUG)

add_executable(profiler "profiler/profiler.cpp")
target_include_directories(profiler PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profiler PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profiler PRIVATE QSBD_QUANTILE_QUADTREE QSBD_GK_FACTORY QSBD_KLL_FACTORY QSBD_DCS_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_UTILS QSBD_LOG QSBD_DEBUG QSBD_STREAM_MAKER)


add_executable(profile_count_sketch "profiler/profile_count_sketch.cpp")
target_include_directories(profile_count_sketch PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profile_count_sketch PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profile_count_sketch PRIVATE  QSBD_COUNT_SKETCH QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(profile_q_digest "profiler/profile_q_digest.cpp")
target_include_directories(profile_q_digest PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profile_q_digest PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profile_q_digest PRIVATE  QSBD_Q_DIGEST QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(profile_dcs "profiler/profile_dcs.cpp")
target_include_directories(profile_dcs PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profile_dcs PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profile_dcs PRIVATE  QSBD_DCS QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(profile_kll "profiler/profile_kll.cpp")
target_include_directories(profile_kll PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profile_kll PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profile_kll PRIVATE  QSBD_KLL QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(profile_gk "profiler/profile_gk.cpp")
target_include_directories(profile_gk PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(profile_gk PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(profile_gk PRIVATE  QSBD_GK QSBD_UTILS QSBD_DEBUG QSBD_STREAM_MAKER)

add_executable(convert_stream "convert_stream/convert_stream.cpp")
target_include_directories(convert_stream PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(convert_stream PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(convert_stream PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_DEBUG)

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

add_executable(new_quantile_quadtree_experiment "quantile_quadtree_experiment/new_quantile_quadtree_experiment.cpp")
target_include_directories(new_quantile_quadtree_experiment PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(new_quantile_quadtree_experiment PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(new_quantile_quadtree_experiment PRIVATE QSBD_TIMER QSBD_QUANTILE_QUADTREE QSBD_GK_FACTORY QSBD_KLL_FACTORY QSBD_DCS_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_UTILS QSBD_LOG QSBD_DEBUG)

add_executable(quadtree_data_bounds "quantile_quadtree_experiment/quadtree_data_bounds.cpp")
target_include_directories(quadtree_data_bounds PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(quadtree_data_bounds PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(quadtree_data_bounds PRIVATE QSBD_AABB QSBD_UTILS QSBD_DEBUG)

add_executable(ks "ks/ks.cpp")
target_include_directories(ks PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(ks PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(ks PRIVATE QSBD_KS QSBD_KLL QSBD_DCS QSBD_GK QSBD_Q_DIGEST)