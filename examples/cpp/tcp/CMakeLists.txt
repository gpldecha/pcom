include_directories(${PCOM_INCLUDE_DIR})

add_executable(tcp_producer src/producer.cpp)
target_link_libraries(tcp_producer ${Boost_LIBRARIES} -lboost_thread -lpthread)

add_executable(tcp_consumer src/consumer.cpp)
target_link_libraries(tcp_consumer ${Boost_LIBRARIES} -lboost_thread -lpthread)
