
include_directories(${PCOM_INCLUDE_DIR})

add_executable(test_circular_buffer test_circular_buffer.cpp)

target_link_libraries(test_circular_buffer gtest_main ${Boost_LIBRARIES})


add_test(NAME test_circular_buffer COMMAND test_circular_buffer)
