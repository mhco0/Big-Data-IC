
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -fno-omit-frame-pointer -g -O2 -ggdb -fstrict-aliasing")
set(QSBD_DIR_SOURCE "${CMAKE_SOURCE_DIR}/src")

add_executable(test_stream_maker test_stream_maker.cpp)
target_link_libraries(test_stream_maker PRIVATE gtest_main QSBD_STREAM_MAKER QSBD_GLOBAL_GEN QSBD_COMMUM_HEADER QSBD_UTILS)
target_include_directories(test_stream_maker PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_stream_maker COMMAND test_stream_maker)

add_executable(test_memory_tracker test_memory_tracker.cpp)
target_link_libraries(test_memory_tracker PRIVATE gtest_main QSBD_MEMORY_TRACKER QSBD_UTILS)
target_include_directories(test_memory_tracker PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_memory_tracker COMMAND test_memory_tracker)

add_executable(test_config test_config.cpp)
target_link_libraries(test_config PRIVATE gtest_main QSBD_CONFIG QSBD_DEBUG QSBD_UTILS)
target_include_directories(test_config PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_config COMMAND test_config)

add_executable(test_kll test_kll.cpp)
target_link_libraries(test_kll PRIVATE gtest_main QSBD_KLL QSBD_STREAM_MAKER QSBD_KLL_FACTORY QSBD_UTILS)
target_include_directories(test_kll PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_kll COMMAND test_kll)

add_executable(test_k_wise_family test_k_wise_family.cpp)
target_link_libraries(test_k_wise_family PRIVATE gtest_main QSBD_K_WISE_FAMILY QSBD_GLOBAL_GEN QSBD_COMMUM_HEADER QSBD_UTILS)
target_include_directories(test_k_wise_family PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_k_wise_family COMMAND test_k_wise_family)

add_executable(test_count_sketch test_count_sketch.cpp)
target_link_libraries(test_count_sketch PRIVATE gtest_main QSBD_COUNT_SKETCH QSBD_STREAM_MAKER QSBD_UTILS)
target_include_directories(test_count_sketch PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_count_sketch COMMAND test_count_sketch)

add_executable(test_dcs test_dcs.cpp)
target_link_libraries(test_dcs PRIVATE gtest_main QSBD_DCS QSBD_STREAM_MAKER QSBD_DCS_FACTORY QSBD_UTILS)
target_include_directories(test_dcs PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_dcs COMMAND test_dcs)

add_executable(test_gk test_gk.cpp)
target_link_libraries(test_gk PRIVATE gtest_main QSBD_GK QSBD_UTILS)
target_include_directories(test_gk PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_gk COMMAND test_gk)

add_executable(test_q_digest test_q_digest.cpp)
target_link_libraries(test_q_digest PRIVATE gtest_main QSBD_Q_DIGEST QSBD_UTILS)
target_include_directories(test_q_digest PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_q_digest COMMAND test_q_digest)

add_executable(test_quadtree test_quadtree.cpp)
target_link_libraries(test_quadtree PRIVATE gtest_main QSBD_QUADTREE QSBD_STREAM_MAKER QSBD_UTILS)
target_include_directories(test_quadtree PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_quadtree COMMAND test_quadtree)

add_executable(test_quantile_quadtree test_quantile_quadtree.cpp)
target_link_libraries(test_quantile_quadtree PRIVATE gtest_main QSBD_QUANTILE_QUADTREE QSBD_STREAM_MAKER QSBD_COUNT_SKETCH QSBD_KLL_FACTORY QSBD_GK_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_DCS_FACTORY QSBD_MEMORY_TRACKER QSBD_TIMER QSBD_DEBUG QSBD_UTILS)
target_include_directories(test_quantile_quadtree PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_quantile_quadtree COMMAND test_quantile_quadtree)

add_executable(test_log test_log.cpp)
target_link_libraries(test_log PRIVATE gtest_main QSBD_LOG)
target_include_directories(test_log PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME test_log COMMAND test_log)

add_executable(tester tester.cpp)
target_link_libraries(tester PRIVATE QSBD_CONFIG QSBD_UTILS QSBD_DEBUG)
target_include_directories(tester PRIVATE ${QSBD_DIR_SOURCE})
add_test(NAME tester COMMAND tester)
