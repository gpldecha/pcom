include_directories(${PCOM_INCLUDE_DIR})

add_executable(ipc_posix_consumer src/consumer.cpp)
target_link_libraries(ipc_posix_consumer ${Boost_LIBRARIES} rt pthread)

add_executable(ipc_posix_producer src/producer.cpp)
target_link_libraries(ipc_posix_producer ${Boost_LIBRARIES} rt pthread)
