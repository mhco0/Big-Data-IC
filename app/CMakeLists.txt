set(QSBD_DIR_SOURCE "${CMAKE_SOURCE_DIR}/src")
set(THIRD_PARTY_DIR_SOURCE "${CMAKE_SOURCE_DIR}/thirdparty")

add_executable(stream_creator "stream_creator/stream_creator.cpp")
target_include_directories(stream_creator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(stream_creator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(stream_creator PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_DEBUG)

add_executable(query_creator "query_creator/query_creator.cpp")
target_include_directories(query_creator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(query_creator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(query_creator PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_DEBUG)

add_executable(quantile_quadtree_experiment "quantile_quadtree_experiment/quantile_quadtree_experiment.cpp")
target_include_directories(quantile_quadtree_experiment PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(quantile_quadtree_experiment PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(quantile_quadtree_experiment PRIVATE QSBD_TIMER QSBD_QUANTILE_QUADTREE QSBD_GK_FACTORY QSBD_KLL_FACTORY QSBD_DCS_FACTORY QSBD_Q_DIGEST_FACTORY QSBD_UTILS QSBD_LOG QSBD_DEBUG QSBD_MEMORY_TRACKER)
