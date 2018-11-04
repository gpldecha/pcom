find_package(Threads REQUIRED)
include(ExternalProject)

add_subdirectory(test_producer)
add_subdirectory(test_circular_buffer)
