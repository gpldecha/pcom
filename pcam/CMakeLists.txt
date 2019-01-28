find_package(OpenCV REQUIRED)

include_directories(${PCOM_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(pcam SHARED src/video_capture.cpp)
target_link_libraries(pcam ${OpenCV_LIBS} ${Boost_LIBRARIES} -lboost_thread -lpthread)
