include_directories(${PCAM_INCLUDE_DIR})
include_directories(${PCOM_INCLUDE_DIR})

find_package( OpenCV REQUIRED )

add_executable(usb_cam_producer src/usb_cam_producer.cpp)
target_link_libraries(usb_cam_producer pcam ${Boost_LIBRARIES} ${OpenCV_LIBS} -lboost_thread -lpthread)

add_executable(video_consumer src/video_consumer.cpp)
target_link_libraries(video_consumer pcam ${Boost_LIBRARIES} ${OpenCV_LIBS} -lboost_thread -lpthread)

add_executable(test_serialisation src/test_serialisation.cpp)
target_link_libraries(test_serialisation pcam ${Boost_LIBRARIES} ${OpenCV_LIBS} -lboost_thread -lpthread)
