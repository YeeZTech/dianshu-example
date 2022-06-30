
add_enclave_library(clickhouse_query SRCS eparser.cpp
  EDL ${PROJECT_SOURCE_DIR}/ypc/core/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ypc/core/edl/:${PROJECT_SOURCE_DIR}/ypc/stbox/src/
)

target_link_libraries(clickhouse_query stbox_common_t stbox_channel_t ypc_analyzer_t ypc_stdeth_t)

enclave_sign(clickhouse_query KEY enclave_private.pem
  CONFIG enclave.config.xml)
