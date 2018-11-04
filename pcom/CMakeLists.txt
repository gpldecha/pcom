

message(STATUS "PCOM_INCLUDE_DIR: ${PCOM_INCLUDE_DIR}")

set(PCOM_INC
${PCOM_INCLUDE_DIR}/pcom/ipc_posix_circular_buffer
${PCOM_INCLUDE_DIR}/pcom/pcom_producer.h
${PCOM_INCLUDE_DIR}/pcom/pcom_consumer.h
${PCOM_INCLUDE_DIR}/pcom/pcom_types.h
${PCOM_INCLUDE_DIR}/pcom/utils.h)


include_directories(${PCOM_INCLUDE_DIR})

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${PCOM_INC})
target_include_directories(${PROJECT_NAME} INTERFACE
    "${PROJECT_SOURCE_DIR}/lib1"
)
