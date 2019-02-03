include_directories(${KCOM_INCLUDE_DIR})

add_executable(robot src/robot.cpp)
target_link_libraries(robot ${Boost_LIBRARIES} kcom rt pthread)

add_executable(controller src/controller.cpp)
target_link_libraries(controller ${Boost_LIBRARIES} kcom rt pthread)
