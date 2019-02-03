set(KCOM_INC
${KCOM_INCLUDE_DIR}/kcom/shared_memory.h
${KCOM_INCLUDE_DIR}/kcom/kcom_types.h)


include_directories(${KCOM_INCLUDE_DIR})

add_library(kcom SHARED src/shared_memory)
