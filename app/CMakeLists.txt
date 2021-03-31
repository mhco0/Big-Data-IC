set(QSBD_DIR_SOURCE "${CMAKE_SOURCE_DIR}/src")
set(THIRD_PARTY_DIR_SOURCE "${CMAKE_SOURCE_DIR}/thirdparty")

add_executable(stream_creator "stream_creator/stream_creator.cpp")
target_include_directories(stream_creator PRIVATE ${QSBD_DIR_SOURCE})
target_include_directories(stream_creator PRIVATE ${THIRD_PARTY_DIR_SOURCE})
target_link_libraries(stream_creator PRIVATE nlohmann_json QSBD_STREAM_MAKER QSBD_UTILS QSBD_LOG QSBD_CONFIG QSBD_DEBUG)