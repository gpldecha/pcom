cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pcom)

set(CMAKE_CXX_FLAGS "-O1 -O2 -O3 -std=c++11 -Wall -Wno-write-strings")        ## Optimize
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(PCOM_INCLUDE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/pcom/include)
find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(pcom)

# Examples

add_subdirectory(examples)

# Tests

include(cmake/gtest.cmake)
fetch_googletest(
	${PROJECT_SOURCE_DIR}/cmake
    	${PROJECT_BINARY_DIR}/googletest)

enable_testing()
add_subdirectory(test)
