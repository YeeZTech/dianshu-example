
set(T_SRCS eparser.cpp)

add_enclave_library(gdp_query_parser SRCS ${T_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/include/ypc/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/include/ypc/edl/:${PROJECT_SOURCE_DIR}/include/ypc/stbox/
  )

target_link_libraries(gdp_query_parser stbox_common_t stbox_channel_t ypc_analyzer_t ypc_stdeth_t)

enclave_sign(gdp_query_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/gdp_query_parser.signed.so DESTINATION lib COMPONENT c_example OPTIONAL)
