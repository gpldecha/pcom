find_package(PythonInterp)
find_package(PythonLibs)
find_package(Boost COMPONENTS python system REQUIRED)


set(PYTHON_INCLUDES /usr/include/python2.7/)
include_directories(${PYTHON_INCLUDES} ${PCOM_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

message(STATUS "PCOM_INCLUDE_DIR: ${PCOM_INCLUDE_DIR}")
# BUILD PYTHON MODULES

message(STATUS "PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")

python_add_module(pypcom src/pcom_module.cpp)
target_link_libraries(pypcom ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} rt pthread)
