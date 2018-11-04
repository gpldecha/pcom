
include_directories(${PCOM_INCLUDE_DIR})

add_executable(test_producer test_producer.cpp)

target_link_libraries(test_producer gtest_main ${Boost_LIBRARIES})


add_test(NAME test_producer COMMAND test_producer)
