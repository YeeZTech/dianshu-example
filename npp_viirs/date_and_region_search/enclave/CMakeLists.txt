set(T_SRCS eparser.cpp csv_parser.cpp sgx_plugin.cpp)

add_enclave_library(date_and_region_search SRCS ${T_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  )

target_link_libraries(date_and_region_search stbox_common_t stbox_channel_t ypc_analyzer_t)


enclave_sign(date_and_region_search KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/date_and_region_search.signed.so DESTINATION lib)

