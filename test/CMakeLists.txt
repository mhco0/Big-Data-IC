
add_executable(test_kll test_kll.cpp)

target_link_libraries(test_kll PRIVATE gtest_main KLL GLOBAL_GEN COMMUM_HEADER)

add_test(NAME test_kll COMMAND test_kll)
