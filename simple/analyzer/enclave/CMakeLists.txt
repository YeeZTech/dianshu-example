set(T_SRCS eparser.cpp simple_parser.cpp sgx_plugin.cpp)

add_enclave_library(simple_parser SRCS ${T_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  )

target_link_libraries(simple_parser stbox_common_t stbox_channel_t ypc_analyzer_t)


enclave_sign(simple_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/simple_parser.signed.so DESTINATION lib)



#set(minT_SRCS eparser_min.cpp simple_parser.cpp sgx_plugin.cpp)

#add_enclave_library(min_parser SRCS ${minT_SRCS}
  #EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  #EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  #)

#target_link_libraries(min_parser stbox_common_t stbox_channel_t ypc_analyzer_t)


#enclave_sign(min_parser KEY enclave_private.pem
  #CONFIG enclave.config.xml)
#install(FILES ${PROJECT_SOURCE_DIR}/lib/min_parser.signed.so DESTINATION lib)



#set(maxT_SRCS eparser_max.cpp simple_parser.cpp sgx_plugin.cpp)

#add_enclave_library(max_parser SRCS ${maxT_SRCS}
  #EDL ${PROJECT_SOURCE_DIR}/core/edl/eparser.edl
  #EDL_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/core/edl/:${PROJECT_SOURCE_DIR}/stbox/src/:${PROJECT_SOURCE_DIR}/stbox/src/tsgx/crypto/
  #)

#target_link_libraries(max_parser stbox_common_t stbox_channel_t ypc_analyzer_t)


#enclave_sign(max_parser KEY enclave_private.pem
  #CONFIG enclave.config.xml)
#install(FILES ${PROJECT_SOURCE_DIR}/lib/max_parser.signed.so DESTINATION lib)
