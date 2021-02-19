
set(LIBS_SOURCE "${CMAKE_SOURCE_DIR}/src")

add_executable(test_stream_maker test_stream_maker.cpp)
target_link_libraries(test_stream_maker PRIVATE gtest_main STREAM_MAKER GLOBAL_GEN COMMUM_HEADER PBPLOTS_PBPLOTTER)
target_include_directories(test_stream_maker PRIVATE ${LIBS_SOURCE})

add_test(NAME test_stream_maker COMMAND test_stream_maker)

add_executable(test_kll test_kll.cpp)
target_link_libraries(test_kll PRIVATE gtest_main KLL STREAM_MAKER GLOBAL_GEN COMMUM_HEADER)
target_include_directories(test_kll PRIVATE ${LIBS_SOURCE})

add_test(NAME test_kll COMMAND test_kll)

add_executable(test_k_wise_family test_k_wise_family.cpp)
target_link_libraries(test_k_wise_family PRIVATE gtest_main K_WISE_FAMILY GLOBAL_GEN COMMUM_HEADER)
target_include_directories(test_k_wise_family PRIVATE ${LIBS_SOURCE})

add_test(NAME test_k_wise_family COMMAND test_k_wise_family)

add_executable(test_count_sketch test_count_sketch.cpp)
target_link_libraries(test_count_sketch PRIVATE gtest_main COUNT_SKETCH STREAM_MAKER)
target_include_directories(test_count_sketch PRIVATE ${LIBS_SOURCE})

add_test(NAME test_count_sketch COMMAND test_count_sketch)

add_executable(test_dcs test_dcs.cpp)
target_link_libraries(test_dcs PRIVATE gtest_main DCS STREAM_MAKER PBPLOTS_PBPLOTTER)
target_include_directories(test_dcs PRIVATE ${LIBS_SOURCE})

add_test(NAME test_dcs COMMAND test_dcs)

add_executable(test_pbplotter test_pbplotter.cpp)
target_link_libraries(test_pbplotter PRIVATE gtest_main PBPLOTS_PBPLOTTER COMMUM_HEADER)
target_include_directories(test_pbplotter PRIVATE ${LIBS_SOURCE})

add_executable(test_quadtree test_quadtree.cpp)
target_link_libraries(test_quadtree PRIVATE gtest_main QUADTREE STREAM_MAKER)
target_include_directories(test_quadtree PRIVATE ${LIBS_SOURCE})

add_test(NAME test_quadtree COMMAND test_quadtree)

add_executable(test_quantile_quadtree test_quantile_quadtree.cpp)
target_link_libraries(test_quantile_quadtree PRIVATE gtest_main QUANTILE_QUADTREE STREAM_MAKER COUNT_SKETCH)
target_include_directories(test_quantile_quadtree PRIVATE ${LIBS_SOURCE})

add_test(NAME test_quantile_quadtree COMMAND test_quantile_quadtree)