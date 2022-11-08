set(districtT_SRCS eparser_district.cpp sgx_plugin.cpp)

add_enclave_library(ip_search_district SRCS ${districtT_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  )

target_link_libraries(ip_search_district stbox_common_t stbox_channel_t ypc_analyzer_t)


enclave_sign(ip_search_district KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/ip_search_district.signed.so DESTINATION lib)



set(streetT_SRCS eparser_street.cpp sgx_plugin.cpp)

add_enclave_library(ip_search_street SRCS ${streetT_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  )

target_link_libraries(ip_search_street stbox_common_t stbox_channel_t ypc_analyzer_t)


enclave_sign(ip_search_street KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/ip_search_street.signed.so DESTINATION lib)
