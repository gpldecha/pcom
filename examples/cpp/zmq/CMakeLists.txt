include_directories(${PCOM_INCLUDE_DIR})

FIND_FILE(ZMQLIB libzmq.so /usr/lib/x86_64-linux-gnu/)
IF(NOT ZMQLIB)
    MESSAGE(SEND_ERROR "Ah.. Cannot find library libzmq.so.")
ENDIF(NOT ZMQLIB)

find_package(jsoncpp)

add_executable(zmq_publisher src/zmq_publisher.cpp)
target_link_libraries(zmq_publisher ${ZMQLIB} jsoncpp)

add_executable(zmq_subscriber src/zmq_subscriber.cpp)
target_link_libraries(zmq_subscriber ${ZMQLIB} jsoncpp)
