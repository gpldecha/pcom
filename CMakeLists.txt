cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pcom)

set(CMAKE_CXX_FLAGS "-O3 -std=c++14 -Wall -Wno-write-strings")        ## Optimize
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(COMPILE_TESTS false)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

set(PCOM_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/pcom/include)
find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(pcom)
#add_subdirectory(pypcom)

add_subdirectory(pcam)

# Examples

add_subdirectory(examples)

# Tests

if(COMPILE_TESTS)
	include(cmake/gtest.cmake)
	fetch_googletest(${PROJECT_SOURCE_DIR}/cmake ${PROJECT_BINARY_DIR}/googletest)

	enable_testing()
	add_subdirectory(test)
endif(COMPILE_TESTS)
