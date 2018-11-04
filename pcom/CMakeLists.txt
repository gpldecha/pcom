set(PCOM_INC
${PCOM_INCLUDE_DIR}/pcom/pcom_circular_buffer.h
${PCOM_INCLUDE_DIR}/pcom/pcom_producer.h
${PCOM_INCLUDE_DIR}/pcom/pcom_consumer.h
${PCOM_INCLUDE_DIR}/pcom/pcom_types.h
${PCOM_INCLUDE_DIR}/pcom/pcom_utils.h)


include_directories(${PCOM_INCLUDE_DIR})

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${PCOM_INC})

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/
        DESTINATION "/usr/local/include"
        PATTERN "gmock/*" EXCLUDE )
